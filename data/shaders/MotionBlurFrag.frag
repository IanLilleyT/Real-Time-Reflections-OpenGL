#version 330
layout(std140) uniform;

//Output
out vec4 outputColor;

//Uniforms
uniform sampler2D colorBufferTexture;
uniform sampler2D depthTexture;

uniform mat4 worldToClipMatrix;
uniform mat4 oldWorldToClipMatrix;
uniform mat4 clipToWorldMatrix;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	float zNear;
	float zFar;
	float screenWidth;
	float screenHeight;
};

//Z buffer is nonlinear by default, so we fix this here
float linearizeDepth(float depth)
{
	return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

vec2 getScreenSpacePosition()
{
	return gl_FragCoord.xy/vec2(screenWidth,screenHeight);
}

vec3 convertWorldSpaceToScreenSpace(in vec3 worldSpace, in mat4 worldToClipTransformation)
{
	vec4 clipSpace = worldToClipTransformation * vec4(worldSpace, 1);
	vec3 NDCSpace = clipSpace.xyz / clipSpace.w;
	vec3 screenSpace = 0.5 * NDCSpace + 0.5;
	return screenSpace;
}

//Convert something in screen space to world space
vec3 convertScreenSpaceToWorldSpace(in vec2 screenSpacePosition)
{
	float depth = texture(depthTexture, screenSpacePosition).x;
	vec4 clipSpacePosition = vec4(screenSpacePosition.x*2-1,screenSpacePosition.y*2-1, depth*2-1,1);
	vec4 transformed = clipToWorldMatrix*clipSpacePosition;
	vec4 worldPos = transformed/transformed.w;
	return vec3(worldPos);
}

//Main
void main()
{
	vec2 screenSpacePosition = getScreenSpacePosition();
	float depth = texture(depthTexture, screenSpacePosition).x;
	if(depth < .999) //Don't draw background pixels
	{
		vec3 worldPosition = convertScreenSpaceToWorldSpace(screenSpacePosition);
		vec2 oldScreenSpacePosition = convertWorldSpaceToScreenSpace(worldPosition,oldWorldToClipMatrix).xy;
		vec2 difference = (screenSpacePosition - oldScreenSpacePosition);

		int numSamples = 3;
		for(int i = 0; i <= numSamples; i++)
		{
			outputColor += texture(colorBufferTexture, screenSpacePosition);
			screenSpacePosition += difference;
		}
		outputColor = outputColor/ numSamples;
		//outputColor = vec4(clamp(difference,0,1),0,1);
	}
}
