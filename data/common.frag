#version 150

uniform float cW;
uniform float cH;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

void main(){
	vec4 cd=vec4(1.0);
	cd[0]=gl_FragCoord.x/cW;
	cd[1]=gl_FragCoord.y/cH;
	outputColor=cd;
}

/*

	if active == 1 :
		r=((psin(pos[2]/91.213+pos[0]/540,4,double(pos[0]/631-math.sin(pos[2]/30+pos[1])*5),4))*.5+.5)*255
		g=((psin(-pos[2]/51.213+pos[1]/140,3,double(pos[0]/431-math.sin(pos[2]/30+pos[1])*5),5))*.5+.5)*255
		b=(math.sin(math.sin(pos[0]/122)*.2+.5-pos[2]/113)*math.sin(pos[2]/31-pos[1]/22*math.sin(pos[0]/3+pos[1]/2+pos[2]/33)))*255
		r=min(255, max(1, r*((math.sin(pos[2]/213+pos[1]/24)*.7+.5) )))
		g=min(255, max(1, g*((math.sin(pos[2]/332+pos[0]/43)*.7+.5)*(math.sin(pos[2]/260)+.5) )))
		b=((psin(-pos[2]/51.213+pos[1]/240,3,double(pos[0]/231-math.sin(pos[2]/30+pos[1])*5),3))*.5+.5)*255
		return (r,g,b)
	}else{
		return (0,0,0)
*/