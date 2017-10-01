#version 400
in vec3 vp;
in vec2 tc;
out vec2 vertexTc;
uniform mat4 mvp;
void main() {
	gl_Position = mvp * vec4(vp, 1.0);
	vertexTc = tc;
}