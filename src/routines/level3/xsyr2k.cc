
// =================================================================================================
// This file is part of the CLBlast project. The project is licensed under Apache Version 2.0. This
// project loosely follows the Google C++ styleguide and uses a tab-size of two spaces and a max-
// width of 100 characters per line.
//
// Author(s):
//   Cedric Nugteren <www.cedricnugteren.nl>
//
// This file implements the Xsyr2k class (see the header for information about the class).
//
// =================================================================================================

#include "internal/routines/level3/xsyr2k.h"

#include <string>
#include <vector>

namespace clblast {
// =================================================================================================

// Specific implementations to get the memory-type based on a template argument
template <> const Precision Xsyr2k<float>::precision_ = Precision::kSingle;
template <> const Precision Xsyr2k<double>::precision_ = Precision::kDouble;
template <> const Precision Xsyr2k<float2>::precision_ = Precision::kComplexSingle;
template <> const Precision Xsyr2k<double2>::precision_ = Precision::kComplexDouble;

// =================================================================================================

// Constructor: forwards to base class constructor
template <typename T>
Xsyr2k<T>::Xsyr2k(CommandQueue &queue, Event &event):
    Routine(queue, event, "SYR2K", {"Copy","Pad","Transpose","PadTranspose","Xgemm"}, precision_) {
  source_string_ =
    #include "../../kernels/copy.opencl"
    #include "../../kernels/pad.opencl"
    #include "../../kernels/transpose.opencl"
    #include "../../kernels/padtranspose.opencl"
    #include "../../kernels/xgemm.opencl"
  ;
}

// =================================================================================================

// The main routine
template <typename T>
StatusCode Xsyr2k<T>::DoSyr2k(const Layout layout, const Triangle triangle, const Transpose ab_transpose,
                              const size_t n, const size_t k,
                              const T alpha,
                              const Buffer &a_buffer, const size_t a_offset, const size_t a_ld,
                              const Buffer &b_buffer, const size_t b_offset, const size_t b_ld,
                              const T beta,
                              const Buffer &c_buffer, const size_t c_offset, const size_t c_ld) {

  // Makes sure all dimensions are larger than zero
  if ((n == 0) || (k == 0) ) { return StatusCode::kInvalidDimension; }

  // Computes whether or not the matrices are transposed in memory. This is based on their layout
  // (row or column-major) and whether or not they are requested to be pre-transposed.
  auto ab_rotated = (layout == Layout::kColMajor && ab_transpose != Transpose::kNo) ||
                    (layout == Layout::kRowMajor && ab_transpose == Transpose::kNo);
  auto c_rotated = (layout == Layout::kRowMajor);

  // Computes the first and second dimensions of the A and B matrices taking the layout into account
  auto ab_one = (ab_rotated) ? k : n;
  auto ab_two = (ab_rotated) ? n : k;

  // Tests the matrices (A, B, C) for validity, first from a perspective of the OpenCL buffers and
  // their sizes, and then from a perspective of parameter values (e.g. n, k). Tests whether the
  // OpenCL buffers are valid and non-zero and whether the OpenCL buffers have sufficient storage
  // space. Also tests that the leading dimensions of:
  //    matrix A cannot be less than N when rotated, or less than K when not-rotated
  //    matrix B cannot be less than N when rotated, or less than K when not-rotated
  //    matrix C cannot be less than N
  auto status = TestMatrixA(ab_one, ab_two, a_buffer, a_offset, a_ld, sizeof(T));
  if (ErrorIn(status)) { return status; }
  status = TestMatrixB(ab_one, ab_two, b_buffer, b_offset, b_ld, sizeof(T));
  if (ErrorIn(status)) { return status; }
  status = TestMatrixC(n, n, c_buffer, c_offset, c_ld, sizeof(T));
  if (ErrorIn(status)) { return status; }

  // Calculates the ceiled versions of n and k
  auto n_ceiled = Ceil(n, db_["NWG"]);
  auto k_ceiled = Ceil(k, db_["KWG"]);

  // Decides which kernel to run: the upper-triangular or lower-triangular version
  auto kernel_name = (triangle == Triangle::kUpper) ? "XgemmUpper" : "XgemmLower";

  // The padded/transposed input/output matrices: if memory allocation fails, throw an exception
  try {

    // Loads the program from the database
    auto& program = GetProgramFromCache();

    // Determines whether or not temporary matrices are needed
    auto a_no_temp = ab_one == n_ceiled && ab_two == k_ceiled && a_ld == n_ceiled && a_offset == 0 &&
                     ab_rotated == false;
    auto b_no_temp = ab_one == n_ceiled && ab_two == k_ceiled && b_ld == n_ceiled && b_offset == 0 &&
                     ab_rotated == false;

    // Creates the temporary matrices
    auto a_temp = (a_no_temp) ? a_buffer : Buffer(context_, CL_MEM_READ_WRITE, k_ceiled*n_ceiled*sizeof(T));
    auto b_temp = (b_no_temp) ? b_buffer : Buffer(context_, CL_MEM_READ_WRITE, k_ceiled*n_ceiled*sizeof(T));
    auto c_temp = Buffer(context_, CL_MEM_READ_WRITE, n_ceiled*n_ceiled*sizeof(T));

    // Runs the pre-processing kernels. This transposes the matrices A and B, but also pads zeros to
    // to fill it up until it reaches a certain multiple of size (kernel parameter dependent). In
    // case nothing has to be done, these kernels can be skipped.
    if (!a_no_temp) {
      status = PadCopyTransposeMatrix(ab_one, ab_two, a_ld, a_offset, a_buffer,
                                      n_ceiled, k_ceiled, n_ceiled, 0, a_temp,
                                      program, true, ab_rotated, false);
      if (ErrorIn(status)) { return status; }
    }
    if (!b_no_temp) {
      status = PadCopyTransposeMatrix(ab_one, ab_two, b_ld, b_offset, b_buffer,
                                      n_ceiled, k_ceiled, n_ceiled, 0, b_temp,
                                      program, true, ab_rotated, false);
      if (ErrorIn(status)) { return status; }
    }

    // Furthermore, also creates a (possibly padded) copy of matrix C, since it is not allowed to
    // modify the other triangle.
    status = PadCopyTransposeMatrix(n, n, c_ld, c_offset, c_buffer,
                                    n_ceiled, n_ceiled, n_ceiled, 0, c_temp,
                                    program, true, c_rotated, false);
    if (ErrorIn(status)) { return status; }

    // Retrieves the XgemmUpper or XgemmLower kernel from the compiled binary
    try {
      auto kernel = Kernel(program, kernel_name);

      // Sets the kernel arguments
      kernel.SetArgument(0, static_cast<int>(n_ceiled));
      kernel.SetArgument(1, static_cast<int>(k_ceiled));
      kernel.SetArgument(2, alpha);
      kernel.SetArgument(3, beta);
      kernel.SetArgument(4, a_temp());
      kernel.SetArgument(5, b_temp());
      kernel.SetArgument(6, c_temp());

      // Computes the global and local thread sizes
      auto global = std::vector<size_t>{
        (n_ceiled * db_["MDIMC"]) / db_["MWG"],
        (n_ceiled * db_["NDIMC"]) / db_["NWG"]
      };
      auto local = std::vector<size_t>{db_["MDIMC"], db_["NDIMC"]};

      // Launches the kernel
      status = RunKernel(kernel, global, local);
      if (ErrorIn(status)) { return status; }

      // Swaps the arguments for matrices A and B, and sets 'beta' to 1
      auto one = static_cast<T>(1);
      kernel.SetArgument(3, one);
      kernel.SetArgument(4, b_temp());
      kernel.SetArgument(5, a_temp());

      // Runs the kernel again
      status = RunKernel(kernel, global, local);
      if (ErrorIn(status)) { return status; }

      // Runs the post-processing kernel
      auto upper = (triangle == Triangle::kUpper);
      auto lower = (triangle == Triangle::kLower);
      status = PadCopyTransposeMatrix(n_ceiled, n_ceiled, n_ceiled, 0, c_temp,
                                      n, n, c_ld, c_offset, c_buffer,
                                      program, false, c_rotated, false, upper, lower, false);
      if (ErrorIn(status)) { return status; }

      // Successfully finished the computation
      return StatusCode::kSuccess;
    } catch (...) { return StatusCode::kInvalidKernel; }
  } catch (...) { return StatusCode::kTempBufferAllocFailure; }
}

// =================================================================================================

// Compiles the templated class
template class Xsyr2k<float>;
template class Xsyr2k<double>;
template class Xsyr2k<float2>;
template class Xsyr2k<double2>;

// =================================================================================================
} // namespace clblast
