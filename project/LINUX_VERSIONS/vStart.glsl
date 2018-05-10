attribute vec4 vPosition; 
attribute vec3 vNormal;
attribute vec2 vTexCoord;

varying vec2 texCoord;

varying vec3 fL,fE,fN;

//Second light;
varying vec3 fL2,fE2;

varying float light_strength;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition, LightPosition2;


void main()
{

    //vec4 vpos = vec4(vPosition, 1.0); --> vPosition is vec4 now;

    // Transform vertex position into eye coordinates
    vec3 pos = (ModelView * vPosition).xyz;


    // The vector to the light from the vertex    
    fL = LightPosition.xyz - pos;            // Direction to the light source
    fE = -pos;                               // Direction to the eye/camera
    fN = (ModelView*vec4(vNormal, 0.0)).xyz; // Transform vertex normal into eye coordinates
    
    //Second light
    vec3 origin = (ModelView * vec4(0.0,0.0,0.0,1.0)).xyz;
    fL2 = LightPosition2.xyz - origin;
    fE2 = -origin; 
    

    light_strength = 0.1*pow(length(LightPosition),2.0);

    gl_Position = Projection * ModelView * vPosition;
    texCoord = vTexCoord;
}
