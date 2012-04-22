#version 330
layout(std140) uniform;

//Input
in vec3 cameraSpaceNormal;
in vec3 cameraSpacePosition;

//Output
layout(location = 0) out vec4 outputColor0;
layout(location = 1) out vec4 outputColor1;
layout(location = 2) out vec4 outputColor2;
layout(location = 3) out vec4 outputColor3; //includes specular shininess
layout(location = 4) out vec4 outputColor4; //roughness, reflectivity, refractivity, refractiveIndex

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
	outputColor0.xyz = cameraSpacePosition;
	outputColor0.w = 1.0;

	//Camera space normals
	outputColor1.xyz = cameraSpaceNormal;
	outputColor1.w = 1.0;

	//Diffuse color
	outputColor2.xyz = diffuseColor.xyz;
	outputColor2.w = 1.0;

	//Specular color
	outputColor3.xyz = specularColor.xyz;
	outputColor3.w = specularShininess;

	//Other
	outputColor4.x = roughness;
	outputColor4.y = reflectivity;
	outputColor4.z = refractivity;
	outputColor4.w = refractiveIndex;
}
