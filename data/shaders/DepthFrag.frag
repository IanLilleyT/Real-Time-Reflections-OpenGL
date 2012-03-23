#version 330

out vec4 gl_FragColor;

void main()
{
	float d = gl_FragCoord.z;
	//gl_FragDepth = .8;
    gl_FragColor = vec4(d,d,d,1);
}
