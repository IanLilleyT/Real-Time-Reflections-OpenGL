#version 330
layout(std140) uniform;

//Input
in vec3 cameraSpaceNormal;
in vec3 cameraSpacePosition;

//Output
layout(location = 0) out vec4 outputPosition;
layout(location = 1) out vec4 outputNormal;
layout(location = 2) out vec4 outputDiffuseColor;
layout(location = 3) out vec4 outputSpecularColor; //includes specular shininess
layout(location = 4) out vec4 outputOther; //roughness, reflectivity, refractivity, refractiveIndex

//Uniforms
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float specularShininess;
uniform float roughness;
uniform float reflectivity;
uniform float refractivity;
uniform float refractiveIndex;

//Main
void main()
{
	//Camera space positions
	outputPosition.xyz = cameraSpacePosition;
	outputPosition.w = 1.0;

	//Camera space normals
	outputNormal.xyz = cameraSpaceNormal;
	outputNormal.w = 1.0;

	//Diffuse color
	outputDiffuseColor.xyz = diffuseColor.xyz;
	outputDiffuseColor.w = 1.0;

	//Specular color
	outputSpecularColor.xyz = specularColor.xyz;
	outputSpecularColor.w = specularShininess;

	//Other
	outputOther.x = roughness;
	outputOther.y = reflectivity;
	outputOther.z = refractivity;
	outputOther.w = refractiveIndex;
}
