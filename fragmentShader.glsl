#version 400
in vec4 vertexColor;
out vec4 frag_colour;
void main() {
	frag_colour = vertexColor;
}