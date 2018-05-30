#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_ModelViewProj;

void main()
{
    v_TexCoord = texCoord;
    gl_Position = u_ModelViewProj * vec4(position, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, v_TexCoord);
}