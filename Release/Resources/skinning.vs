in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices;
in lowp vec4 boneWeights;


smooth out vec2 texCoord;
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
	for (int i = 0; i < 64; i++)
	vec4 localPos = skin.palette[boneIndices[0]] * vec4(inputPosition, 1.0f);
	vec4 outVertexPos = mvp * localPos;

	vec4 pos = vec4(inputPosition, 1.0f);

	gl_Position = sm.projectionMatrix * (modelViewMatrix * vec4(outVertexPos.xyz, 1.0f));
	outNormal = mat3(modelViewMatrix) * normal;

	vec3 localNormal = mat3(skin.palette[boneIndices[0]]) * normal;
	outNormal = mat3(mvp) * localNormal;
}

