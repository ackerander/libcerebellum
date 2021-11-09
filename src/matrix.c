#include "cerebellum.h"
#include <math.h>

inline void
scale(double c, double *const restrict a, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		a[i] *= c;
}

inline void
add(const double *const restrict a1, double *const restrict a2,
	size_t len)
{
	for (size_t i = 0; i < len; ++i)
		a2[i] += a1[i];
}

inline void
sub(const double *const restrict a1, const double *const restrict a2,
	double *const restrict out, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		out[i] = a1[i] - a2[i];
}

double
dot(const double *const restrict a1, const double *const restrict a2,
	size_t len)
{
	double acc = 0.0f;

	for (size_t i = 0; i < len; ++i)
		acc += a1[i] * a2[i];
	return acc;
}

double
dist(const double *const restrict a1, const double *const restrict a2,
	size_t len)
{
	double acc = 0.0f;

	for (size_t i = 0; i < len; ++i)
		acc += (a1[i] - a2[i]) * (a1[i] - a2[i]);
	return sqrt(acc);
}

void
mult(const double *const restrict m1, const double *const restrict m2,
	double *const restrict mf, size_t rows, size_t c, size_t cols)
{
	size_t row, col;

	for (size_t i = 0; i < rows * cols; ++i) {
		row = i / cols;
		col = i % cols;
		mf[i] = m1[row * c] * m2[col];
		for (size_t j = 1; j < c; ++j)
			mf[i] += m1[row * c + j] * m2[col + j * cols];
	}
}


void
multV(const double *const restrict m, const double *const restrict v,
	double *const restrict vf, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i) {
		vf[i] = m[i * cols] * v[0];
		for (size_t j = 1; j < cols; ++j)
			vf[i] += m[i * cols + j] * v[j];
	}
}

inline void
elemMult(const double *const a1, double *const a2, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		a2[i] *= a1[i];
}

inline void
map(double (*const f)(double), double *const restrict a, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		a[i] = f(a[i]);
}

double
dsquare(const double *const restrict a, const double *const restrict b, size_t len)
{
	double acc = 0;

	for (size_t i = 0; i < len; ++i)
		acc += (a[i] - b[i]) * (a[i] - b[i]);
	return acc;
}
