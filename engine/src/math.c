#include <GearBox/Math.h>

void GBMathCross3(GBVector3 r, GBVector3 v0, GBVector3 v1) {
	r[0] = v0[1] * v1[2] - v0[2] * v1[1];
	r[1] = v0[2] * v1[0] - v0[0] * v1[2];
	r[2] = v0[0] * v1[1] - v0[1] * v1[0];
}

void GBMathNormalize3(GBVector3 vec) {
	double l = 0;
	int    i;

	for(i = 0; i < 3; i++) l += vec[i] * vec[i];

	l = sqrt(l);

	if(l > 0) {
		l = (double)1 / l;
	} else {
		l = 0;
	}

	vec[0] *= l;
	vec[1] *= l;
	vec[2] *= l;
}

void GBMathSubtract3(GBVector3 r, GBVector3 v0, GBVector3 v1) {
	int i;

	for(i = 0; i < 3; i++) r[i] = v0[i] - v1[i];
}

void GBMathAdd3(GBVector3 r, GBVector3 v0, GBVector3 v1) {
	int i;

	for(i = 0; i < 3; i++) r[i] = v0[i] + v1[i];
}

/*
 * v0-v2
 *  | /
 * v1/
 */
void GBMathNormal3x3(GBVector3 r, GBVector3 v0, GBVector3 v1, GBVector3 v2) {
	GBVector3 t0, t1;

	GBMathSubtract3(t0, v1, v0);
	GBMathSubtract3(t1, v2, v0);

	GBMathCross3(r, t0, t1);

	GBMathNormalize3(r);
}

/*
 * v0-v3
 *  | |
 * v1-v2
 */
void GBMathNormal3x4(GBVector3 r, GBVector3 v0, GBVector3 v1, GBVector3 v2, GBVector3 v3) {
	GBVector3 t0, t1;

	GBMathNormal3x3(t0, v0, v1, v3);
	GBMathNormal3x3(t1, v1, v2, v3);

	GBMathAdd3(r, t0, t1);

	GBMathNormalize3(r);
}

double GBMathCot(double x) {
	return (double)1 / tan(x);
}

void GBMathInvert4x4(GBMatrix4x4 out, GBMatrix4x4 in) {
	double inv[16], det;
	int    i;

	inv[0]	= in[5] * in[10] * in[15] - in[5] * in[11] * in[14] - in[9] * in[6] * in[15] + in[9] * in[7] * in[14] + in[13] * in[6] * in[11] - in[13] * in[7] * in[10];
	inv[4]	= -in[4] * in[10] * in[15] + in[4] * in[11] * in[14] + in[8] * in[6] * in[15] - in[8] * in[7] * in[14] - in[12] * in[6] * in[11] + in[12] * in[7] * in[10];
	inv[8]	= in[4] * in[9] * in[15] - in[4] * in[11] * in[13] - in[8] * in[5] * in[15] + in[8] * in[7] * in[13] + in[12] * in[5] * in[11] - in[12] * in[7] * in[9];
	inv[12] = -in[4] * in[9] * in[14] + in[4] * in[10] * in[13] + in[8] * in[5] * in[14] - in[8] * in[6] * in[13] - in[12] * in[5] * in[10] + in[12] * in[6] * in[9];
	inv[1]	= -in[1] * in[10] * in[15] + in[1] * in[11] * in[14] + in[9] * in[2] * in[15] - in[9] * in[3] * in[14] - in[13] * in[2] * in[11] + in[13] * in[3] * in[10];
	inv[5]	= in[0] * in[10] * in[15] - in[0] * in[11] * in[14] - in[8] * in[2] * in[15] + in[8] * in[3] * in[14] + in[12] * in[2] * in[11] - in[12] * in[3] * in[10];
	inv[9]	= -in[0] * in[9] * in[15] + in[0] * in[11] * in[13] + in[8] * in[1] * in[15] - in[8] * in[3] * in[13] - in[12] * in[1] * in[11] + in[12] * in[3] * in[9];
	inv[13] = in[0] * in[9] * in[14] - in[0] * in[10] * in[13] - in[8] * in[1] * in[14] + in[8] * in[2] * in[13] + in[12] * in[1] * in[10] - in[12] * in[2] * in[9];
	inv[2]	= in[1] * in[6] * in[15] - in[1] * in[7] * in[14] - in[5] * in[2] * in[15] + in[5] * in[3] * in[14] + in[13] * in[2] * in[7] - in[13] * in[3] * in[6];
	inv[6]	= -in[0] * in[6] * in[15] + in[0] * in[7] * in[14] + in[4] * in[2] * in[15] - in[4] * in[3] * in[14] - in[12] * in[2] * in[7] + in[12] * in[3] * in[6];
	inv[10] = in[0] * in[5] * in[15] - in[0] * in[7] * in[13] - in[4] * in[1] * in[15] + in[4] * in[3] * in[13] + in[12] * in[1] * in[7] - in[12] * in[3] * in[5];
	inv[14] = -in[0] * in[5] * in[14] + in[0] * in[6] * in[13] + in[4] * in[1] * in[14] - in[4] * in[2] * in[13] - in[12] * in[1] * in[6] + in[12] * in[2] * in[5];
	inv[3]	= -in[1] * in[6] * in[11] + in[1] * in[7] * in[10] + in[5] * in[2] * in[11] - in[5] * in[3] * in[10] - in[9] * in[2] * in[7] + in[9] * in[3] * in[6];
	inv[7]	= in[0] * in[6] * in[11] - in[0] * in[7] * in[10] - in[4] * in[2] * in[11] + in[4] * in[3] * in[10] + in[8] * in[2] * in[7] - in[8] * in[3] * in[6];
	inv[11] = -in[0] * in[5] * in[11] + in[0] * in[7] * in[9] + in[4] * in[1] * in[11] - in[4] * in[3] * in[9] - in[8] * in[1] * in[7] + in[8] * in[3] * in[5];
	inv[15] = in[0] * in[5] * in[10] - in[0] * in[6] * in[9] - in[4] * in[1] * in[10] + in[4] * in[2] * in[9] + in[8] * in[1] * in[6] - in[8] * in[2] * in[5];

	det = in[0] * inv[0] + in[1] * inv[4] + in[2] * inv[8] + in[3] * inv[12];
	if(det == 0)
		return;

	det = 1.0 / det;

	for(i = 0; i < 16; i++)
		out[i] = inv[i] * det;
}
