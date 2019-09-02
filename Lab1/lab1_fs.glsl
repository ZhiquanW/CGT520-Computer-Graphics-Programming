#version 400
out vec4 fragcolor; //the output color for this fragment    
uniform float time_v;
void main(void)
{   

	//Exercise: try to change the color to red
	fragcolor = vec4(abs(cos(time_v)), 0.0, 0.0, 1.0);
	//fragcolor = vec4(0, 0.0, 0.0, 1.0);
}




















