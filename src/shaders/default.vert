#version $VERSION

precision mediump float;

layout (location = 0) in vec3  a_position;
layout (location = 1) in vec3  aColor;
layout (location = 2) in vec2  aTex;
layout (location = 3) in float aTexId;
layout (location = 4) in vec3  a_normal;
layout (location = 5) in vec4  a_joints;
layout (location = 6) in vec4  a_weights;

out vec3  crntPos;
out vec3  color;
out vec2  texCoord;
out float texId;
out vec3  Normal;

uniform mat4 worldToView;
uniform mat4 modelToWorld;
uniform mat4 u_boneToWorldTransforms[32];

void main() {


  vec3 world_position = vec3(0);
  vec3 world_normal   = vec3(0);
  vec3 world_tangent  = vec3(0);
/*  
  for (int i = 0; i < 4; i++)
  {
        int index = int(a_joints[i]);

        float weight = a_weights[i];
        mat4 bone_to_world = u_boneToWorldTransforms[index];

        world_position += (bone_to_world * vec4(a_position, 1)).xyz * weight;
        world_normal   += (bone_to_world * vec4(a_normal,   0)).xyz * weight;
  }
*/
 // TODO use world_position
 // For now at least in this shader just attribute position
 // with no bone transformations
  crntPos     = vec3(modelToWorld * vec4(a_position, 1.0f));
  gl_Position = worldToView       * vec4(crntPos, 1.0f);
  color = aColor;
  texCoord = aTex;
  texId = aTexId;
  Normal = a_normal;
}
