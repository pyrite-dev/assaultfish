#version 110

varying vec4 vPos;
varying vec3 vNrm;
varying vec4 vShadowCoord;
varying vec4 vColor;

void main(void){
	vPos = gl_ModelViewMatrix * gl_Vertex;
	vNrm = normalize(gl_NormalMatrix * gl_Normal);
	vShadowCoord = gl_TextureMatrix[7] * gl_ModelViewMatrix * gl_Vertex;

	vColor = gl_Color;

	gl_Position = gl_ProjectionMatrix * vPos;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
