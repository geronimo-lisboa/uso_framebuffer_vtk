#version 400
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 position;
in vec2 textureCoordinate;
in vec3 normal;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTextureCoordinate;

void main(){
	vTextureCoordinate = textureCoordinate;
	vPosition = position;
	vNormal = normal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 );
}