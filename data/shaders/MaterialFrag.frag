#version 330
layout(std140) uniform;

in vec3 vertexNormal;
in vec3 cameraSpacePosition;
out vec4 outputColor;

uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularShininess;
uniform float alpha;
uniform float reflectivity;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

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

vec4 ComputeReflection()
{
	//Camera space reflection vector
	vec3 cameraSpaceViewDirection = normalize(cameraSpacePosition);
	vec3 cameraSpaceSurfaceNormal = normalize(vertexNormal);
	vec3 cameraSpaceReflectionVector = normalize(reflect(cameraSpaceViewDirection,cameraSpaceSurfaceNormal));

	vec3 cameraSpaceReflectionPosition = cameraSpacePosition + cameraSpaceReflectionVector*.1;
	vec4 clipSpaceReflectionPosition = ProjectionBlck.cameraToClipMatrix * vec4(cameraSpaceReflectionPosition, 1);
	vec3 NDCSpaceReflectionPosition = clipSpaceReflectionPosition.xyz / clipSpaceReflectionPosition.w;
	vec3 screenSpaceReflectionPosition = 0.5 * NDCSpaceReflectionPosition + 0.5;
	
	vec3 oldCameraSpaceReflectionPosition = cameraSpacePosition;
	vec3 oldScreenSpaceReflectionPosition = screenSpaceReflectionPosition;
	vec4 color = vec4(0,0,0,1);

	float oldDepth = oldScreenSpaceReflectionPosition.z;
	float currDepth = screenSpaceReflectionPosition.z;

	bool valid = true;
	int count = 0;
	while(count < 200 && valid)
	{
		cameraSpaceReflectionPosition = oldCameraSpaceReflectionPosition + cameraSpaceReflectionVector*.05;
		clipSpaceReflectionPosition = ProjectionBlck.cameraToClipMatrix * vec4(cameraSpaceReflectionPosition, 1);
		NDCSpaceReflectionPosition = clipSpaceReflectionPosition.xyz / clipSpaceReflectionPosition.w;
		screenSpaceReflectionPosition = 0.5 * NDCSpaceReflectionPosition + 0.5;
		oldDepth = oldScreenSpaceReflectionPosition.z;
		currDepth = screenSpaceReflectionPosition.z;
		float sampleDepth = texture(depthTexture, screenSpaceReflectionPosition.xy).x; 
		if((oldDepth <= sampleDepth && currDepth >= sampleDepth) || (oldDepth >= sampleDepth && currDepth <= sampleDepth))
		{
			color = texture(colorTexture, screenSpaceReflectionPosition.xy);
			break;
		}

		oldCameraSpaceReflectionPosition = cameraSpaceReflectionPosition;
		oldScreenSpaceReflectionPosition = screenSpaceReflectionPosition;
		count++;

		valid = screenSpaceReflectionPosition.x > 0 && screenSpaceReflectionPosition.x < 1 &&
			    screenSpaceReflectionPosition.y > 0 && screenSpaceReflectionPosition.y < 1 &&
				screenSpaceReflectionPosition.z > 0 && screenSpaceReflectionPosition.z < 1;
	}

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

	vec4 reflectionColor = vec4(0,0,0,0);
	if(ReflectionToggleBlck.reflectionToggle == 1 && reflectivity > .01)
	{
		reflectionColor = ComputeReflection();
		outputColor = reflectivity*reflectionColor + (1-reflectivity)*accumLighting;
	}
	else
	{
		outputColor = accumLighting;
	}
	outputColor.w = alpha;
}
