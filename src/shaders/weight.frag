#version $VERSION

precision mediump float;
out vec4 fragColor;

in vec3 crntPos;
in vec3 o_color;
in vec2 texCord;
in float texId;
in vec3 Normal;

uniform sampler2D sampler;

// for specular
uniform vec3 camPos;

void main() {
	fragColor = vec4(o_color, 1.0f);
}
