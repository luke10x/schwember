#version $VERSION

in vec4 aPos;

uniform mat4 worldToView;
uniform mat4 modelToWorld;

void main() {
  gl_Position = worldToView * modelToWorld * vec4(aPos.rgb, 1);
}
