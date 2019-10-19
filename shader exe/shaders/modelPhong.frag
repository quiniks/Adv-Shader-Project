#version 330 core

out vec4 FragColor;

in VERT_OUT {
    vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} frag_in; 

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
#define NR_POINT_LIGHTS 1

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;

uniform Material material1;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = texture(material1.normal, frag_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	//normal = normalize(frag_in.TBN * normal); 

    vec3 viewDir = normalize(frag_in.TangentViewPos - frag_in.TangentFragPos);

	vec3 result = vec3(0.0f);
    //result = CalcDirLight(dirLight, normal, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, frag_in.FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material1.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material1.diffuse, frag_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material1.diffuse, frag_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material1.specular, frag_in.TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(frag_in.TangentLightPos - frag_in.TangentFragPos);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material1.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material1.diffuse, frag_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material1.diffuse, frag_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material1.specular, frag_in.TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 