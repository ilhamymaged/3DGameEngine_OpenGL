#version 460 core
out vec4 FragColor;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;
in vec4 v_FragPosLightSpace;

layout (std140, binding = 1) uniform ShadowBuffer 
{
    mat4 u_LightSpaceMatrix;
    vec3 u_LightPos;
};

uniform vec3 u_Color;
uniform bool u_HasColor;

uniform sampler2D u_AlbedoTexture; // Slot 0
uniform bool u_HasAlbedoTexture;

uniform sampler2D u_NormalTexture; // Slot 1
uniform bool u_HasNormalTexture;

uniform sampler2D u_ShadowMap; // Slot 7

float CalculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    if (dot(normal, lightDir) <= 0.0)
    {
        return 0.0; 
    }

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(u_ShadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0f;
    
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

     vec2 offsets[4] = vec2[](
        vec2(-0.94201624, -0.39906216), vec2(0.94558609, -0.76890725),
        vec2(-0.094184101, -0.92938870), vec2(0.34495938, 0.29387760)
    );

    for (int i = 0; i < 4; i++)
    {
        float pcfDepth = texture(u_ShadowMap, projCoords.xy + offsets[i] * texelSize * 1.5).r; 
        shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
    }

    shadow /= 4.0;
    return shadow;
}

void main()
{           
    vec3 baseColor = u_HasColor ? u_Color : vec3(1.0);
    if (u_HasAlbedoTexture) baseColor *= texture(u_AlbedoTexture, v_TexCoords).rgb;
    
    vec3 norm;
    if (u_HasNormalTexture) 
    {
        vec3 normalMapValue = texture(u_NormalTexture, v_TexCoords).rgb;
        normalMapValue = normalize(normalMapValue * 2.0 - 1.0); // Map from [0,1] to [-1,1]
        norm = normalize(v_Normal + normalMapValue * 0.5); 
    } else 
    {
        norm = normalize(v_Normal);
    }

     if (!gl_FrontFacing)
    {
        norm = -norm;
    }
    
    vec3 ambient = 0.15 * baseColor;
    
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * baseColor;
    
    float shadow = CalculateShadow(v_FragPosLightSpace, norm, lightDir);       
    vec3 finalLighting = ambient + (1.0 - shadow) * diffuse;
    
    FragColor = vec4(finalLighting, 1.0);
}
