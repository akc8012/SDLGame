#version 330 core

in vec3 Normal;
in vec2 TexureCoord;
in vec3 FragmentPosition;

uniform sampler2D diffuseTexture;
uniform vec3 flatColor;
uniform vec3 viewPosition;
uniform bool renderPerspective;

out vec4 FragmentColor;


vec3 CalcLighting();
vec3 CalcAmbient(vec3 lightColor);
vec3 CalcDiffuse(vec3 normal, vec3 lightDirection, vec3 lightColor);
vec3 CalcSpecular(vec3 normal, vec3 lightDirection, vec3 lightColor);

void main()
{
	vec4 fragmentColor = texture(diffuseTexture, TexureCoord);
	if (!renderPerspective)
	{
		FragmentColor = fragmentColor;
		return;
	}

	FragmentColor = vec4(CalcLighting(), 1) * fragmentColor;
}

vec3 CalcLighting()
{
	vec3 normal = normalize(Normal);
	vec3 lightColor = vec3(1, 1, 1);

	vec3 lightPosition = vec3(5, 5, 5);
	vec3 lightDirection = normalize(lightPosition - FragmentPosition);

	vec3 ambient = CalcAmbient(lightColor);
	vec3 diffuse = CalcDiffuse(normal, lightDirection, lightColor);
	vec3 specular = CalcSpecular(normal, lightDirection, lightColor);

	return ambient + diffuse + specular;
}

vec3 CalcAmbient(vec3 lightColor)
{
	float ambientStrength = 0.5;
	return lightColor * ambientStrength;
}

vec3 CalcDiffuse(vec3 normal, vec3 lightDirection, vec3 lightColor)
{
	float diffuse = max(dot(normal, lightDirection), 0);
	return diffuse * lightColor;
}

vec3 CalcSpecular(vec3 normal, vec3 lightDirection, vec3 lightColor)
{
	vec3 viewDirection = normalize(-viewPosition - FragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);

	float shininess = 32;
	float specular = pow(max(dot(viewDirection, reflectDirection), 0), shininess);

	return specular * lightColor;
}
