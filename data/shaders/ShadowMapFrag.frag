#version 330
layout(std140) uniform;

//Output
out vec4 outputColor;

//Uniforms
uniform sampler2D positionTexture;
uniform sampler2D shadowMapTexture;
uniform sampler2D colorBufferTexture;
uniform sampler2D depthTexture;
uniform mat4 cameraToWorldMatrix;
uniform mat4 lightWorldToCameraMatrix;

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

//Convert something in camera space to screen space
vec3 convertCameraSpaceToScreenSpace(in vec3 cameraSpace)
{
	vec4 clipSpace = cameraToClipMatrix * vec4(cameraSpace, 1);
	vec3 NDCSpace = clipSpace.xyz / clipSpace.w;
	vec3 screenSpace = 0.5 * NDCSpace + 0.5;
	return screenSpace;
}

float CalcShadowFactor() 
{
	vec3 cameraSpacePosition = texture(positionTexture,getScreenSpacePosition()).xyz;
	vec3 worldSpacePosition = vec3(cameraToWorldMatrix*vec4(cameraSpacePosition,1.0));
	vec3 lightSpacePosition = vec3(lightWorldToCameraMatrix*vec4(worldSpacePosition,1.0));
	vec3 screenSpaceLightPosition = convertCameraSpaceToScreenSpace(lightSpacePosition);
	float textureDepth = linearizeDepth(texture(shadowMapTexture, screenSpaceLightPosition.xy).x);
	float currDepth = linearizeDepth(screenSpaceLightPosition.z);
	return clamp(1-30*(currDepth-.01-textureDepth),.5,1);
}  

//Main
void main()
{
	vec2 screenSpacePosition = getScreenSpacePosition();
	float depth = texture(depthTexture, screenSpacePosition).x;
	if(depth < .999) //Don't draw background pixels
	{
		float shadowAmount = CalcShadowFactor();
		outputColor = shadowAmount * texture(colorBufferTexture, screenSpacePosition);
	}
}
