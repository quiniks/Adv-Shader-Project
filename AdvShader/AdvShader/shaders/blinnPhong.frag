#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec3 Normal;   

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;
uniform Light light;  
uniform vec3 viewPos;

void main()
{    
	vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}