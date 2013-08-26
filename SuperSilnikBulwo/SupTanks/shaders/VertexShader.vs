
varying vec3 vertexColor;
uniform float xPosition;
uniform float yPosition;

void main() {  
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vertexColor = gl_Vertex.xyz;
  
	vec4 vertexPosition = ftransform();
	vertexPosition.x += xPosition/512;
	vertexPosition.y -= yPosition/300;
	gl_Position = vertexPosition;
}
