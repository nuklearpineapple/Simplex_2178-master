#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform int Complimentary = 0;

out vec4 Fragment;

void main()
{
	vec3 output = Color;
	if(SolidColor != -1.0)
		output = SolidColor;
	
	if(Complimentary == 1)
		output = vec3(1.0f, 1.0f, 1.0f) - output;
	
	Fragment = vec4(output,1);
		
	return;
}