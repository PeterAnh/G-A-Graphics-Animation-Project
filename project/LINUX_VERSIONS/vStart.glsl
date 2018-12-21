attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoord;

varying vec2 texCoord;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition, LightPosition2, SpotLightPosition;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL, fL2, fS;

attribute vec4 boneIDs;
attribute vec4 boneWeights;
uniform mat4 boneTransforms[64];


void main()
{
    ivec4 bone = ivec4(boneIDs); // convert boneIDs to ivec4
    mat4 boneTransform = boneWeights[0] * boneTransforms[bone[0]] +
                         boneWeights[1] * boneTransforms[bone[1]] +
                         boneWeights[2] * boneTransforms[bone[2]] +
                         boneWeights[3] * boneTransforms[bone[3]];

    vec4 vpos    = boneTransform * vec4(vPosition, 1.0);
    vec4 vnormal = boneTransform * vec4(vNormal, 0.0);

    // Transform vertex position into eye coordinates
    vec3 pos = (ModelView * vpos).xyz;

    vec3 origin = (ModelView * vec4(0.0,0.0,0.0,1.0)).xyz;

    fN = (ModelView * vnormal).xyz;
    fE = -pos;   
    fL = LightPosition.xyz - pos;
    fL2 = LightPosition2.xyz - pos;
    fS = SpotLightPosition.xyz - pos;

    
    gl_Position = Projection * ModelView * vpos;
    texCoord = vTexCoord;
}

