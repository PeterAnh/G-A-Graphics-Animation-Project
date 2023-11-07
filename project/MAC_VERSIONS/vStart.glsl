#define MAX_LIGHT_SOURCES 16
#define TRUE 1
#define FALSE 0

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoord;

varying vec3 position;
varying vec3 normal;
varying vec2 texCoord;
varying float attenuation[MAX_LIGHT_SOURCES];

uniform mat4 ModelView;
uniform mat4 Projection;
uniform int  LightCount;
uniform vec4 LightPosition[MAX_LIGHT_SOURCES];
uniform int  UseDirectional[MAX_LIGHT_SOURCES];

void main()
{
    vec4 vpos = ModelView * vec4(vPosition, 1.0);

    // Transform vertex position into eye coordinates
    position = vpos.xyz;

    // Calculate the drop-off of light intensity over distance.
    float A = 0.1;      // Light that's always present.
    float B = 0.09;     // Light intensity decreases over distance.
    float C = 0.032;    // Light intensity decreases over distance sqaured.

    int i = 0;
    for (int i = 0; i < LightCount; i++)
    {
        // The distance from light to vector to eye.
        vec3 lightVec = LightPosition[i].xyz - position;

        if (UseDirectional[i] == TRUE)
            lightVec = LightPosition[i].xyz;

        float d = length(lightVec) + length(position);
        attenuation[i] = 1.0 / (A + B * d + C * d*d);
    }

    // Transform vertex normal into eye coordinates (assumes scaling
    // is uniform across dimensions)
    normal = normalize( (ModelView * vec4(vNormal, 0.0)).xyz );

    gl_Position = Projection * vpos;
    texCoord = vTexCoord;
}
