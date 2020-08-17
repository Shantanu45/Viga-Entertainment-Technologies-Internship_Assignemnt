#version 330

layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 tex;
layout (location = 1) in vec3 norm;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

//out vec4 vCol;
out vec2 TexCoord;
//out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};	

uniform DirectionalLight directionalLight;
uniform vec3 eyePosition;

void main()
{

	//vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoord = tex;
	
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * norm);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	FragPos = (model * vec4(pos, 1.0)).xyz; 

	mat3 TBN = transpose(mat3(T, B, N)); 
	TangentLightPos = TBN * directionalLight.direction;
	TangentViewPos  = TBN * eyePosition;
	TangentFragPos  = TBN * FragPos;
	
	gl_Position = projection * view * model * vec4(pos, 1.0);
}