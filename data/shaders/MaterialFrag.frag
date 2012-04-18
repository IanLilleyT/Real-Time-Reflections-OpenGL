#version 330
layout(std140) uniform;

in vec3 vertexNormal;
in vec3 cameraSpacePosition;
in vec3 lightSpacePosition;
out vec4 outputColor;

//Material properties
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularShininess;
uniform float transparency;
uniform float reflectivity;
uniform float reflectiveScatter;
uniform float refractivity;
uniform float refractiveIndex;

//FBO textures
uniform sampler2D colorTextureFront;
uniform sampler2D depthTextureFront;
uniform sampler2D colorTextureBack;
uniform sampler2D depthTextureBack;
uniform sampler2D depthTextureShadow;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	mat4 shadowLightWorldToCameraMatrix;
	float zNear;
	float zFar;
} ProjectionBlck;

//Lighting structs
uniform int numLights;
const int maxNumberOfLights = 8;
struct PerLight
{
	vec4 cameraSpaceLightPos;
	vec4 lightIntensity;
};
uniform LightsBlock
{
	vec4 ambientIntensity;
	float lightAttenuation;
	float maxIntensity;
	float gamma;
	PerLight lights[maxNumberOfLights];
} LightsBlck;

//Set different effects
uniform EffectTypeBlock
{
	int effectType;
} EffectTypeBlck;

//Random function used for jittering rays (kinda slow)
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

//Diffuse / specular lighting calculations
float CalcAttenuation(in vec3 cameraSpacePosition,in vec3 cameraSpaceLightPos,out vec3 lightDirection)
{
	vec3 lightDifference =  cameraSpaceLightPos - cameraSpacePosition;
	float lightDistanceSqr = dot(lightDifference, lightDifference);
	lightDirection = lightDifference * inversesqrt(lightDistanceSqr);
	
	return (1 / ( 1.0 + LightsBlck.lightAttenuation * lightDistanceSqr));
}
vec4 ComputeLighting(in PerLight lightData)
{
	vec3 lightDir;
	vec4 lightIntensity;
	if(lightData.cameraSpaceLightPos.w == 0.0)
	{
		lightDir = vec3(lightData.cameraSpaceLightPos);
		lightIntensity = lightData.lightIntensity;
	}
	else
	{
		float atten = CalcAttenuation(cameraSpacePosition,
			lightData.cameraSpaceLightPos.xyz, lightDir);
		lightIntensity = atten * lightData.lightIntensity;
	}
	
	vec3 surfaceNormal = normalize(vertexNormal);
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = cosAngIncidence < 0.0001 ? 0.0 : cosAngIncidence;
	
	vec3 viewDirection = normalize(-cameraSpacePosition);
	
	vec3 halfAngle = normalize(lightDir + viewDirection);
	float angleNormalHalf = acos(dot(halfAngle, surfaceNormal));
	float exponent = angleNormalHalf / specularShininess;
	exponent = -(exponent * exponent);
	float gaussianTerm = exp(exponent);
	gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;
	
	vec4 lighting = diffuseColor * lightIntensity * cosAngIncidence;
	lighting += specularColor * lightIntensity * gaussianTerm;
	return lighting;
}

//Convert something in camera space to screen space
vec3 convertCameraSpaceToScreenSpace(in vec3 cameraSpace)
{
	vec4 clipSpace = ProjectionBlck.cameraToClipMatrix * vec4(cameraSpace, 1);
	vec3 NDCSpace = clipSpace.xyz / clipSpace.w;
	vec3 screenSpace = 0.5 * NDCSpace + 0.5;
	return screenSpace;
}

//Effect types
const int DIFFUSE = 0;
const int REFLECTION = 1;
const int REFRACTION = 2;
const int SHADOW = 3;

//Front vs Back face
const int FRONT = 0;
const int BACK = 1;

//Calculates two positions in screen space and outputs the screen space ray
void calcScreenSpaceVector(in int effectType, out vec3 screenSpacePosition, out vec3 screenSpaceVector)
{
	//Camera space vector
	vec3 cameraSpaceViewDirection = normalize(cameraSpacePosition);
	vec3 cameraSpaceSurfaceNormal = normalize(vertexNormal);
	vec3 cameraSpaceVector;
	if(effectType == REFLECTION)
		cameraSpaceVector = reflect(cameraSpaceViewDirection,cameraSpaceSurfaceNormal);
	else if(effectType == REFRACTION)
		cameraSpaceVector = refract(cameraSpaceViewDirection,cameraSpaceSurfaceNormal,1.0/refractiveIndex);
	cameraSpaceVector = normalize(cameraSpaceVector);

	//Screen space vector
	screenSpacePosition = convertCameraSpaceToScreenSpace(cameraSpacePosition);
	vec3 cameraSpaceVectorPosition = cameraSpacePosition + cameraSpaceVector;
	vec3 screenSpaceVectorPosition = convertCameraSpaceToScreenSpace(cameraSpaceVectorPosition);
	screenSpaceVector = normalize(screenSpaceVectorPosition - screenSpacePosition);
}

//Z buffer is nonlinear by default, so we fix this here
float linearizeDepth(float depth)
{
	float n = ProjectionBlck.zNear;
	float f = ProjectionBlck.zFar;
	return (2.0 * n) / (f + n - depth * (f - n));
}

