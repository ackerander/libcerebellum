#include "cerebellum.h"
#include <string.h>

inline void
zeroVec(size_t len, vec_t *const restrict v)
{
	v->arr = calloc(len, sizeof(double));
	v->len = len;
}

inline void
newVec(size_t len, vec_t *const restrict v)
{
	v->arr = malloc(len * sizeof(double));
	v->len = len;
}

int
cpyVec(const vec_t *const restrict v1, vec_t *const restrict v2)
{
	if (v2->arr)
		return -1;
	v2->arr = malloc(v1->len * sizeof(double));
	memcpy(v2->arr, v1->arr, v1->len * sizeof(double));
	v2->len = v1->len;
	return 0;
}

inline void
rmVec(vec_t *const restrict v)
{
	free(v->arr);
	v->arr = 0;
	v->len = 0;
}

void
scaleVec(double c, const vec_t *const restrict v)
{
	for (size_t i = 0; i < v->len; ++i)
		v->arr[i] *= c;
}

int
vecAdd(const vec_t *const restrict v1, const vec_t *const restrict v2)
{
	if (v1->len != v2->len)
		return -1;
	for (size_t i = 0; i < v1->len; ++i)
		v2->arr[i] += v1->arr[i];
	return 0;
}

double
dot(const vec_t *const restrict v1, const vec_t *const restrict v2)
{
	double acc = 0.0f;

	if (v1->len != v2->len)
		return 0.0f;
	for (size_t i = 0; i < v1->len; ++i)
		acc += v1->arr[i] * v2->arr[i];
	return acc;
}

inline void
zeroMatrix(size_t rows, size_t cols, matrix_t *const restrict m)
{
	m->arr = calloc(rows * cols, sizeof(double));
	m->rows = rows;
	m->cols = cols;
}

inline void
newMatrix(size_t rows, size_t cols, matrix_t *const restrict m)
{
	m->arr = malloc(rows * cols * sizeof(double));
	m->rows = rows;
	m->cols = cols;
}

int
cpyMatrix(const matrix_t *const restrict m1, matrix_t *const restrict m2)
{
	if (m2->arr)
		return -1;
	m2->arr = malloc(m1->rows * m1->cols * sizeof(double));
	memcpy(m2->arr, m1->arr, m1->rows * m1->cols * sizeof(double));
	m2->rows = m1->rows;
	m2->cols = m2->cols;
	return 0;
}

inline void
rmMatrix(matrix_t *const restrict m)
{
	free(m->arr);
	m->arr = 0;
	m->rows = m->cols = 0;
}

void
scaleMatrix(double c, const matrix_t *const restrict m)
{
	for (size_t i = 0; i < m->rows * m->cols; ++i)
		m->arr[i] *= c;
}

int
matrixAdd(const matrix_t *const restrict m1, const matrix_t *const restrict m2)
{
	if (m1->rows != m2->rows || m1->cols != m2->cols)
		return -1;
	for (size_t i = 0; i < m1->rows * m1->cols; ++i)
		m2->arr[i] += m1->arr[i];
	return 0;
}

int
mult(const matrix_t *const restrict m1, const matrix_t *const restrict m2, matrix_t *const restrict mf)
{
	size_t row, col;

	if (m1->cols != m2->rows || mf->rows != m1->rows || mf->cols != m2->cols)
		return -1;
	for (size_t i = 0; i < m1->rows * m2->cols; ++i) {
		row = i / m2->cols;
		col = i % m2->cols;
		mf->arr[i] = m1->arr[row * m1->cols] * m2->arr[col];
		for (size_t j = 1; j < m1->cols; ++j)
			mf->arr[i] += m1->arr[row * m1->cols + j] * m2->arr[col + j * m2->cols];
	}
	return 0;
}
