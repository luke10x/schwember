#version $VERSION

precision mediump float;
out vec4 fragColor;

in vec3 crntPos;
in vec3 color;
in vec2 texCord;
in float texId;
in vec3 Normal;

uniform sampler2D sampler;

// for specular
uniform vec3 camPos;

void main() {
	color = vec4(color, 1.0f);
	fragColor = color;
}
