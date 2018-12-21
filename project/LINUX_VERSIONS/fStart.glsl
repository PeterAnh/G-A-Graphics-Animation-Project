varying vec4 color;
varying vec2 texCoord;  // The third coordinate is always 0.0 and is discarded

//Task C: tex scale when move middle mouse
uniform float texScale;

uniform sampler2D texture;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL, fL2, fS;

uniform vec3 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec3 AmbientProduct2, DiffuseProduct2, SpecularProduct2;
uniform vec3 AmbientProduct3, DiffuseProduct3, SpecularProduct3;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;
uniform float cutOff;

void main()
{
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);

	float LightDistance = length(fL);
	float LightAtenuation = LightDistance * LightDistance;

	vec3 H = normalize(L+E);
	vec3 ambient = AmbientProduct;

	float Kd = max(dot(L,N),0.0);
	vec3 diffuse = (Kd*DiffuseProduct)/LightAtenuation;

	float Ks = pow(max(dot(N,H),0.0), Shininess);
	vec3 specular = (Ks*SpecularProduct)/LightAtenuation;

	if(dot(L,N) < 0.0)
	{
		specular = vec3(0.0,0.0,0.0);
	}

	vec3 L2 = normalize(fL2);

	vec3 H2 = normalize(L2+E);

	vec3 ambient2 = AmbientProduct2;

	float Kd2 = max(dot(L2,N),0.0);
	vec3 diffuse2 = Kd2*DiffuseProduct2;

	float Ks2 = pow(max(dot(N,H2),0.0), Shininess);
	vec3 specular2 = Ks2*SpecularProduct2;

	if(dot(L2,N) < 0.0)
	{
		specular2 = vec3(0.0,0.0,0.0);
	}


	vec3 L3 = normalize(fS);

	vec3 H3 = normalize(L3+E);

	vec3 ambient3 = AmbientProduct3;

	float Kd3 = max(dot(L3,N),0.0);
	vec3 diffuse3 = Kd3*DiffuseProduct3*cutOff;

	float Ks3 = pow(max(dot(N,H3),0.0), Shininess);
	vec3 specular3 = Ks3*SpecularProduct3*cutOff;

	vec3 globalAmbient = vec3(0.1, 0.1, 0.1);
	color.rgb = globalAmbient + ambient + diffuse + ambient2 + diffuse2 + ambient3 + diffuse3;
	color.a = 1.0;

    gl_FragColor = color * texture2D( texture, texCoord * texScale ) + vec4(specular + specular2 + specular3, 1.0);
}
