#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct PointLight
{
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;



uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D shadow_map;

uniform vec3 viewPos;
uniform DirectionalLight dirLight;
#define N_POINT_LIGHTS 4
uniform PointLight pointLights[N_POINT_LIGHTS];

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow_map, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.005 * (1.0 - dot(normalize(Normal), normalize(dirLight.direction))), 0.0005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
    
    return shadow;
}  

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //ambient from directional light
    float dirAmbientStrength = 0.1;
    vec3 dirAmbient = dirAmbientStrength * dirLight.color * (texture(diffuse_map, TexCoord)).rgb;
    
    //diffuse from directional light
    float dirDiffuseStrength = 0.9;
    vec3 dirLightDir = normalize(-dirLight.direction);
    float dirDiff = max(dot(norm, dirLightDir), 0.0);
    vec3 dirDiffuse = dirDiffuseStrength * dirDiff * dirLight.color * (texture(diffuse_map, TexCoord)).rgb; 

    //specular from directional light
    float dirSpecularStrength = 0.8;
    vec3 dirReflectDir = reflect(-dirLightDir, norm);
    float dirSpec = pow(max(dot(viewDir, dirReflectDir), 0.0), 32);
    vec3 dirSpecular = dirSpecularStrength * dirSpec * dirLight.color * (texture(specular_map, TexCoord)).rgb;

    vec3 pointContrib;
    for(int i = 0; i < N_POINT_LIGHTS; i++)
    {
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);

        //diffuse from point light
        float pDiffuseStrength = 0.8;
        vec3 pLightDir = normalize(pointLights[i].position - FragPos);
        float pDiff = max(dot(norm, pLightDir), 0.0);
        vec3 pDiffuse = pDiff * pointLights[i].color * (texture(diffuse_map, TexCoord)).rgb; 

        // specular from point light
        float pSpecularStrength = 1.0;
        vec3 pReflectDir = reflect(-pLightDir, norm);
        float pSpec = pow(max(dot(viewDir, pReflectDir), 0.0), 32);
        vec3 pSpecular = pSpec * pointLights[i].color * (texture(specular_map, TexCoord)).rgb;

        pointContrib += attenuation * (pDiffuse + pSpecular);
    }

    float shadow = ShadowCalculation(FragPosLightSpace);  
    // vec3 result = pointContrib + dirAmbient + (1.0 - shadow)*(dirDiffuse + dirSpecular);
    vec3 result = dirAmbient + (1.0 - shadow)*(dirDiffuse + dirSpecular);
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(lightColor, 1.0);
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}