#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 worldPos;
layout(location = 3) in vec3 EyeDirection_cameraspace; 
layout(location = 4) in vec3 Normal_cameraspace; 

struct PointLight{
	vec3 position;
	float power;
	vec3 color;
	float aux1;
	vec3 LightPosition_cameraspace;
	float aux2;
};
layout(binding = 1) uniform Lights{
	PointLight[10] lights;
	int num_lights;
} lightsStruct;


layout(binding = 2) uniform sampler2D texSampler;

layout(binding = 3) uniform AmbientLight{
	vec3 color;
	float coef;
} ambientLight;

layout(location = 0) out vec4 outColor;

void main() {

    float kd = 0.4;
	float ks = 0.4;

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );

	vec3 ambient = ambientLight.color * ambientLight.coef;
	vec3 texColor = texture(texSampler, fragTexCoord).rgb;


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

		vec3 specular = pl.color * pl.power * pow(cosAlpha,10) / (distance * distance);


		lightSum = kd*diffuse + ks*specular;
	}

	outColor.xyz = texColor*(lightSum + ambient);
	outColor.w = 1;
}