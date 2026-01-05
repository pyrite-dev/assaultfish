#version 110

uniform sampler2DShadow texture;
varying vec4 shadow;
varying vec4 color;
uniform float solid_color;
uniform sampler2D texture_in;

void main (void){
	vec4 c;

	if(solid_color > 0.5){
		c = color;
	}else{
		c = texture2D(texture_in, gl_TexCoord[1].st);
	}

	gl_FragColor = (shadow + (gl_Color - shadow) * shadow2DProj(texture, gl_TexCoord[0])) * c;
}
