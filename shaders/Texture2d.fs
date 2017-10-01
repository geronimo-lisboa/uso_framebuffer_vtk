#version 400
uniform sampler2D texUnit;
out vec4 frag_colour;
in vec2 vertexTc;
void main() {
    vec4 texColor = texture(texUnit, vertexTc);
	frag_colour = texColor;
}