#include "cerebellum.h"
#include <math.h>
#include <stdio.h>

void
initNet(net_t *const restrict net, double (*ws)(size_t), double (*bs)(size_t))
{
/* Initializes weights, biases, nws, nbs & buffers if not already assigned */
	size_t max;

	if (!net->nws) {
		for (size_t i = 1; i < net->nlayers; ++i)
			net->nws += net->layers[i]*net->layers[i - 1];
	}
	if (!net->nbs) {
		for (size_t i = 1; i < net->nlayers; ++i)
			net->nbs += net->layers[i];
	}
	if (!net->b1 && !net->b2) {
		max = 0;
		for (size_t i = 0; i < net->nlayers; ++i)
			max = net->layers[i] > max ? net->layers[i] : max;
		net->b1 = malloc(max*sizeof(double));
		net->b2 = malloc(max*sizeof(double));
	}
	if (!net->weights) {
		net->weights = malloc(net->nws*sizeof(double));
		if (ws) {
			for (size_t i = 0; i < net->nws; ++i)
				net->weights[i] = ws(i);
		}
	}
	if (!net->biases) {
		net->biases = malloc(net->nbs*sizeof(double));
		if (bs) {
			for (size_t i = 0; i < net->nbs; ++i)
				net->biases[i] = bs(i);
		}
	}
	if (!net->vals)
		net->vals = malloc(net->nbs*sizeof(double));
}

void
netFromFile(net_t *const restrict net, const char *const restrict fn)
{
	FILE *file = fopen(fn, "rb");

	fread(&net->nlayers, sizeof(size_t), 1, file);
	net->layers = malloc(net->nlayers*sizeof(size_t));
	fread(net->layers, sizeof(size_t), net->nlayers, file);
	initNet(net, 0, 0);
	fread(net->weights, sizeof(double), net->nws, file);
	fread(net->biases, sizeof(double), net->nbs, file);
	fclose(file);
}

void
savenet(const net_t *const restrict net, const char *const restrict fn)
{
	FILE *out;

	out = fopen(fn, "wb");
	fwrite(&net->nlayers, sizeof(size_t), 1, out);
	fwrite(net->layers, sizeof(size_t), net->nlayers, out);
	fwrite(net->weights, sizeof(double), net->nws, out);
	fwrite(net->biases, sizeof(double), net->nbs, out);
	fclose(out);
}

void
delNet(net_t *const restrict net)
{
	free(net->weights);
	free(net->biases);
	free(net->vals);
	free(net->b1);
	free(net->b2);
}

inline double
sigm(double x)
{
	return 1/(1 + exp(-x));
}

inline void
zip(double c, const double *const restrict a, double *const restrict b,
	size_t len)
{
	for (size_t i = 0; i < len; ++i)
		b[i] = c*a[i]*b[i]*(1 - b[i]);
}

void
printNet(const net_t *const restrict net)
{
	size_t i = 0, n = 0;
	for (size_t j = 1; j < net->nlayers; ++j) {
		printf("Layer %ld\n", j);
		for (size_t k = 0; k < net->layers[j]; ++k) {
			for (size_t n = 0; n < net->layers[j - 1]; ++n)
				printf("%f, ", net->weights[i++]);
			printf("%f\n", net->biases[n++]);
		}
	}
}

void
multcTV(double c, const double *const restrict m, const double *const restrict v,
	double *const restrict vf, size_t rows, size_t cols)
{
	for (size_t i = 0; i < cols; ++i) {
		vf[i] = c * m[i] * v[0];
		for (size_t j = 1; j < rows; ++j)
			vf[i] += c * m[i + j * cols] * v[j];
	}
}

void addDotT(const double *const v1, const double *const v2, double *const restrict m,
	size_t len1, size_t len2)
{
	for (size_t i = 0; i < len1; ++i) {
		for (size_t j = 0; j < len2; ++j)
			m[i*len2 + j] += v1[i]*v2[j];
	}
}


double *
feedforward(const net_t *const restrict net, const double *const restrict in)
{
	size_t woff = 0, loff = 0, i;

	multV(net->weights, in, net->vals, net->layers[1], net->layers[0]);
	add(net->biases, net->vals, net->layers[1]);
	map(sigm, net->vals, net->layers[1]);
	for (i = 2; i < net->nlayers; ++i) {
		loff += net->layers[i - 1];
		woff += net->layers[i - 1] * net->layers[i - 2];
		multV(net->weights + woff, net->vals + loff - net->layers[i - 1],
			net->vals + loff, net->layers[i], net->layers[i - 1]);
		add(net->biases + loff, net->vals + loff, net->layers[i]);
		map(sigm, net->vals + loff, net->layers[i]);
	}
	return net->vals + loff;
}

void
train(net_t *const restrict net, const double *const restrict in,
	const double *const restrict act)
{
	size_t loff = net->nbs - net->layers[net->nlayers - 1], woff = net->nws;
	double *tmp;
/* Send through feedforward */
	feedforward(net, in);
/* Last layer error */
	sub(act, net->vals + loff, net->b1, net->layers[net->nlayers - 1]);
	for (size_t i = net->nlayers - 1; i > 1; --i) {
		woff -= net->layers[i]*net->layers[i - 1];
	/* Scaled gradient */
		zip(net->rate, net->b1, net->vals + loff, net->layers[i]);
	/* Update Biases */
		add(net->vals + loff, net->biases + loff, net->layers[i]);
	/* Update weights */
		addDotT(net->vals + loff, net->vals + loff - net->layers[i - 1],
			net->weights + woff, net->layers[i],
			net->layers[i - 1]);
	/* Prepare next error */
		multcTV(1/net->rate, net->weights + woff, net->vals + loff,
			net->b2, net->layers[i], net->layers[i - 1]);
//		multTV(net->weights + woff, net->b1, net->b2,
//			net->layers[i], net->layers[i - 1]);
		tmp = net->b1;
		net->b1 = net->b2;
		net->b2 = tmp;
		loff -= net->layers[i - 1];
	}
	zip(net->rate, net->b1, net->vals + loff, net->layers[1]);
	add(net->vals + loff, net->biases + loff, net->layers[1]);
	addDotT(net->vals, in, net->weights, net->layers[1], net->layers[0]);
}
