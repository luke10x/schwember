#version $VERSION

precision mediump float;

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aColor;
layout (location = 2) in vec2  aTex;
layout (location = 3) in float aTexId;
layout (location = 4) in vec3  aNormal;
layout (location = 5) in vec4  a_weights;
layout (location = 6) in uvec4 a_joints;

out vec3  crntPos;
out vec3  v_color;
out vec2  texCoord;
out float texId;
out vec3  Normal;

uniform mat4 worldToView;
uniform mat4 modelToWorld;
uniform uint u_selectedJointIndex;

void main() {
  // uint firstJoint = a_joints[1];
  uint firstJoint = uint(3);


  //int index = int(a_joints[i]);
  //float weight = a_weights[i];

  crntPos     = vec3(modelToWorld * vec4(aPos, 1.0f));
  gl_Position = worldToView       * vec4(crntPos, 1.0f);
  if (firstJoint == u_selectedJointIndex) {
    v_color = vec3(1.0f, 0.0f, 1.0f);
  } else {
    v_color = vec3(0.0f, 1.0f, 0.0f);
  }
  texCoord = aTex;
  texId = aTexId;
  Normal = aNormal;
}
