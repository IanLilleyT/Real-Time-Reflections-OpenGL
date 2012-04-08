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
    return 0.0;//fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
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

/*
vec4 GetRayBounce(in vec4 cameraSpaceViewDirection, int vec4 cameraSpaceSurfaceNormal)
{
	if(ReflectionToggleBlck.reflectionToggle == 1 && reflectivity > 0)
	{
		return reflect(cameraSpaceViewDirection,cameraSpaceSurfaceNormal);
	}
	else if(ReflectionToggleBlock.refractionToggle == 1 && refractvivity != 1.0)
	{
		return refract(cameraSpaceViewDirection,cameraSpaceSurfaceNormal,1.0/refractivity);
	}
}*/

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

	//Fragment position in screen space
	screenSpacePosition = convertCameraSpaceToScreenSpace(cameraSpacePosition);

	//Point along camera space reflection vector in screen space
	vec3 cameraSpaceVectorPosition = cameraSpacePosition + cameraSpaceVector;
	vec3 screenSpaceVectorPosition = convertCameraSpaceToScreenSpace(cameraSpaceVectorPosition);

	//Scale reflection vector
	screenSpaceVector = normalize(screenSpaceVectorPosition - screenSpacePosition);
	float pixelSize = 1/100.0;
	float scaleAmount = pixelSize / length(screenSpaceVector.xy);
	screenSpaceVector *= scaleAmount;
}

vec4 ComputeReflection()
{
	//Initial reflection positions
	vec3 screenSpacePosition;
	vec3 screenSpaceReflectionVector;
	calcScreenSpaceVector(REFLECTION,screenSpacePosition,screenSpaceReflectionVector);
	vec3 oldPosition = screenSpacePosition + screenSpaceReflectionVector;
	vec3 currentPosition = oldPosition + screenSpaceReflectionVector;
	
	float colorStrength = 1.0;
	vec4 color = vec4(0,0,0,1);
	int count = 0;
	while(count < 200) //Hard limit on number of ray marches
	{
		//Stop ray trace when it goes outside screen space
		if(currentPosition.x <= 0 || currentPosition.x >= 1 ||
		   currentPosition.y <= 0 || currentPosition.y >= 1 ||
		   currentPosition.z <= 0 || currentPosition.z >= 1)
			break;

		float oldDepth = oldPosition.z;
		float currentDepth = currentPosition.z;

		//Sample front faces
		float sampleDepth = texture(depthTextureFront, (currentPosition.xy + oldPosition.xy)/2.0).x;
		float depthDifference = abs(oldDepth - currentDepth);
		float sampleDifference = abs(sampleDepth - min(oldDepth, currentDepth));
		if(sampleDifference <= depthDifference)
			{color = texture(colorTextureFront, (currentPosition.xy + oldPosition.xy)/2.0);break;}

		//Sample backfaces
		sampleDepth = texture(depthTextureBack, (currentPosition.xy + oldPosition.xy)/2.0).x;
		depthDifference = abs(oldDepth - currentDepth);
		sampleDifference = abs(sampleDepth - min(oldDepth, currentDepth));
		if(sampleDifference <= depthDifference)
			{color = texture(colorTextureBack, (currentPosition.xy + oldPosition.xy)/2.0);break;}

		//Blur and scatter effects
		colorStrength = clamp((1-count/50.0),0.0,1.0);
		float scatterAmount = clamp((count/20.0),0.0,1.0);
		vec3 scatter = vec3(rand(screenSpaceReflectionVector.xx),rand(screenSpaceReflectionVector.yy),0)*reflectiveScatter*scatterAmount;
		
		//Update vectors
		oldPosition = currentPosition;
		currentPosition = oldPosition + screenSpaceReflectionVector + scatter;
		count++;
	}
	color *= colorStrength;
	return color;
}

vec4 ComputeRefraction()
{
	vec3 screenSpacePosition;
	vec3 screenSpaceRefractionVector;
	calcScreenSpaceVector(REFRACTION,screenSpacePosition,screenSpaceRefractionVector);
	vec3 oldPosition = screenSpacePosition + screenSpaceRefractionVector;
	vec3 currentPosition = oldPosition + screenSpaceRefractionVector;
	return vec4(0,0,0,0);
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
			reflectiveColor = ComputeReflection();
		if(refractivity > 0)
			refractiveColor = ComputeRefraction();
		
		outputColor = reflectivity*reflectiveColor + refractivity*refractiveColor + (1-reflectivity-refractivity)*accumLighting;
	}
	else
	{
		outputColor = accumLighting;
	}
	outputColor.w = clamp(1-transparency,0,1);
}