//Variables used across several methods
vec3 screenSpacePosition;
vec3 screenSpaceVector;
vec3 oldPosition;
vec3 currentPosition;
vec4 color = vec4(0,0,0,1);
int count = 0;
int numRefinements = 0;

//Tweakable variables
float initialStepAmount = .01;
float stepRefinementAmount = .7;
int maxRefinements = 3;

//Finds the intersection of the ray and the depth texture
//and performs the binary search operation
bool findIntersection(in int faceType, in int effectType)
{
	vec2 samplePos = currentPosition.xy;
	float currentDepth = linearizeDepth(currentPosition.z);
	float sampleDepth = faceType == FRONT ? 
		linearizeDepth(texture(depthTextureFront, samplePos).x) :
		linearizeDepth(texture(depthTextureBack, samplePos).x);
	float diff = currentDepth - sampleDepth;
	float error = effectType == REFLECTION ? length(screenSpaceVector) : 100;
	if(diff >= 0 && diff < error)
	{
		screenSpaceVector *= stepRefinementAmount;
		currentPosition = oldPosition;
		numRefinements ++;
		if(numRefinements >= maxRefinements)
		{
			color = faceType == FRONT ? 
				texture(colorTextureFront, samplePos) :
				texture(colorTextureBack, samplePos);
			return true;
		}
	}
	return false;
}
vec4 ComputeEffect(in int effectType)
{
	//Initial reflection positions
	calcScreenSpaceVector(effectType,screenSpacePosition,screenSpaceVector);

	//Jitter the initial ray
	float randomOffset = clamp(rand(gl_FragCoord.xy),0,1)/10000.0;
	screenSpaceVector *= initialStepAmount;
	oldPosition = screenSpacePosition;// + screenSpaceVector;
	oldPosition *= clamp((1-randomOffset*reflectiveScatter),0,1);
	currentPosition = oldPosition + screenSpaceVector;
	
	while(count < 1000)
	{
		//Stop ray trace when it goes outside screen space
		if(currentPosition.x < 0 || currentPosition.x > 1 ||
		   currentPosition.y < 0 || currentPosition.y > 1 ||
		   currentPosition.z < 0 || currentPosition.z > 1)
			break;

		//intersections
		if(findIntersection(FRONT,effectType))// ||
		   //findIntersection(BACK,effectType))
		   break;

		//Step ray
		oldPosition = currentPosition;
		currentPosition = oldPosition + screenSpaceVector;
		count++;
	}
	
	//Fade the reflection at large distance
	float travelLength = clamp(2*distance(screenSpacePosition, currentPosition),0,1);
	color *= 1.0 - travelLength*reflectiveScatter;
	return color;
}
float CalcShadowFactor() 
{
	vec3 screenSpaceLightPosition = convertCameraSpaceToScreenSpace(lightSpacePosition);
	float depth = texture(depthTextureShadow, screenSpaceLightPosition.xy).x;
	if (depth <= (screenSpaceLightPosition.z + 0.009))
		return 0.5;
	else
		return 1.0;
}  
void main()
{
	vec2 screenSpacePosition = convertCameraSpaceToScreenSpace(cameraSpacePosition).xy;
	if(EffectTypeBlck.effectType == DIFFUSE)
	{
		//Calculate diffuse/specular lighting
		if(refractivity > 0) discard;
		outputColor = diffuseColor * LightsBlck.ambientIntensity;
		for(int light = 0; light < numLights; light++)
			outputColor += ComputeLighting(LightsBlck.lights[light]);
		outputColor = outputColor / LightsBlck.maxIntensity;
		vec4 gamma = vec4(1.0 / LightsBlck.gamma);
		gamma.w = 1.0;
		outputColor = pow(outputColor, gamma);
	}
	else if(EffectTypeBlck.effectType == REFLECTION)
	{
		//Calculate reflection
		if(refractivity > 0) discard;
		vec4 reflectiveColor;
		reflectiveColor = ComputeEffect(REFLECTION);
		float reflectionAmount = refractivity == 1.0 ? 0 : reflectivity / (1.0 - refractivity);
		float diffuseAmount = 1.0 - reflectionAmount;
		vec4 diffuseColor = texture(colorTextureFront, screenSpacePosition);
		outputColor = reflectionAmount * reflectiveColor + diffuseAmount * diffuseColor;
	}
	else if(EffectTypeBlck.effectType == REFRACTION)
	{
		//Calculate refraction
		vec4 refractiveColor;
		refractiveColor = ComputeEffect(REFRACTION);
		float otherAmount = 1.0 - refractivity;
		vec4 otherColor = texture(colorTextureFront, screenSpacePosition);
		outputColor = refractivity * refractiveColor + otherAmount * otherColor;
	}
	else if(EffectTypeBlck.effectType == SHADOW)
	{
		vec4 otherColor = texture(colorTextureFront, screenSpacePosition);
		float shadowAmount = CalcShadowFactor();
		outputColor = vec4(vec3(shadowAmount),1.0);
		//outputColor = vec4(vec3(otherColor)*shadowAmount,1.0);
	}
}
