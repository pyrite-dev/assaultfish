#version 110

varying vec4 vPos;
varying vec3 vNrm;
varying vec4 vShadowCoord;

uniform sampler2D depth_texture;
uniform sampler2D current_texture;

vec4 PhongShading(void)
{
        vec3 N = normalize(vNrm);
        vec3 L = normalize(gl_LightSource[0].position.xyz-vPos.xyz);

        vec4 ambient = gl_FrontLightProduct[0].ambient;

        float dcoef = max(dot(L, N), 0.0);
        vec4 diffuse = gl_FrontLightProduct[0].diffuse * dcoef;

        vec4 specular = vec4(0.0);
        if(dcoef > 0.0){
                vec3 V = normalize(-vPos.xyz);

                vec3 R = reflect(-L, N);
                float specularLight = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);

                specular = gl_FrontLightProduct[0].specular * specularLight;
        }
        return ambient+diffuse+specular;
}

float ShadowCoef(void){
	vec4 shadow_coord = vShadowCoord / vShadowCoord.w;
	float view = shadow_coord.z;
	float light = texture2D(depth_texture, shadow_coord.xy).z;
	float shadow_coef = 1.0;

	if(vShadowCoord.w > 0.0){
		shadow_coef = light < view ? 0.0 : 1.0;
	}

	return shadow_coef;
}

void main(void){
	vec4 c = PhongShading();
	vec4 ambient = gl_LightSource[0].ambient;
	float shadow_coef = ShadowCoef();

	//c = c * texture2D(current_texture, gl_TexCoord[0].st);

	gl_FragColor = ambient * shadow_coef * c + (1.0 - ambient) * c;
	gl_FragColor.a = 1.0;
}
