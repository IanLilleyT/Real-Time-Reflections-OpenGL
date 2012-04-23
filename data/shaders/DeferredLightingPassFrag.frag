#version 330
layout(std140) uniform;

//Output
out vec4 outputColor;

//Uniforms
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseColorTexture;
uniform sampler2D specularColorTexture;
uniform sampler2D otherTexture;
uniform sampler2D depthTexture;

//Single light (add support for more later)
uniform vec3 lightCameraSpacePosition;
uniform vec3 lightIntensity;

//Projection matrix
uniform ProjectionBlock
{
	mat4 cameraToClipMatrix;
	float zNear;
	float zFar;
	float screenWidth;
	float screenHeight;
};

//Light data
uniform LightBlock
{
	vec4 ambientIntensity;
	float lightAttenuation;
	float maxIntensity;
	float gamma;
};

//Z buffer is nonlinear by default, so we fix this here
float linearizeDepth(float depth)
{
	float n = zNear;
	float f = zFar;
	return (2.0 * n) / (f + n - depth * (f - n));
}

vec2 getScreenSpacePosition()
{
	return gl_FragCoord.xy/vec2(screenWidth,screenHeight);
}

vec3 ComputeLighting()
{
	vec2 screenSpacePosition = getScreenSpacePosition();
	vec3 cameraSpacePosition = texture(positionTexture,screenSpacePosition).xyz;
	vec3 cameraSpaceSurfaceNormal = texture(normalTexture,screenSpacePosition).xyz;
	vec3 diffuseColor = texture(diffuseColorTexture,screenSpacePosition).xyz;
	vec4 specularValues = texture(specularColorTexture,screenSpacePosition);
	vec3 specularColor = specularValues.xyz;
	float specularShininess = specularValues.w;

	vec3 lightDifference = lightCameraSpacePosition - cameraSpacePosition;
	float lightDistanceSqr = dot(lightDifference, lightDifference);
	vec3 lightDir = lightDifference * inversesqrt(lightDistanceSqr);
	float attenuation = (1.0 / ( 1.0 + lightAttenuation * lightDistanceSqr));
	vec3 newLightIntensity = attenuation * lightIntensity;
	
	float cosAngIncidence = dot(cameraSpaceSurfaceNormal, lightDir);
	cosAngIncidence = cosAngIncidence < 0.0001 ? 0.0 : cosAngIncidence;
	
	vec3 viewDirection = normalize(-cameraSpacePosition);
	vec3 halfAngle = normalize(lightDir + viewDirection);
	float angleNormalHalf = acos(dot(halfAngle, cameraSpaceSurfaceNormal));
	float exponent = angleNormalHalf / specularShininess;
	exponent = -(exponent * exponent);
	float gaussianTerm = exp(exponent);
	gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;
	
	vec3 lighting = diffuseColor * newLightIntensity * cosAngIncidence;
	//lighting += specularColor * newLightIntensity * gaussianTerm;
	return lighting;
}

//Main
void main()
{
	vec2 screenSpacePosition = getScreenSpacePosition();
	//outputColor = texture(normalTexture,screenSpacePosition);
	float depth = linearizeDepth(texture(depthTexture,screenSpacePosition).x);
	if(depth < .999)
	{
		outputColor = vec4(0,0,0,0);
		outputColor += vec4(ComputeLighting(),1.0);
		//outputColor = outputColor / maxIntensity;
		//vec4 gamma = vec4(vec3(1.0 / gamma),1.0);
		//outputColor = pow(outputColor, gamma);
		outputColor.w = 1.0;
		//outputColor = vec4(1,0,0,1);
	
	}
}
