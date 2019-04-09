#version 150

float psin(float rate, float mult, float off, int ittr){
	float ret=0.;
	for(int x=0; x<ittr; ++x){
		ret=sin(rate*mult*float(ittr)+off+ret+cos(float(ittr)+off + float(x)))*.5+.5;
	}
	return ret;
}
/*
void kalido(float (*arr)[2], float div){
	float x=*arr[0];
	float y=*arr[1];
	x=abs(x-cW/2)/div;
	y=abs(y-cH/2)/div;
	*arr[0]=x;
	*arr[1]=y;
}*/

uniform float cW;
uniform float cH;
uniform float runner;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y


void main(){
	vec2 vUv=gl_FragCoord.xy / vec2(cW,cH) *.03;
	vUv=vec2(vUv.y,vUv.x);
	vec4 cd=vec4(1.0);
	float run=runner*.003;
	cd.r=(( psin(run*91.213+vUv.x/540.,4.,(vUv.x*631.-sin(run*30.+vUv.y)*5.),4) )*.5+.5);
	cd.g=(( psin(-run*51.213+vUv.y/140.,3.,(vUv.x*431.-sin(run*30.+vUv.y)*5.),5) )*.5+.5);
	cd.b=( sin(sin(vUv.x/122.)*.2+.5-run/113.)* sin(run/31.-vUv.y/22.*sin(vUv.x/3.+vUv.y/2.+run/33.)) );
	cd.r=min(1., max(.001, cd.r*(sin(run/213.+vUv.y/24.)*.7+.5) ));
	cd.g=min(1., max(.001, cd.g*(sin(run/332.+vUv.x/43.)*.7+.5)*(sin(run/260.)+.5) ));
	cd.b=((psin(-run/51.213+vUv.y/240.,3.,(vUv.x/231.-sin(run/30.+vUv.y)*5.),3))*.5+.5);

	outputColor=cd;


}

/*

					xy=kalido((fx,fy),6,0)
					fx=xy[0]
					fy=xy[1]
					rgb=rgbMode((0,fy,fr), 1)
					rgb=list(toHSV(rgb))
					rgb[0]=(rgb[0]+fr*3.0+fy*5.0)%360
					rgb=toRGB(rgb)
					modeVal[runMode]=rgb
					*/