
// =================================================================================================
// This file is part of the CLBlast project. The project is licensed under Apache Version 2.0. This
// project loosely follows the Google C++ styleguide and uses a tab-size of two spaces and a max-
// width of 100 characters per line.
//
// Author(s):
//   Cedric Nugteren <www.cedricnugteren.nl>
//
// This file implements a wrapper around the clBLAS library, such that its routines can be called
// in a similar way as the CLBlast routines: using alpha and beta to determine the precision.
//
// =================================================================================================

#ifndef CLBLAST_TEST_WRAPPER_CLBLAS_H_
#define CLBLAST_TEST_WRAPPER_CLBLAS_H_

#include <clBLAS.h>

#include "internal/utilities.h"

namespace clblast {
// =================================================================================================
// BLAS level-1 (vector-vector) routines

// Calls {clblasSaxpy, clblasDaxpy, clblasCaxpy, clblasZaxpy} with the arguments forwarded.
clblasStatus clblasXaxpy(
  size_t n, float alpha,
  const cl_mem x_vec, size_t x_offset, size_t x_inc,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasSaxpy(n, alpha,
                       x_vec, x_offset, static_cast<int>(x_inc),
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXaxpy(
  size_t n, double alpha,
  const cl_mem x_vec, size_t x_offset, size_t x_inc,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDaxpy(n, alpha,
                       x_vec, x_offset, static_cast<int>(x_inc),
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXaxpy(
  size_t n, float2 alpha,
  const cl_mem x_vec, size_t x_offset, size_t x_inc,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    return clblasCaxpy(n, cl_alpha,
                       x_vec, x_offset, static_cast<int>(x_inc),
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXaxpy(
  size_t n, double2 alpha,
  const cl_mem x_vec, size_t x_offset, size_t x_inc,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    return clblasZaxpy(n, cl_alpha,
                       x_vec, x_offset, static_cast<int>(x_inc),
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}

// =================================================================================================
// BLAS level-2 (matrix-vector) routines

// Calls {clblasSgemv, clblasDgemv, clblasCgemv, clblasZgemv} with the arguments forwarded.
clblasStatus clblasXgemv(
  clblasOrder layout, clblasTranspose a_transpose, size_t m, size_t n, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem x_vec, size_t x_offset, size_t x_inc, float beta,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasSgemv(layout, a_transpose, m, n, alpha,
                       a_mat, a_offset, a_ld,
                       x_vec, x_offset, static_cast<int>(x_inc), beta,
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXgemv(
  clblasOrder layout, clblasTranspose a_transpose, size_t m, size_t n, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem x_vec, size_t x_offset, size_t x_inc, double beta,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDgemv(layout, a_transpose, m, n, alpha,
                       a_mat, a_offset, a_ld,
                       x_vec, x_offset, static_cast<int>(x_inc), beta,
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXgemv(
  clblasOrder layout, clblasTranspose a_transpose, size_t m, size_t n, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem x_vec, size_t x_offset, size_t x_inc, float2 beta,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_float2{{beta.real(), beta.imag()}};
    return clblasCgemv(layout, a_transpose, m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       x_vec, x_offset, static_cast<int>(x_inc), cl_beta,
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXgemv(
  clblasOrder layout, clblasTranspose a_transpose, size_t m, size_t n, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem x_vec, size_t x_offset, size_t x_inc, double2 beta,
  const cl_mem y_vec, size_t y_offset, size_t y_inc,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_double2{{beta.real(), beta.imag()}};
    return clblasZgemv(layout, a_transpose, m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       x_vec, x_offset, static_cast<int>(x_inc), cl_beta,
                       y_vec, y_offset, static_cast<int>(y_inc),
                       num_queues, queues, num_wait_events, wait_events, events);
}

// =================================================================================================
// BLAS level-3 (matrix-matrix) routines

// This calls {clblasSgemm, clblasDgemm, clblasCgemm, clblasZgemm} with the arguments forwarded.
clblasStatus clblasXgemm(
  clblasOrder layout, clblasTranspose a_transpose, clblasTranspose b_transpose,
  size_t m, size_t n, size_t k, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasSgemm(layout, a_transpose, b_transpose,
                       m, n, k, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXgemm(
  clblasOrder layout, clblasTranspose a_transpose, clblasTranspose b_transpose,
  size_t m, size_t n, size_t k, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDgemm(layout, a_transpose, b_transpose,
                       m, n, k, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXgemm(
  clblasOrder layout, clblasTranspose a_transpose, clblasTranspose b_transpose,
  size_t m, size_t n, size_t k, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_float2{{beta.real(), beta.imag()}};
    return clblasCgemm(layout, a_transpose, b_transpose,
                       m, n, k, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXgemm(
  clblasOrder layout, clblasTranspose a_transpose, clblasTranspose b_transpose,
  size_t m, size_t n, size_t k, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_double2{{beta.real(), beta.imag()}};
    return clblasZgemm(layout, a_transpose, b_transpose,
                       m, n, k, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasSsymm, clblasDsymm, clblasCsymm, clblasZsymm} with the arguments forwarded.
clblasStatus clblasXsymm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  size_t m, size_t n, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasSsymm(layout, side, triangle,
                       m, n, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsymm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  size_t m, size_t n, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDsymm(layout, side, triangle,
                       m, n, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsymm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  size_t m, size_t n, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_float2{{beta.real(), beta.imag()}};
    return clblasCsymm(layout, side, triangle,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsymm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  size_t m, size_t n, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_double2{{beta.real(), beta.imag()}};
    return clblasZsymm(layout, side, triangle,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasChemm, clblasZhemm} with the arguments forwarded.
clblasStatus clblasXhemm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  size_t m, size_t n, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_float2{{beta.real(), beta.imag()}};
    return clblasChemm(layout, side, triangle,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXhemm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  size_t m, size_t n, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_double2{{beta.real(), beta.imag()}};
    return clblasZhemm(layout, side, triangle,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasSsyrk, clblasDsyrk, clblasCsyrk, clblasZsyrk} with the arguments forwarded.
clblasStatus clblasXsyrk(
  clblasOrder layout, clblasUplo triangle, clblasTranspose a_transpose,
  size_t n, size_t k, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld, float beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasSsyrk(layout, triangle, a_transpose,
                       n, k, alpha,
                       a_mat, a_offset, a_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsyrk(
  clblasOrder layout, clblasUplo triangle, clblasTranspose a_transpose,
  size_t n, size_t k, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld, double beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDsyrk(layout, triangle, a_transpose,
                       n, k, alpha,
                       a_mat, a_offset, a_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsyrk(
  clblasOrder layout, clblasUplo triangle, clblasTranspose a_transpose,
  size_t n, size_t k, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld, float2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_float2{{beta.real(), beta.imag()}};
    return clblasCsyrk(layout, triangle, a_transpose,
                       n, k, cl_alpha,
                       a_mat, a_offset, a_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsyrk(
  clblasOrder layout, clblasUplo triangle, clblasTranspose a_transpose,
  size_t n, size_t k, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld, double2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_double2{{beta.real(), beta.imag()}};
    return clblasZsyrk(layout, triangle, a_transpose,
                       n, k, cl_alpha,
                       a_mat, a_offset, a_ld, cl_beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasCherk, clblasZherk} with the arguments forwarded.
clblasStatus clblasXherk(
  clblasOrder layout, clblasUplo triangle, clblasTranspose a_transpose,
  size_t n, size_t k, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld, float beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasCherk(layout, triangle, a_transpose,
                       n, k, alpha,
                       a_mat, a_offset, a_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXherk(
  clblasOrder layout, clblasUplo triangle, clblasTranspose a_transpose,
  size_t n, size_t k, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld, double beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasZherk(layout, triangle, a_transpose,
                       n, k, alpha,
                       a_mat, a_offset, a_ld, beta,
                       c_mat, c_offset, c_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasSsyr2k, clblasDsyr2k, clblasCsyr2k, clblasZsyr2k} with the arguments forwarded.
clblasStatus clblasXsyr2k(
  clblasOrder layout, clblasUplo triangle, clblasTranspose ab_transpose,
  size_t n, size_t k, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasSsyr2k(layout, triangle, ab_transpose,
                        n, k, alpha,
                        a_mat, a_offset, a_ld,
                        b_mat, b_offset, b_ld, beta,
                        c_mat, c_offset, c_ld,
                        num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsyr2k(
  clblasOrder layout, clblasUplo triangle, clblasTranspose ab_transpose,
  size_t n, size_t k, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDsyr2k(layout, triangle, ab_transpose,
                        n, k, alpha,
                        a_mat, a_offset, a_ld,
                        b_mat, b_offset, b_ld, beta,
                        c_mat, c_offset, c_ld,
                        num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsyr2k(
  clblasOrder layout, clblasUplo triangle, clblasTranspose ab_transpose,
  size_t n, size_t k, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_float2{{beta.real(), beta.imag()}};
    return clblasCsyr2k(layout, triangle, ab_transpose,
                        n, k, cl_alpha,
                        a_mat, a_offset, a_ld,
                        b_mat, b_offset, b_ld, cl_beta,
                        c_mat, c_offset, c_ld,
                        num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXsyr2k(
  clblasOrder layout, clblasUplo triangle, clblasTranspose ab_transpose,
  size_t n, size_t k, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double2 beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    auto cl_beta = cl_double2{{beta.real(), beta.imag()}};
    return clblasZsyr2k(layout, triangle, ab_transpose,
                        n, k, cl_alpha,
                        a_mat, a_offset, a_ld,
                        b_mat, b_offset, b_ld, cl_beta,
                        c_mat, c_offset, c_ld,
                        num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasCher2k, clblasZher2k} with the arguments forwarded.
clblasStatus clblasXher2k(
  clblasOrder layout, clblasUplo triangle, clblasTranspose ab_transpose,
  size_t n, size_t k, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, float beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    return clblasCher2k(layout, triangle, ab_transpose,
                        n, k, cl_alpha,
                        a_mat, a_offset, a_ld,
                        b_mat, b_offset, b_ld, beta,
                        c_mat, c_offset, c_ld,
                        num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXher2k(
  clblasOrder layout, clblasUplo triangle, clblasTranspose ab_transpose,
  size_t n, size_t k, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld, double beta,
  const cl_mem c_mat, size_t c_offset, size_t c_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    return clblasZher2k(layout, triangle, ab_transpose,
                        n, k, cl_alpha,
                        a_mat, a_offset, a_ld,
                        b_mat, b_offset, b_ld, beta,
                        c_mat, c_offset, c_ld,
                        num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasStrmm, clblasDtrmm, clblasCtrmm, clblasZtrmm} with the arguments forwarded.
clblasStatus clblasXtrmm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasStrmm(layout, side, triangle, a_transpose, diagonal,
                       m, n, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXtrmm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDtrmm(layout, side, triangle, a_transpose, diagonal,
                       m, n, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXtrmm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    return clblasCtrmm(layout, side, triangle, a_transpose, diagonal,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXtrmm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    return clblasZtrmm(layout, side, triangle, a_transpose, diagonal,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// This calls {clblasStrsm, clblasDtrsm, clblasCtrsm, clblasZtrsm} with the arguments forwarded.
clblasStatus clblasXtrsm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, float alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasStrsm(layout, side, triangle, a_transpose, diagonal,
                       m, n, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXtrsm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, double alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    return clblasDtrsm(layout, side, triangle, a_transpose, diagonal,
                       m, n, alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXtrsm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, float2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_float2{{alpha.real(), alpha.imag()}};
    return clblasCtrsm(layout, side, triangle, a_transpose, diagonal,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}
clblasStatus clblasXtrsm(
  clblasOrder layout, clblasSide side, clblasUplo triangle,
  clblasTranspose a_transpose, clblasDiag diagonal,
  size_t m, size_t n, double2 alpha,
  const cl_mem a_mat, size_t a_offset, size_t a_ld,
  const cl_mem b_mat, size_t b_offset, size_t b_ld,
  cl_uint num_queues, cl_command_queue *queues,
  cl_uint num_wait_events, const cl_event *wait_events, cl_event *events) {
    auto cl_alpha = cl_double2{{alpha.real(), alpha.imag()}};
    return clblasZtrsm(layout, side, triangle, a_transpose, diagonal,
                       m, n, cl_alpha,
                       a_mat, a_offset, a_ld,
                       b_mat, b_offset, b_ld,
                       num_queues, queues, num_wait_events, wait_events, events);
}

// =================================================================================================
} // namespace clblast

// CLBLAST_TEST_WRAPPER_CLBLAS_H_
#endif
