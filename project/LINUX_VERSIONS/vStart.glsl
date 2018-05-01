attribute vec4 vPosition; //change vPosition type to vec4
attribute vec3 vNormal;
attribute vec2 vTexCoord;

varying vec2 texCoord;
//varying vec4 color;
//No need this line as the lighting will be calculated in fragment shader

//Add these following variables which will be passed down to fragment shader for per-frag lighting calculation
varying vec3 fN;
varying vec3 fL;
varying vec3 fE;

//uniform vec3 AmbientProduct, DiffuseProduct, SpecularProduct; <-- move to fragment shader
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
//uniform float Shininess; <-- move to fragment shader

void main()
{

    //vec4 vpos = vec4(vPosition, 1.0); --> vPosition is vec4 now;

    // Transform vertex position into eye coordinates
    vec3 pos = (ModelView * vPosition).xyz;


    // The vector to the light from the vertex    
    fL = LightPosition.xyz - pos;            // Direction to the light source
    fE = -pos;                               // Direction to the eye/camera
    fN = (ModelView*vec4(vNormal, 0.0)).xyz; // Transform vertex normal into eye coordinates
    /*
    // Unit direction vectors for Blinn-Phong shading calculation
    vec3 L = normalize( Lvec );   // Direction to the light source
    vec3 E = normalize( -pos );   // Direction to the eye/camera
    vec3 H = normalize( L + E );  // Halfway vector

    // Transform vertex normal into eye coordinates (assumes scaling
    // is uniform across dimensions)
    vec3 N = normalize( (ModelView*vec4(vNormal, 0.0)).xyz );

    // Compute terms in the illumination equation
    vec3 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 );
    vec3  diffuse = Kd*DiffuseProduct;

    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec3  specular = Ks * SpecularProduct;
    
    if (dot(L, N) < 0.0 ) {
	specular = vec3(0.0, 0.0, 0.0);
    } 

    // globalAmbient is independent of distance from the light source
    vec3 globalAmbient = vec3(0.1, 0.1, 0.1);
    color.rgb = globalAmbient  + ambient + diffuse + specular;
    color.a = 1.0;
    */
    //Everything above is moved to fragment shader (task G)

    gl_Position = Projection * ModelView * vPosition;
    texCoord = vTexCoord;
}
