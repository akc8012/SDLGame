#version 330 core

in vec3 Normal;
in vec2 TexureCoord;
in vec3 FragPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

out vec4 FragColor;

vec3 lightPos = vec3(20, 20, 0);
vec3 lightColor = vec3(0.1f, 0.4f, 1);
vec3 viewPos = vec3(50, -230, 0);


vec3 CalcAmbient()
{
	return lightColor * material.ambient;
}

vec3 CalcDiffuse(vec3 normal, vec3 lightDir)
{
	float diff = max(dot(normal, lightDir), 0.0);
	return lightColor * (diff * material.diffuse);
}

vec3 CalcSpecular(vec3 normal, vec3 lightDir)
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	return lightColor * (spec * material.specular);  
}

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	FragColor = vec4(CalcAmbient() + CalcDiffuse(normal, lightDir) + CalcSpecular(normal, lightDir), 1.0);
}