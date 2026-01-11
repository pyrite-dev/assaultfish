#include "colors.inc"
#include "shapes.inc"
#include "skies.inc"

object {
        O_Cloud1
}

sky_sphere {
        pigment {
                gradient y
                color_map {
                        [0.0 White*0.8]
                        [0.2 rgb<0.2 0.2 0.7>]
                }
        }
}

plane {
	y, 0
	pigment {
		checker White Red
	}
	finish {
		phong 1.0
		reflection 0.3
	}
}

camera {
	location <0.75 * 1.25, 0.25, -1.5 * 1.25>
	look_at <0, 0.5, 0>
	angle 35
	up <0, 1, 0>
	right image_width/image_height*x
}

light_source {
	<125, 250, -250>
	color White * 2
	parallel point_at <0, 0, 0>
}

#macro Gear(NumTeeth, Diameter, Thickness)
	#macro Tooth()
		prism {
			linear_sweep
			linear_spline
			-1,
			1,
			7,
			<-1, -1.5>,
			<-1, 0>,
			<-0.5, 1>,
			<0.5, 1>,
			<1, 0>,
			<1, -1.5>,
			<-1, -1.5>
			rotate -90 * x
			scale 0.5
		}
	#end
	union {
		union {
			#declare T=NumTeeth;
			#declare A=0;
			#while (A < 360)
				object {
					Tooth()
					translate <0, T / pi, 0>
					rotate z * A
				}
				#declare A = A + 360 / T;
			#end
			scale 1 / (T / pi)
		}
		cylinder {
			<0, 0, -(1 / (T / pi) / 2)>,
			<0, 0, (1 / (T / pi) / 2)>,
			1 - (1 / (T / pi) / 2)
		}
		scale <0.5 * Diameter, 0.5 * Diameter, (T / pi) * 2 * Thickness>
	}
#end

object {
	difference {
		Gear(16, 1, 0.1)
		cylinder {
			<0, 0, -1>
			<0, 0, 1>
			0.1
		}
	}
	translate <0, 0.55, 0>
	pigment {
		color rgb <1, 0, 0>
	}
}

object {
	text {
		ttf "../font/IBMPlexSans-Bold.ttf"
		"GearSrc"
		1, 0
	}
	pigment {
		color rgb <1, 1, 1>
	}
	translate <-2, 0, -2>
	scale <0.25, 0.25, 0.1>
}
