#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 tex;

out vec2 texCoords;
out vec3 rayDirection;
out vec3 eye;

uniform mat4 cam;
uniform mat4 staticCam;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 cameraPosition;

void main(){
	mat4 mvp = projection * cam * model;

	gl_Position = mvp * vec4(vertex.xyz , 1.0);

	eye = vec3(cam[3][0], cam[3][1], cam[3][2]);
	rayDirection = normalize(vertex - eye);

	texCoords = tex.xy;
}
