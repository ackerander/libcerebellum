#ifndef LIBCEREBELLUM_H
#define LIBCEREBELLUM_H

#include <stdlib.h>

typedef struct {
	double *arr;
	size_t len;
} vec_t;

typedef struct {
	double *arr;
	size_t rows;
	size_t cols;
} matrix_t;

void zeroVec(size_t len, vec_t *const v);
void newVec(size_t len, vec_t *const v);
int cpyVec(const vec_t *const v1, vec_t *const v2);
void rmVec(vec_t *const v);
void scaleVec(double c, const vec_t *const v);
int vecAdd(const vec_t *const v1, const vec_t *const v2);
double dot(const vec_t *const v1, const vec_t *const v2);

void zeroMatrix(size_t rows, size_t cols, matrix_t *const m);
void newMatrix(size_t rows, size_t cols, matrix_t *const m);
int cpyMatrix(const matrix_t *const m1, matrix_t *const m2);
void rmMatrix(matrix_t *const m);
void scaleMatrix(double c, const matrix_t *const m);
int matrixAdd(const matrix_t *const m1, const matrix_t *const m2);
int mult(const matrix_t *const m1, const matrix_t *const m2, matrix_t *const mf);

#endif
