#include <GearBox/Math.h>

void GBMathCross(double* out, double v00, double v01, double v02, double v10, double v11, double v12) {
	out[0] = v01 * v12 - v02 * v11;
	out[1] = v02 * v10 - v00 * v12;
	out[2] = v00 * v11 - v01 * v10;
}

/*
 * v0-v2
 *  | /
 * v1/
 */
void GBMathNormal3(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22) {
	double v[3];
	double l = 0;
	int    i;

	GBMathCross(&v[0],			     /**/
		    v10 - v00, v11 - v01, v12 - v02, /**/
		    v20 - v00, v21 - v01, v22 - v02  /**/
	);

	l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	for(i = 0; i < 3; i++) {
		out[i] = v[i] / l;
	}
}

/*
 * v0-v3
 *  | |
 * v1-v2
 */
void GBMathNormal4(double* out, double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22, double v30, double v31, double v32) {
	double v0[3];
	double v1[3];
	int    i;
	double l = 0;

	GBMathNormal3(&v0[0], v00, v01, v02, v10, v11, v12, v30, v31, v32);
	GBMathNormal3(&v1[0], v10, v11, v12, v20, v21, v22, v30, v31, v32);

	for(i = 0; i < 3; i++) {
		out[i] = v0[i] + v1[i];
	}

	l = sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);

	for(i = 0; i < 3; i++) {
		out[i] = out[i] / l;
	}
}
