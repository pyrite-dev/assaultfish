#version 110

varying vec4 vPos;
varying vec3 vNrm;
varying vec4 vShadowCoord;

uniform sampler2D depth_texture;
uniform sampler2D current_texture;
uniform float enable_lighting;

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
	vec2 d[4];
	float light = 1.0;
	float shadow_coef = 1.0;
	int i;

	d[0] = vec2(-0.94201624, -0.39906216);
	d[1] = vec2(0.94558609, -0.76890725);
	d[2] = vec2(-0.094184101, -0.92938870);
	d[3] = vec2(0.34495938, 0.29387760);

	light = texture2D(depth_texture, shadow_coord.xy).z;

	for(i = 0; i < 4; i++){
		vec2 p = shadow_coord.xy + d[i] / 700.0;

		if(p.x < 0.0) continue;
		if(p.y < 0.0) continue;
		if(p.x > 1.0) continue;
		if(p.y > 1.0) continue;

		if(texture2D(depth_texture, p).z < view){
			shadow_coef -= 0.2;
		}
	}

	return shadow_coef;
}

void main(void){
	vec4 c = enable_lighting > 0.5 ? PhongShading() : gl_Color;
	vec4 ambient = gl_LightSource[0].ambient;
	float shadow_coef = ShadowCoef();

	//c = c * texture2D(current_texture, gl_TexCoord[0].st);

	gl_FragColor = ambient * shadow_coef * c + (1.0 - ambient) * c;
	gl_FragColor.a = 1.0;
}
