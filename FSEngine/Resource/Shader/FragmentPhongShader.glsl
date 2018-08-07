#version 330 core

in vec3 Normal;
in vec2 TexureCoord;
in vec3 FragmentPosition;
in float RenderPerspective;

uniform sampler2D diffuseTexture;
uniform vec3 flatColor;
uniform vec3 viewPosition;

out vec4 FragmentColor;


vec3 CalcLighting();
vec3 CalcAmbient(vec3 lightColor);
vec3 CalcDiffuse(vec3 normal, vec3 lightDirection, vec3 lightColor);
vec3 CalcSpecular(vec3 normal, vec3 lightDirection, vec3 lightColor);

void main()
{
	if (RenderPerspective == 0)
	{
		FragmentColor = texture(diffuseTexture, TexureCoord);
		return;
	}

	vec3 color = vec3(texture2D(diffuseTexture, TexureCoord));
	FragmentColor = vec4(CalcLighting() * color, 1.0);
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
	float diffuse = max(dot(normal, lightDirection), 0.0);
	return diffuse * lightColor;
}

vec3 CalcSpecular(vec3 normal, vec3 lightDirection, vec3 lightColor)
{
	vec3 viewDirection = normalize(-viewPosition - FragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);

	float shininess = 32;
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);

	float specularStrength = 1;
	return specularStrength * specular * lightColor;
}