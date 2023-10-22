#version $VERSION

precision mediump float;

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aColor;
layout (location = 2) in vec2  aTex;
layout (location = 3) in float aTexId;
layout (location = 4) in vec3  aNormal;

out vec3  crntPos;
out vec3  o_color;
out vec2  texCoord;
out float texId;
out vec3  Normal;

uniform mat4 worldToView;
uniform mat4 modelToWorld;

void main() {
  crntPos     = vec3(modelToWorld * vec4(aPos, 1.0f));
  gl_Position = worldToView       * vec4(crntPos, 1.0f);
  o_color = vec3(0.0f, 1.0f, 0.0f); //aColor;
  texCoord = aTex;
  texId = aTexId;
  Normal = aNormal;
}
