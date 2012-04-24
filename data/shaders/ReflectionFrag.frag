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

//Random function used for jittering rays
float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

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

vec4 ComputeReflection()
{
	//Tweakable variables
	float initialStepAmount = .01;
	float stepRefinementAmount = .7;
	int maxRefinements = 3;
	int maxDepth = 1;
	
	//Values from textures
	vec2 screenSpacePosition2D = getScreenSpacePosition();
	vec3 cameraSpacePosition = texture(positionTexture,screenSpacePosition2D).xyz;
	vec3 cameraSpaceSurfaceNormal = texture(normalTexture,screenSpacePosition2D).xyz;
	float roughness = texture(otherTexture,screenSpacePosition2D).x;
	float reflectivity = texture(otherTexture,screenSpacePosition2D).y;

	//Screen space vector
	vec3 cameraSpaceViewDir = normalize(cameraSpacePosition);
	vec3 cameraSpaceVector = normalize(reflect(cameraSpaceViewDir,cameraSpaceSurfaceNormal));
	vec3 screenSpacePosition = convertCameraSpaceToScreenSpace(cameraSpacePosition);
	vec3 cameraSpaceVectorPosition = cameraSpacePosition + cameraSpaceVector;
	vec3 screenSpaceVectorPosition = convertCameraSpaceToScreenSpace(cameraSpaceVectorPosition);
	vec3 screenSpaceVector = initialStepAmount*normalize(screenSpaceVectorPosition - screenSpacePosition);
	
	//Jitter the initial ray
	float randomOffset = clamp(rand(gl_FragCoord.xy),0,1)/10000.0;
	vec3 oldPosition = screenSpacePosition;// + screenSpaceVector;
	oldPosition *= clamp((1-randomOffset*roughness),0,1);
	vec3 currentPosition = oldPosition + screenSpaceVector;
	
	//State
	vec4 color = vec4(0,0,0,1);
	int count = 0;
	int numRefinements = 0;
	int depth = 0;

	//Ray trace!
	while(depth < maxDepth)
	{
		while(count < 1000)
		{
			//Stop ray trace when it goes outside screen space
			if(currentPosition.x < 0 || currentPosition.x > 1 ||
			   currentPosition.y < 0 || currentPosition.y > 1 ||
			   currentPosition.z < 0 || currentPosition.z > 1)
				break;

			//intersections
			vec2 samplePos = currentPosition.xy;
			float currentDepth = linearizeDepth(currentPosition.z);
			float sampleDepth = linearizeDepth(texture(depthTexture, samplePos).x);
			float diff = currentDepth - sampleDepth;
			float error = length(screenSpaceVector);
			if(diff >= 0 && diff < error)
			{
				screenSpaceVector *= stepRefinementAmount;
				currentPosition = oldPosition;
				numRefinements++;
				if(numRefinements >= maxRefinements)
				{
					vec3 normalAtPos = texture(normalTexture, samplePos).xyz;
					float orientation = dot(cameraSpaceVector,normalAtPos);
					if(orientation < 0)
					{
						float cosAngIncidence = -dot(cameraSpaceViewDir,cameraSpaceSurfaceNormal);
						cosAngIncidence = clamp(1-cosAngIncidence,0.0,1.0);
						color = texture(colorBufferTexture, samplePos) * cosAngIncidence;
					}
					break;
				}
			}

			//Step ray
			oldPosition = currentPosition;
			currentPosition = oldPosition + screenSpaceVector;
			count++;
		}
		depth++;
	}
	
	//Fade the reflection at large distance
	float travelLength = clamp(2*distance(screenSpacePosition, currentPosition),0,1);
	color *= 1.0 - travelLength*roughness;
	return color;
}

//Main
void main()
{
	vec2 screenSpacePosition = getScreenSpacePosition();
	outputColor = texture(colorBufferTexture, screenSpacePosition);
	float reflectivity = texture(otherTexture, screenSpacePosition).y;
	if(reflectivity > .01) //Don't draw background or non reflective pixels
	{
		outputColor = reflectivity*ComputeReflection() + (1.0 - reflectivity)*outputColor;
	}
	outputColor.w = 1.0;
}
