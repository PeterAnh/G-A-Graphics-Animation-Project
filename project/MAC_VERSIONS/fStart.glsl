#define MAX_LIGHT_SOURCES 16
#define TRUE 1
#define FALSE 0

varying vec3 position;
varying vec3 normal;
varying vec2 texCoord;
varying float attenuation[MAX_LIGHT_SOURCES];

uniform vec4 LightPosition[MAX_LIGHT_SOURCES];
uniform vec3 AmbientProduct[MAX_LIGHT_SOURCES];
uniform vec3 DiffuseProduct[MAX_LIGHT_SOURCES];
uniform vec3 SpecularProduct[MAX_LIGHT_SOURCES];
uniform int  UseDirectional[MAX_LIGHT_SOURCES];
uniform mat4 ModelView;
uniform mat4 Projection;
uniform float Shininess;
uniform float texScale;
uniform int  LightCount;

uniform sampler2D texture;

const vec3 invert3 = vec3(1.0 / 3.0);

void main()
{
    gl_FragColor = vec4(0.0);

    for (int i = 0; i < LightCount; i++)
    {
        // The vector to the light from the vertex
        vec3 lightVec = LightPosition[i].xyz - position;

        if (UseDirectional[i] == TRUE)
            lightVec = LightPosition[i].xyz;

        // Unit direction vectors for Blinn-Phong shading calculation
        vec3 L = normalize( lightVec ); // Direction to the light source
        vec3 N = normalize( normal );   // Re-normalize after interpolation.

        // Compute terms in the illumination equation
        vec3 ambient = AmbientProduct[i];

        float Kd = max( dot(L, N), 0.0 );
        vec3  diffuse = Kd * attenuation[i] * DiffuseProduct[i];

        // globalAmbient is independent of distance from the light source
        vec3 globalAmbient = vec3(0.1, 0.1, 0.1);

        vec4 textureColor = texture2D(texture, texCoord * texScale);
        gl_FragColor += vec4(globalAmbient + ambient + diffuse, 1.0) * textureColor;

        // Compute specular light.
        if (Kd > 0.0)
        {
            vec3 E = normalize( -position );    // Direction to the eye/camera
            vec3 H = normalize( L + E );        // Halfway vector
            float Ks = pow( max(dot(N, H), 0.0), Shininess );
            float grey = dot (textureColor.rgb, invert3); // Tend specular to white
            gl_FragColor.rgb += Ks * attenuation[i] * grey * SpecularProduct[i];
        }
    }
}
