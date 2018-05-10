//varying vec4 color;
varying vec2 texCoord;  // The third coordinate is always 0.0 and is discarded

uniform sampler2D texture;
uniform float texScale; //Part B: texture scaling

//First light
varying vec3 fN, fE, fL;
varying float light_strength;
uniform vec3 AmbientProduct, DiffuseProduct, SpecularProduct; 

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
vec4 color;
uniform float Shininess;

//Second light
varying vec3 fL2, fE2;
uniform vec3 AmbientProduct2, DiffuseProduct2, SpecularProduct2;
uniform vec4 LightPosition2;



void main()
{
    //From lecture 17 (task H)
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    vec3 H = normalize(L+E); //half-way vector

    vec3 ambient = AmbientProduct;

    float Kd = max(dot(L,N), 0.0);
    vec3 diffuse = Kd * DiffuseProduct;

    float Ks = pow(max(dot(N,H),0.0), 64.0); //Change shininess to a high fixed value (Task H)
    vec3 specular = (Ks * SpecularProduct);
    //vec3 white = vec3(255.0,255.0,255.0);

    if(dot(L,N) < 0.0)
    {
        specular = vec3(0.0,0.0,0.0);
    }
       
    
    //Calculate second light ambient, diffuse and specular
    vec3 E2 = normalize(fE2);
    vec3 L2 = normalize(fL2);    
    
    vec3 H2 = normalize(L2+E2);
    
    vec3 ambient2 = AmbientProduct2;
    
    float Kd2 = max(dot(L2,N),0.0);
    vec3 diffuse2 = Kd2 * DiffuseProduct2;
    
    float Ks2 = pow(max(dot(N,H2),0.0), 64.0);
    vec3 specular2 = (Ks2 * SpecularProduct);
    
    if(dot(L2,N) < 0.0)
    {
        specular2 = vec3(0.0,0.0,0.0);
    }

    // globalAmbient is independent of distance from the light source
    vec3 globalAmbient = vec3(0.1, 0.1, 0.1);
    color.rgb = globalAmbient  + (ambient+ambient2) + (diffuse/light_strength + diffuse2) + (specular/light_strength + specular2);
    color.a = 1.0;

    gl_FragColor = color * texture2D( texture , texCoord * 2.0 * texScale);
}
