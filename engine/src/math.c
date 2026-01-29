#include <GearSrc/Math.h>

void GSMathCross3(GSVector3 r, GSVector3 v0, GSVector3 v1) {
	r[0] = v0[1] * v1[2] - v0[2] * v1[1];
	r[1] = v0[2] * v1[0] - v0[0] * v1[2];
	r[2] = v0[0] * v1[1] - v0[1] * v1[0];
}

GSNumber GSMathInner3(GSVector3 v0, GSVector3 v1) {
	GSNumber r = 0;
	int	 i;

	for(i = 0; i < 3; i++) r += v0[i] * v1[i];

	return r;
}

void GSMathNormalize3(GSVector3 vec) {
	GSNumber l = 0;
	int	 i;

	for(i = 0; i < 3; i++) l += vec[i] * vec[i];

	l = sqrt(l);

	if(l > 0) {
		l = (GSNumber)1 / l;
	} else {
		l = 0;
	}

	vec[0] *= l;
	vec[1] *= l;
	vec[2] *= l;
}

void GSMathSubtract3(GSVector3 r, GSVector3 v0, GSVector3 v1) {
	int i;

	for(i = 0; i < 3; i++) r[i] = v0[i] - v1[i];
}

void GSMathAdd3(GSVector3 r, GSVector3 v0, GSVector3 v1) {
	int i;

	for(i = 0; i < 3; i++) r[i] = v0[i] + v1[i];
}

/*
 * v0-v2
 *  | /
 * v1/
 */
void GSMathNormalTriangle(GSVector3 r, GSVector3 v0, GSVector3 v1, GSVector3 v2) {
	GSVector3 t0, t1;

	/**
	 * this used to be:
	 * (v1 - v0) * (v2 - v0)
	 *
	 * why do i have to do this now? i know it is because of +z front
	 * but why?
	 */

	GSMathSubtract3(t0, v1, v2);
	GSMathSubtract3(t1, v0, v2);

	GSMathCross3(r, t0, t1);

	GSMathNormalize3(r);
}

/*
 * v0-v3
 *  | |
 * v1-v2
 */
void GSMathNormalQuad(GSVector3 r, GSVector3 v0, GSVector3 v1, GSVector3 v2, GSVector3 v3) {
	GSVector3 t0, t1;

	GSMathNormalTriangle(t0, v0, v1, v3);
	GSMathNormalTriangle(t1, v1, v2, v3);

	GSMathAdd3(r, t0, t1);

	GSMathNormalize3(r);
}

GSNumber GSMathCot(GSNumber x) {
	return (GSNumber)1 / tan(x);
}

void GSMathRowToColumn4x4(GSMatrix4x4 out, GSMatrix4x4 in) {
	int y, x;
	for(y = 0; y < 4; y++) {
		for(x = 0; x < 4; x++) {
			out[4 * x + y] = in[4 * y + x];
		}
	}
}

void GSMathInvert4x4(GSMatrix4x4 out, GSMatrix4x4 in) {
	GSNumber inv[16], det;
	int	 i;

	inv[0]	= in[5] * in[10] * in[15] - in[5] * in[14] * in[11] - in[6] * in[9] * in[15] + in[6] * in[13] * in[11] + in[7] * in[9] * in[14] - in[7] * in[13] * in[10];
	inv[1]	= -in[1] * in[10] * in[15] + in[1] * in[14] * in[11] + in[2] * in[9] * in[15] - in[2] * in[13] * in[11] - in[3] * in[9] * in[14] + in[3] * in[13] * in[10];
	inv[2]	= in[1] * in[6] * in[15] - in[1] * in[14] * in[7] - in[2] * in[5] * in[15] + in[2] * in[13] * in[7] + in[3] * in[5] * in[14] - in[3] * in[13] * in[6];
	inv[3]	= -in[1] * in[6] * in[11] + in[1] * in[10] * in[7] + in[2] * in[5] * in[11] - in[2] * in[9] * in[7] - in[3] * in[5] * in[10] + in[3] * in[9] * in[6];
	inv[4]	= -in[4] * in[10] * in[15] + in[4] * in[14] * in[11] + in[6] * in[8] * in[15] - in[6] * in[12] * in[11] - in[7] * in[8] * in[14] + in[7] * in[12] * in[10];
	inv[5]	= in[0] * in[10] * in[15] - in[0] * in[14] * in[11] - in[2] * in[8] * in[15] + in[2] * in[12] * in[11] + in[3] * in[8] * in[14] - in[3] * in[12] * in[10];
	inv[6]	= -in[0] * in[6] * in[15] + in[0] * in[14] * in[7] + in[2] * in[4] * in[15] - in[2] * in[12] * in[7] - in[3] * in[4] * in[14] + in[3] * in[12] * in[6];
	inv[7]	= in[0] * in[6] * in[11] - in[0] * in[10] * in[7] - in[2] * in[4] * in[11] + in[2] * in[8] * in[7] + in[3] * in[4] * in[10] - in[3] * in[8] * in[6];
	inv[8]	= in[4] * in[9] * in[15] - in[4] * in[13] * in[11] - in[5] * in[8] * in[15] + in[5] * in[12] * in[11] + in[7] * in[8] * in[13] - in[7] * in[12] * in[9];
	inv[9]	= -in[0] * in[9] * in[15] + in[0] * in[13] * in[11] + in[1] * in[8] * in[15] - in[1] * in[12] * in[11] - in[3] * in[8] * in[13] + in[3] * in[12] * in[9];
	inv[10] = in[0] * in[5] * in[15] - in[0] * in[13] * in[7] - in[1] * in[4] * in[15] + in[1] * in[12] * in[7] + in[3] * in[4] * in[13] - in[3] * in[12] * in[5];
	inv[11] = -in[0] * in[5] * in[11] + in[0] * in[9] * in[7] + in[1] * in[4] * in[11] - in[1] * in[8] * in[7] - in[3] * in[4] * in[9] + in[3] * in[8] * in[5];
	inv[12] = -in[4] * in[9] * in[14] + in[4] * in[13] * in[10] + in[5] * in[8] * in[14] - in[5] * in[12] * in[10] - in[6] * in[8] * in[13] + in[6] * in[12] * in[9];
	inv[13] = in[0] * in[9] * in[14] - in[0] * in[13] * in[10] - in[1] * in[8] * in[14] + in[1] * in[12] * in[10] + in[2] * in[8] * in[13] - in[2] * in[12] * in[9];
	inv[14] = -in[0] * in[5] * in[14] + in[0] * in[13] * in[6] + in[1] * in[4] * in[14] - in[1] * in[12] * in[6] - in[2] * in[4] * in[13] + in[2] * in[12] * in[5];
	inv[15] = in[0] * in[5] * in[10] - in[0] * in[9] * in[6] - in[1] * in[4] * in[10] + in[1] * in[8] * in[6] + in[2] * in[4] * in[9] - in[2] * in[8] * in[5];

	det = in[0] * inv[0] + in[4] * inv[1] + in[8] * inv[2] + in[12] * inv[3];
	if(det == 0)
		return;

	det = 1.0 / det;

	for(i = 0; i < 16; i++)
		out[i] = inv[i] * det;
}

GSNumber GSMathClosestPOT(GSNumber x) {
	GSNumber r = 2;

	while(r < x) r *= 2;

	return r;
}
