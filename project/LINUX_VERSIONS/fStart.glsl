//varying vec4 color;
varying vec2 texCoord;  // The third coordinate is always 0.0 and is discarded

uniform sampler2D texture;
uniform float texScale; //Part B: texture scaling

varying vec3 fN; 
varying vec3 fE; 
varying vec3 fL;
varying float distance;

uniform vec3 AmbientProduct, DiffuseProduct, SpecularProduct; 
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;



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

    float Ks = pow(max(dot(N,H),0.0), Shininess);
    vec3 specular = Ks * SpecularProduct;

    if(dot(L,N) < 0.0)
    {
        specular = vec3(0.0,0.0,0.0);
    }
    if(distance < 0.0)
    {
        specular = vec3(255.0,255.0,255.0);
    }
    // globalAmbient is independent of distance from the light source
    //vec3 globalAmbient = vec3(0.1, 0.1, 0.1);

    gl_FragColor = vec4(ambient + diffuse/distance + specular/distance, 1.0) * texture2D( texture , texCoord * 2.0 * texScale);
}
