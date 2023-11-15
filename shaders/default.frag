#version $VERSION

precision mediump float;
out vec4 fragColor;

in vec3 crntPos;
in vec3 color;
in vec2 texCoord;
in float texId;

in vec3 Normal;

// Textures for different texture samplers
uniform sampler2D u_albedoTexture;
uniform sampler2D u_ambientOcclusionTexture;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform bool u_hasAlbedoTexture;

// for specular
uniform vec3 camPos;

void main() {

	// ambient lighting
	float ambient = 0.40f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.3f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 4.0);
	float specular = specAmount * specularLight;

	// outputs final color
    vec4 surface_color;
    if (u_hasAlbedoTexture) {
	    surface_color = texture(u_albedoTexture, texCoord);
    } else {
        surface_color = vec4(color, 1.0f);
    }

	fragColor = surface_color * vec4(vec3(lightColor * (diffuse + ambient + specular)), 1.0f);
}
