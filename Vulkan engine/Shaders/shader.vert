#version 450
layout(location = 0) in vec3 inPosition; 
layout(location = 1 )in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout (binding = 0) uniform Matricies
{
	mat4 world;
	mat4 view;
	mat4 proj;
} ubo;

void main()
{

	vec4 pos  = vec4(inPosition,1.0);
	pos = ubo.world * pos;
	pos = ubo.view * pos;
	pos = ubo.proj * pos;
	gl_Position = pos; 
	fragColor = inColor;
}

