#version 330 core
out vec4 FragColor;


// in vec4 vertexColor; one way to pass information is by specifying ins and outs. names MUST match exactly
//uniform vec4 ourColor; is a global
in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0);
}