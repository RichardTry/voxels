#version 110

varying vec4 color;

void main()
{
	gl_Position = gl_Vertex;
	color = gl_Color;
}
