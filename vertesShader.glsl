#version 400
in vec3 vp;
out vec4 vertexColor;
uniform mat4 mvp;
void main() {
	gl_Position = mvp * vec4(vp, 1.0);
	vertexColor = vec4(vp, 1.0);
}