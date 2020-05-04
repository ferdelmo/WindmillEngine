#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 EyeDirection_cameraspace; 
layout(location = 2) in vec3 Normal_cameraspace;  
layout(location = 3) in vec2 texCoord; 


struct PointLight{
	vec3 position;
	float power;
	vec3 color;
	float aux1;
	vec3 LightPosition_cameraspace;
	float aux2;
};

struct DirectionalLight {
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

layout(binding = 3) uniform PhongShading {
	vec3 difusseColor;
	float kd;
	vec3 specularColor;
	float ks;
	int alpha;
	//aliment
	vec3 aux;
} phong;

layout(binding = 4) uniform sampler2D shadowMap;

layout(location = 0) out vec4 outColor;

float LinearizeDepth(float depth)
{
  float n = 0.1; // camera z near
  float f = 256.0; // camera z far
  float z = depth;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main() {
	
	float depth = texture(shadowMap, texCoord).r;

	

	float kd = phong.kd;
	float ks = phong.ks;

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );

	vec3 finalColor = phong.difusseColor * ambientLight.color * ambientLight.coef;


	vec3 lightSum = {0, 0, 0};

	for(int i = 0; i < lightsStruct.num_lights; i++) {
		PointLight pl = lightsStruct.lights[i];
		
		vec3 LightDirection_cameraspace = pl.LightPosition_cameraspace + EyeDirection_cameraspace;
		// Direction of the light (from the fragment to the light)
		vec3 l = normalize( LightDirection_cameraspace );

		float cosTheta = clamp(dot(n, l), 0, 1);

		float distance = max(distance(pl.position, worldPos), 1.0f);

		vec3 diffuse = pl.color * pl.power * cosTheta / (distance * distance);


		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_cameraspace);

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);

		float cosAlpha = clamp(dot(E, R), 0, 1);

		vec3 specular = pl.color * pl.power * pow(cosAlpha,phong.alpha) / (distance * distance);


		lightSum = kd*diffuse*phong.difusseColor + ks*specular*phong.specularColor;
	}

	vec3 directionalSum = {0, 0, 0};
	// directional light
	for(int i = 0; i < lightsStruct.num_directional; i++) {
		DirectionalLight dl = lightsStruct.directional[i];
		
		vec3 LightDirection_cameraspace = normalize(dl.direction);

		// Direction of the light (from the fragment to the light)
		vec3 l = LightDirection_cameraspace;

		float cosTheta = clamp(dot(n, l), 0, 1);

		vec3 diffuse = dl.color * dl.power * cosTheta;


		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_cameraspace);

		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);

		float cosAlpha = clamp(dot(E, R), 0, 1);

		vec3 specular = dl.color * dl.power * pow(cosAlpha, phong.alpha);

		/*
			sum of diffuse and specular componenets
		*/
		directionalSum += kd*diffuse*phong.difusseColor + ks*specular*phong.specularColor;
	}

	outColor.xyz = lightSum + directionalSum + finalColor;
	outColor.w = 1;
}