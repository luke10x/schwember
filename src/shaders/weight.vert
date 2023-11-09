#version $VERSION

precision mediump float;

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aColor;
layout (location = 2) in vec2  aTex;
layout (location = 3) in float aTexId;
layout (location = 4) in vec3  aNormal;
layout (location = 5) in vec4  a_weights;
layout (location = 6) in vec4  a_joints;

out vec3  crntPos;
out vec3  v_color;
out vec2  texCoord;
out float texId;
out vec3  Normal;

uniform mat4 worldToView;
uniform mat4 modelToWorld;
uniform uint u_selectedJointIndex;


// Higher weight redder it is, lower weight - bluer
vec3 calculateBoneHotnessColor(float weight) {
  return vec3(weight, 0.0, 1.0-weight);
}

void main() {
  uint joint_1   = uint(a_joints[0]);
  uint joint_2   = uint(a_joints[1]);
  uint joint_3   = uint(a_joints[2]);
  uint joint_4   = uint(a_joints[3]);
  float weight_1 = a_weights[0];
  float weight_2 = a_weights[1];
  float weight_3 = a_weights[2];
  float weight_4 = a_weights[3];

  // Default color gray
  v_color = vec3(0.0f, 1.0f, 0.0f);

  if (joint_1 == u_selectedJointIndex) {
    v_color = calculateBoneHotnessColor(weight_1);
  }
  if (joint_2 == u_selectedJointIndex) {
    v_color = calculateBoneHotnessColor(weight_2);
  }
  if (joint_3 == u_selectedJointIndex) {
    v_color = calculateBoneHotnessColor(weight_3);
  }
  if (joint_4 == u_selectedJointIndex) {
    v_color = calculateBoneHotnessColor(weight_4);
  }

  crntPos     = vec3(modelToWorld * vec4(aPos, 1.0f));
  gl_Position = worldToView       * vec4(crntPos, 1.0f);

  texCoord = aTex;
  texId = aTexId;
  Normal = aNormal;
}
