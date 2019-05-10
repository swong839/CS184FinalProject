#version 330 core
// These only work in vertex shader
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos.xyz, 1.0);
	TexCoord = aTexCoord;
}