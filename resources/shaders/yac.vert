#version 430 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex;

out vec2 texCoords;

uniform mat4 cam;
uniform mat4 staticCam;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 cameraPosition;

void main(){
	mat4 mvp = projection * cam * model;

	gl_Position =  vec4(vertex, -1.0 , 1.0);
	texCoords = tex;
}
