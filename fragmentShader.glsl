#version 330 core	     // Minimal GL version support expected from the GPU

struct Material {
	sampler2D albedoTex;
};

uniform Material material;
uniform int texture;
uniform vec3 camPos;
uniform vec3 ambient;
uniform vec3 lightning;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;
out vec4 color;	  // Shader output: the color response attached to this fragment

void main() {
	vec3 usedColor;
	if (texture == 1) { // If the buffer send 1, then the shader applies a texture. If not, it uses a basic color.
		usedColor = texture(material.albedoTex, fTexCoord).rgb;
	} else {
		usedColor = vec3(ambient);
	}
	
	
	vec3 n = normalize(fNormal);
	vec3 l = normalize(lightning - fPosition);

	vec3 v = normalize(camPos - fPosition);
	vec3 r = 2 * dot(l, n) * n - l;
	vec3 diffuse = 0.8 * max(0, dot(l, n)) * usedColor;
	vec3 specular = 0.5 * pow(max(0, dot(v, r)), 0.8) * usedColor;

	color = vec4(usedColor + diffuse + specular, 1.0); // build an RGBA from an RGB
}
