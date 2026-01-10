#version 110

varying vec4 shadow;
varying vec4 color;

void main(void){
	vec3 position = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 light = normalize(gl_LightSource[0].position.xyz - position);
	float diffuse = dot(light, normal);

	color = gl_Color;
	
	gl_FrontColor = gl_LightSource[0].ambient;
	shadow = gl_FrontColor;
	if (diffuse > 0.0){
		vec3 view = normalize(position);
		vec3 halfway = normalize(light - view);
		float specular = pow(max(dot(normal, halfway), 0.0), gl_FrontMaterial.shininess);
		vec4 temp = gl_LightSource[0].diffuse * diffuse;
		shadow += temp * 0.2;
		gl_FrontColor += temp + gl_FrontLightProduct[0].specular * specular;
	}
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_TextureMatrix[1] * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = ftransform();
}
