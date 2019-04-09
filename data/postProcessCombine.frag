#version 150

uniform float cW;
uniform float cH;
//uniform sampler2D tex0;
uniform sampler2DRect curPass;
uniform sampler2DRect nextPass;

in vec2 texcoord;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

void main(){
	vec2 vUv=gl_FragCoord.xy;
	vec4 cd=vec4(1.0);
	vec4 ct=texture2DRect(curPass, vUv);
	vec4 nt=texture2DRect(nextPass, vUv);
	float blend=vUv.x/cW;
	cd=ct*(1.-blend) + nt*blend;
	cd=vec4(cd.rgb, 1);
	cd=ct;
	outputColor=cd;
}