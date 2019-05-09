#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;
uniform float mixAmount;

void main()
{
	TexCoords = vertex.zw;
	
	vec4 white = vec4(1, 1, 1, 1);
	ParticleColor = mix(white, color, mixAmount * length(vertex.xy));

	gl_Position = projection * view * model * vec4(vec3(vertex.xy, 0.0) + offset, 1.0);
}