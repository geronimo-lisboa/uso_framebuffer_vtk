#version 400
uniform sampler2D texUnit;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPosition;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextureCoordinate;

out vec4 frag_colour;

void main() {
    //A posição do vertice responsável pelo fragmento é interpolada, e essa interpolação
    //está guardada em vPosition. vPosition é a posição do fragmento atual em Object
    //Coordinates. Logo, multiplicar pela ModelMatrix tornará ela World Coordinate.
    //O mesmo vale para a normal. Ela também é interpolada e tb tem que estar no sist
    //de coordenadas global
    
	//vec3 fragmentWorldPosition = (modelMatrix * vec4 (vPosition,1.0)).xyz;
    //vec3 normalWorldPosition = normalize( vec3( modelMatrix * vec4( vNormal, 0.0 ) ) );
	
	//Agora o vetor da origem da luz para o ponto do fragmento.
	
    //vec3 lightVector = normalize( lightPosition - fragmentWorldPosition );
	
    //O brilho da superfície será o produto escalar entre a normal e o vetor entre
    //o ponto da luz e o ponto do fragmento. O prod.escalar, para vetores normalizados,
    //e todos os vetores aqui estão normalizados, está entre [-1,1].
	
    //float brightness = dot( normalWorldPosition, lightVector );
	
    //Multiplica a cor (que poderia estar vindo de um sampler) pelo brilho para que
    //a cor varie de acordo com a posição do fragmento em relação à camera. A imagem 
    //da brightness está no intervalo [-1,1] e qqer coisa <= 0 zera a cor de saida.

	vec4 texColor = texture(texUnit, vTextureCoordinate);
	//texColor.x = texColor.x * brightness;
	//texColor.y = texColor.y * brightness;
	//texColor.z = texColor.z * brightness;
    frag_colour = vec4(vTextureCoordinate.x,vTextureCoordinate.y,vNormal.x,texColor.x);
}