#version 330 core
// These only work in vertex shader
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aColor;

out vec3 ourColor;

// out vec4 vertexColor; one way to pass information is by specifying ins and outs. names MUST match exactly

uniform float horizOffset;

void main()
{
	gl_Position = vec4(aPos.x + horizOffset, -aPos.y, aPos.z, 1.0);
	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
	ourColor = vec3(aPos.x + horizOffset, -aPos.y, aPos.z);
}