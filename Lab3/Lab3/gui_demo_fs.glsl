#version 400

uniform sampler2D diffuse_tex;
uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shiness;
uniform bool enable_texture;
uniform vec3 eye_pos;

out vec4 fragcolor;           
in vec2 tex_coord;
in vec3 nw;
in vec3 lw;
in vec3 pw;
in float dis;
void main(void)
{
	vec3 inKa = Ka;
	vec3 inKd = Kd;
   vec4 tex_color = texture(diffuse_tex, tex_coord);
   vec3 rw = normalize(reflect(-lw, nw));
   vec3 vw = normalize(eye_pos - vec3(pw));
   float attenuation = 1 / pow(dis, 2);
   vec3 ambient_col;
   vec3 diffuse_col;
   vec3 specular_col;
   if (enable_texture) {
	  inKa = vec3(tex_color);
	  inKd = vec3(tex_color);
   }
   ambient_col = inKa * ambient;
   diffuse_col = inKd * diffuse * max(0, dot(nw, lw));
   specular_col =  Ks * specular *pow(max(0, dot(rw, vw)), shiness);
   fragcolor = vec4(ambient_col + attenuation * (diffuse_col + specular_col),1.0);
}




















