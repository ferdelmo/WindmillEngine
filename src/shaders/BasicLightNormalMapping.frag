#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 worldPos;
layout(location = 3) in vec3 EyeDirection_cameraspace; 
layout(location = 4) in vec3 normal_cameraspace;  
layout(location = 5) in vec3 tangent_cameraspace;
layout(location = 6) in vec3 bitangent_cameraspace;


struct PointLight{
	vec3 position;
	float power;
	vec3 color;
	float aux1;
	vec3 LightPosition_cameraspace;
	float aux2;
};

struct DirectionalLight {
	mat4 depthBiasMVP;
	vec3 direction;
	float power;
	vec3 color;
	float aux1;
};

layout(binding = 1) uniform Lights{
	PointLight[10] lights;
	DirectionalLight[10] directional;
	int num_lights;
	int num_directional;
} lightsStruct;


layout(binding = 2) uniform AmbientLight{
	vec3 color;
	float coef;
} ambientLight;


layout(binding = 3) uniform sampler2D texSampler;

layout(binding = 4) uniform sampler2D normalSampler;

layout(binding = 5) uniform PhongShading {
	vec3 difusseColor;
	float kd;
	vec3 specularColor;
	float ks;
	int alpha;
	//aliment
	vec3 aux;
} phong;

layout(binding = 6) uniform sampler2D shadowMaps[10];

layout(location = 0) out vec4 outColor;





float textureProj(sampler2D shadowMap, vec4 shadowCoord, vec2 off)
{
	float shadow = 1.0;
	if ( shadowCoord.z > -1.0 && shadowCoord.z < 1.0 ) 
	{
		float dist = texture( shadowMap, shadowCoord.st + off ).r;
		if ( shadowCoord.w > 0.0 && dist < shadowCoord.z ) 
		{
			shadow = 0;
		}
	}
	return shadow;
}

float filterPCF(sampler2D shadowMap, vec4 sc)
{
	ivec2 texDim = textureSize(shadowMap, 0);
	float scale = 1.5;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 1;
	
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += textureProj(shadowMap, sc, vec2(dx*x, dy*y));
			count++;
		}
	
	}
	return shadowFactor / count;
}




void main() {

    float kd = phong.kd;
	float ks = phong.ks;



    mat3 TBN = transpose(mat3(
        tangent_cameraspace,
        bitangent_cameraspace,
        normal_cameraspace
    ));

	vec3 normalTex = 2*texture(normalSampler,fragTexCoord).rgb - 1.0;

	// USE NORMAL MAPPING
	vec3 n = normalize(normalTex); 
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

	vec3 texColor = texture(texSampler, fragTexCoord).rgb;
	vec3 ambient = ambientLight.color * ambientLight.coef * texColor;


	vec3 EyeDirection_tangentspace = TBN * EyeDirection_cameraspace;

	vec3 lightSum = {0, 0, 0};

	// point lights
	for(int i = 0; i < lightsStruct.num_lights; i++) {
		PointLight pl = lightsStruct.lights[i];
		
		vec3 LightDirection_cameraspace = normalize(pl.LightPosition_cameraspace + EyeDirection_cameraspace);

		// Direction of the light (from the fragment to the light)
		vec3 l = TBN * LightDirection_cameraspace;

		float cosTheta = clamp(dot(n, l), 0, 1);

		float distance = max(distance(pl.position, worldPos), 1.0f);

		vec3 diffuse = pl.color * pl.power * cosTheta / (distance * distance);


		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_tangentspace);

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);

		float cosAlpha = clamp(dot(E, R), 0, 1);

		vec3 specular = pl.color * pl.power * pow(cosAlpha, phong.alpha) / (distance * distance);

		/*
			sum of diffuse and specular componenets
		*/
		lightSum += kd*diffuse*texColor*phong.difusseColor + ks*specular*phong.specularColor;
	}

	vec3 directionalSum = {0, 0, 0};
	// directional light
	for(int i = 0; i < lightsStruct.num_directional; i++) {
		DirectionalLight dl = lightsStruct.directional[i];
		
		vec3 LightDirection_cameraspace = normalize(dl.direction);

		vec4 ShadowCoord = dl.depthBiasMVP * vec4(worldPos, 1.0);


		float visibility = textureProj(shadowMaps[i], ShadowCoord/ShadowCoord.w, vec2(0,0));
		visibility = filterPCF(shadowMaps[i], ShadowCoord/ShadowCoord.w);

		// Direction of the light (from the fragment to the light)
		vec3 l = TBN * LightDirection_cameraspace;

		float cosTheta = clamp(dot(n, l), 0, 1);

		vec3 diffuse = dl.color * dl.power * cosTheta;


		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_tangentspace);

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);

		float cosAlpha = clamp(dot(E, R), 0, 1);

		vec3 specular = dl.color * dl.power * pow(cosAlpha, phong.alpha);

		/*
			sum of diffuse and specular componenets
		*/
		directionalSum += visibility*(kd*diffuse*texColor*phong.difusseColor + ks*specular*phong.specularColor);
	}

	outColor.xyz = lightSum + ambient + directionalSum;
	outColor.w = 1;
}