#version 330
layout(std140) uniform;

in vec3 vertexNormal;
in vec3 cameraSpacePosition;
out vec4 outputColor;

uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularShininess;
uniform float transparency;
uniform float reflectivity;
uniform float reflectiveScatter;
uniform float refractivity;
uniform float refractiveIndex;

uniform sampler2D colorTextureFront;
uniform sampler2D depthTextureFront;
uniform sampler2D colorTextureBack;
uniform sampler2D depthTextureBack;

uniform int numLights;

uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	float zNear;
	float zFar;
} ProjectionBlck;

struct PerLight
{
	vec4 cameraSpaceLightPos;
	vec4 lightIntensity;
};

const int maxNumberOfLights = 8;
uniform LightsBlock
{
	vec4 ambientIntensity;
	float lightAttenuation;
	float maxIntensity;
	float gamma;
	PerLight lights[maxNumberOfLights];
} LightsBlck;

uniform ReflectionToggleBlock
{
	int reflectionToggle;
} ReflectionToggleBlck;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float CalcAttenuation(in vec3 cameraSpacePosition,
	in vec3 cameraSpaceLightPos,
	out vec3 lightDirection)
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

const int REFLECTION = 0;
const int REFRACTION = 1;
void calcScreenSpaceVector(in int type, out vec3 screenSpacePosition, out vec3 screenSpaceVector)
{
	//Camera space vector
	vec3 cameraSpaceViewDirection = normalize(cameraSpacePosition);
	vec3 cameraSpaceSurfaceNormal = normalize(vertexNormal);
	vec3 cameraSpaceVector;
	if(type == REFLECTION)
		cameraSpaceVector = reflect(cameraSpaceViewDirection,cameraSpaceSurfaceNormal);
	else if(type == REFRACTION)
		cameraSpaceVector = refract(cameraSpaceViewDirection,cameraSpaceSurfaceNormal,1.0/refractiveIndex);
	cameraSpaceVector = normalize(cameraSpaceVector);

	//Screen space vector
	screenSpacePosition = convertCameraSpaceToScreenSpace(cameraSpacePosition);
	vec3 cameraSpaceVectorPosition = cameraSpacePosition + cameraSpaceVector;
	vec3 screenSpaceVectorPosition = convertCameraSpaceToScreenSpace(cameraSpaceVectorPosition);
	screenSpaceVector = normalize(screenSpaceVectorPosition - screenSpacePosition);
}
float linearizeDepth(float depth)
{
	float n = ProjectionBlck.zNear;
	float f = ProjectionBlck.zFar;
	return (2.0 * n) / (f + n - depth * (f - n));
}

const int FRONT = 0;
const int BACK = 1;

vec3 screenSpacePosition;
vec3 screenSpaceVector;
vec3 oldPosition;
vec3 currentPosition;
vec4 color = vec4(0,0,0,1);
int count = 0;
int numRefinements = 0;

float initialStepAmount = .01;
float stepRefinementAmount = .7;
int maxRefinements = 3;

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
vec4 ComputeEffect(in int type)
{
	//Initial reflection positions
	calcScreenSpaceVector(type,screenSpacePosition,screenSpaceVector);

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
		if(findIntersection(FRONT,type))// ||
		   //findIntersection(BACK,type))
		   break;

		//Update vectors
		oldPosition = currentPosition;
		currentPosition = oldPosition + screenSpaceVector;
		count++;
	}
	
	//color = vec4(rand(currentPosition.xx),rand(currentPosition.yy),rand(currentPosition.xy),0);
	float travelLength = clamp(2*distance(screenSpacePosition, currentPosition),0,1);
	color *= 1.0 - travelLength*reflectiveScatter;
	return color;
}

void main()
{
	//Calculate 
	vec4 accumLighting = diffuseColor * LightsBlck.ambientIntensity;
	for(int light = 0; light < numLights; light++)
		accumLighting += ComputeLighting(LightsBlck.lights[light]);
	accumLighting = accumLighting / LightsBlck.maxIntensity;
	vec4 gamma = vec4(1.0 / LightsBlck.gamma);
	gamma.w = 1.0;
	accumLighting = pow(accumLighting, gamma);

	if(ReflectionToggleBlck.reflectionToggle == 1)
	{
		vec4 reflectiveColor = vec4(0,0,0,0);
		vec4 refractiveColor = vec4(0,0,0,0);
		if(reflectivity > 0)
			reflectiveColor = ComputeEffect(REFLECTION);
		if(refractivity > 0)
			refractiveColor = ComputeEffect(REFRACTION);
		
		outputColor = reflectivity*reflectiveColor + refractivity*refractiveColor + (1-reflectivity-refractivity)*accumLighting;
	}
	else
	{
		outputColor = accumLighting;
	}
	outputColor.w = clamp(1-transparency,0,1);
}
