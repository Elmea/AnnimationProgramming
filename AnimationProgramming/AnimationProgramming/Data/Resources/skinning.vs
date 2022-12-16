
/////////////////////
// INPUT VARIABLES //
/////////////////////
in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices;
in lowp vec4 boneWeights;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
smooth out vec2 texCoord;
smooth out vec3 outNormal;

uniform SceneMatrices
{
	uniform mat4 projectionMatrix;
} sm;

uniform mat4 modelViewMatrix;

uniform SkinningMatrices
{
	uniform mat4 palette[64];
} skin;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
    vec4 localPos = vec4(0.f, 0.f, 0.f, 0.f);
    vec3 localNormal = normal;
    for (int i = 0; i < 4; i++)
    {
        localPos += skin.palette[int(boneIndices[i])] * vec4(inputPosition, 1.0f) * boneWeights[i];
        
        localNormal += mat3(skin.palette[int(boneIndices[i])]) * normal; 
    }
    
    outNormal = mat3(modelViewMatrix) * localNormal; 
	gl_Position = sm.projectionMatrix * (modelViewMatrix * vec4(localPos.xyz, 1.0f));
}
