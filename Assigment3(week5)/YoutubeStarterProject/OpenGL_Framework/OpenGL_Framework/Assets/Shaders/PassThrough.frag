#version 420

uniform vec4 LightPosition;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;

uniform float LightSpecularExponent;
uniform float Attenuation_Constant;
uniform float Attenuation_Linear;
uniform float Atteunation_Quadratic;

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main()
{
	outColor.rgb = LightAmbient;

	//accoutn for rasterizer normalizing
	vec3 normal = normalize(norm);

	vec3 lightVec = LightPosition.xyz - pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	float NdotL = dot (normal, lightDir);

	if (NdotL >0.0)
	{
		//attenuation (fall off)
		float attenuation =1.0 / (Attenuation_Constant + (Attenuation_Linear * dist) + (Atteunation_Quadratic * dist * dist));
		
		//diffuse contribution
		outColor.rgb += LightDiffuse * NdotL * attenuation;

		//Blinn-Phong half vector
		float NdotHV = max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0);

		//calculate speculat contribution
		outColor.rgb += LightSpecular * pow (NdotHV, LightSpecularExponent) * attenuation;
	}

	vec4 textureColor = texture(uTex, texcoord);
	outColor.rgb *= textureColor.rgb;
	outColor.a = textureColor.a;
} 