#version 330

in vec4 vCol;
in vec2 TexCoord;
//in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform sampler2D theNormalTexture;
uniform sampler2D theDispTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;


uniform float heightScale = 0.05;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
//    float height =  texture(theDispTexture, texCoords).r;     
//    return texCoords - viewDir.xy * (height * heightScale); 


    // number of depth layers
    const float minLayers = 16;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(theDispTexture, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(theDispTexture, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

	// get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(theDispTexture, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
    
    return currentTexCoords;       
}


void main()
{
	// obtain normal from normal map in range [0,1]

	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

	vec2 texCoords = TexCoord;
	texCoords = ParallaxMapping(TexCoord,  viewDir); 

	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 normal = texture(theNormalTexture, texCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float diffuseFactor = max(dot(normal, lightDir), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	vec4 specularColour = vec4(0, 0, 0, 0);

	
	
	if(diffuseFactor > 0.0f)
	{
		
//		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(-lightDir, normal));
		vec3 halfwayDir = normalize(lightDir + viewDir);
		
		float specularFactor = dot(normal, halfwayDir);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	colour = texture(theTexture, texCoords) * (ambientColour + diffuseColour + specularColour);
}