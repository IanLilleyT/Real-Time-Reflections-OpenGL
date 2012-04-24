#version 330
layout(std140) uniform;

//Output
out vec4 outputColor;

//Uniforms
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D otherTexture;
uniform sampler2D depthTexture;
uniform sampler2D colorBufferTexture;

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

vec4 ComputeReflection()
{
	return vec4(1,1,1,1);
}

//Main
void main()
{
	vec2 screenSpacePosition = getScreenSpacePosition();
	float depth = linearizeDepth(texture(depthTexture,screenSpacePosition).x);
	if(depth < .999) //Don't draw background color pixels
	{
		float reflectivity = texture(otherTexture, screenSpacePosition).y;
		vec4 diffuseColor = texture(colorBufferTexture, screenSpacePosition);
		float diffuse = 1.0 - reflectivity;
		outputColor = reflectivity*ComputeReflection() + diffuse*diffuseColor;
		outputColor.w = 1.0;
	}
}
