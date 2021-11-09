#ifndef LIBCEREBELLUM_H
#define LIBCEREBELLUM_H

#include <stdlib.h>

typedef struct {
	size_t *layers;
	size_t nlayers;
	double rate;
	double *weights;
	double *biases;
	double *vals;
	size_t nws;
	size_t nbs;
	double *b1;
	double *b2;
} net_t;

void scale(double c, double *const a, size_t len);
void add(const double *const a1, double *const a2, size_t len);
void sub(const double *const a1, const double *const a2, double *const out,
	size_t len);
double dot(const double *const a1, const double *const a2, size_t len);
double dist(const double *const a1, const double *const a2, size_t len);

void mult(const double *const restrict m1, const double *const restrict m2,
	double *const restrict mf, size_t rows, size_t c, size_t cols);
void multV(const double *const m, const double *const v,
	double *const vf, size_t rows, size_t cols);
void elemMult(const double *const a1, double *const a2, size_t len);
void map(double (*const f)(double), double *const a, size_t len);
void zip(double c, const double *const a, double *const b, size_t len);
double dsquare(const double *const a, const double *const b, size_t len);

void initNet(net_t *const net, double (*ws)(size_t), double (*bs)(size_t));
void netFromFile(net_t *const net, const char *const fn);
void saveNet(const net_t *const net, const char *const fn);
void delNet(net_t *const net);
double sigm(double x);
void printNet(const net_t *const net);
double *feedforward(const net_t *const net, const double *const in);
void train(net_t *const net, const double *const act, const double *const in);

#endif
