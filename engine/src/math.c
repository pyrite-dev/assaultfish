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
	GBMathSubtract3(t0, v2, v0);

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
