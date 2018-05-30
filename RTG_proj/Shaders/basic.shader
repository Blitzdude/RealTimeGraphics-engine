#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_ModelViewProj;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_ModelViewProj * vec4(position, 1.0);
    v_FragPos = vec3(u_Model * vec4(position, 1.0f));
    v_Normal = mat3(transpose(inverse(u_Model))) * normal;
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3 u_ViewPos; 
uniform vec4 u_Color; // objects color
uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    // diffuse lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * u_Color;

    vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
    color = result * texColor;
}
