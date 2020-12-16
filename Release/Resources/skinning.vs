in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices;
in lowp vec4 boneWeights;

smooth out vec3 outNormal;

uniform SceneMatrices
{
	uniform mat4 projectionMatrix;
} sm;

uniform SkinningMatrices
{
	uniform mat4 palette[64];
} skin;

uniform mat4 modelViewMatrix;

void main(void)
{
    vec4 outPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < 4; i++)
    {
        outPos = outPos + boneWeights[i] * (vec4(inputPosition, 1.0f) * skin.palette[int(boneIndices[i])]);
    }

    gl_Position = sm.projectionMatrix * (modelViewMatrix * vec4(outPos.xyz, 1.0f));
    outNormal = normalize(mat3(modelViewMatrix) * normal);
}