#version 330
uniform vec3 viewPos;

in vec3 objectColor;
in vec3 normal;
in vec3 FragPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D texture;
uniform bool compute_lighting; //TODO: split up into multiple shaders

struct Material{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct PointLight{
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};
uniform Material material;

PointLight light;

void main()
{
   light.position = vec3(0,0,4);
   light.ambient = vec3(1,1,1);
   light.diffuse = vec3(1,1,1);
   light.specular = vec3(1,1,1);
   if(compute_lighting){
      vec3 color = objectColor+vec3(texture2D(texture,texCoord));


      vec3 ambient = material.ambient * light.ambient; //TODO: make light color/strength uniform

      vec3 norm = normalize(normal);
      vec3 lightDirection = normalize(light.position - FragPos);
      float diff = max(dot(norm,lightDirection),0.0);
      vec3 diffuse = light.diffuse * (diff * material.diffuse);

      vec3 viewDir = normalize(viewPos-FragPos);
      vec3 reflectDir = reflect(-lightDirection,norm);
      float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
      vec3 specular = light.specular*(spec*material.specular);

      vec3 result = ambient + diffuse + specular;

      outColor = vec4(result,1.0);
   }else{
      outColor = vec4(objectColor,1.0);
   }

}