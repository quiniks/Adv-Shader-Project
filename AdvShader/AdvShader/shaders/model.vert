#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent; 

out VERT_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vert_out; 

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	vert_out.TangentLightPos = TBN * lightPos;
    vert_out.TangentViewPos  = TBN * viewPos;
    vert_out.TangentFragPos  = TBN * vert_out.FragPos;

	vert_out.FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
	vert_out.TexCoords = aTexCoords;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}