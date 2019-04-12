#include <chrono>
#include "ofApp.h"

//double PI = 3.14159265358979;
int active=1;
const int colorModes=7;

const int lineCountBase = 15000;
int lineCount = 7000;
int lineCounMax = 15000;
int lineCountPrev = 0;
double absRunner = 0;
double curTimer = 0;
int sW = 500;
int sH = 500;
double sWd = double(sW);
double sHd = double(sH);
double mouseX = 0;
double mouseY = 0;
double mouseXNorm = 0;
double mouseYNorm = 0;
int mouseDown=0;
int mouseDrag=0;
double mouseDragStart[2]={};
double caps = 0;
int curShaderMode=0; // Temp!!
int nextShaderMode=0;
int shiftDown=0;
int forceCook=0;

int transitionActive=0;
double transitionTimer=0.;
double transitionRate=.05;

//--------------------------------------------------------------
// -- Timers ---------------------------------------------------
long initTimer=0;
long timer=0;
double msDelta=0.;
int FPS=0;
double runner=0.;
long clockTick(){
	std::chrono::milliseconds curMS=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	return curMS.count();
}
void clockUpdate(){
    long curTime=clockTick();
	msDelta=double(curTime-timer);
	FPS=int( 1000./msDelta );
	runner=double(clockTick()-initTimer)/1000.;
	timer=curTime;
}

//--------------------------------------------------------------
//-- General Functions -----------------------------------------

// Don't mind the unused functions
// Just needed some basic math
// OpenFramework prob has these... too lazy

void delta(float *arr, float px1, float py1, float px2, float py2) {
	float distX = px2 - px1;
	float distY = py2 - py1;
	arr[0] = distX;
	arr[1] = distY;
}
void delta(double *arr, double px1, double py1, double px2, double py2) {
	double distX = px2 - px1;
	double distY = py2 - py1;
	arr[0] = distX;
	arr[1] = distY;
}

float distance(float p1[2], float p2[2]) {
	float distX = p2[0] - p1[0];
	float distY = p2[1] - p1[1];
	float ret = sqrt(distX*distX + distY * distY);
	return ret;
}
double distance(double p1[2], double p2[2]) {
	double distX = p2[0] - p1[0];
	double distY = p2[1] - p1[1];
	double ret = sqrt(distX*distX + distY * distY);
	return ret;
}
float distance(float px1, float py1, float px2, float py2) {
	float distX = px2 - px1;
	float distY = py2 - py1;
	float ret = sqrt(distX*distX + distY * distY);
	return ret;
}
double distance(double px1, double py1, double px2, double py2) {
	double distX = px2 - px1;
	double distY = py2 - py1;
	double ret = sqrt(distX*distX + distY * distY);
	return ret;
}
float distance(float px, float py) {
	float ret = sqrt(px*px + py * py);
	return ret;
}
double distance(double px, double py) {
	double ret = sqrt(px*px + py * py);
	return ret;
}
/*float distance(float pos[2]) {
	float ret = sqrt(pos[0]* pos[0] + pos[1] * pos[1]);
	return ret;
double distance(double pos[2]) {
	double ret = sqrt(pos[0]* pos[0] + pos[1] * pos[1]);
	return ret;
}*/
void normalize(float *arr) {
	float dist = distance(arr[0], arr[1]);
	float distX = arr[0] / dist;
	float distY = arr[1] / dist;
	arr[0] = distX;
	arr[1] = distY;
}
void normalize(double *arr) {
	double dist = distance(arr[0], arr[1]);
	double distX = arr[0] / dist;
	double distY = arr[1] / dist;
	arr[0] = distX;
	arr[1] = distY;
}
float sign(float val) {
	float ret = float((0 < val) - (val < 0));
	if (ret == 0) {
		ret = 1;
	}
	return ret;
}
double sign(double val) {
	double ret = double((0 < val) - (val < 0));
	if (ret == 0) {
		ret = 1;
	}
	return ret;
}

void print(short *val){
	std::cout<<val;
	std::cout<<"\n";
}
void print(int *val){
	std::cout<<val;
	std::cout<<"\n";
}
void print(long *val){
	std::cout<<val;
	std::cout<<"\n";
}
void print(float *val){
	std::cout<<val;
	std::cout<<"\n";
}
void print(double *val){
	std::cout<<val;
	std::cout<<"\n";
}
void print(char *val){
	std::cout<<val;
	std::cout<<"\n";
}

//--------------------------------------------------------------
//-- Math Functions --------------------------------------------


void toHSV(double (*RGB)[3]) {
	double H,S,V,r,g,b,d,minv,maxv;
	r = double(*RGB[0]);
	g = double(*RGB[1]);
	b = double(*RGB[2]);
	minv = min(r, min(g, b));
	maxv = max(r, max(g, b));
	V = maxv;
	d = max(1., maxv - minv);

	if(maxv != 0){
		S = d / maxv;
	}else { 
		S = 0;
		H = -1;
		*RGB[0]=H;
		*RGB[1]=S;
		*RGB[2]=V;
		return;
	}
	if(r == maxv){
		H=(g-b)/d;
	}else if( g == maxv){
		H=2+(b-r)/d;
	}else{
		H=4+(r-g)/d;
	}
	H *= 60;
	if(H < 0){
		H += 360;
	}
	*RGB[0]=H;
	*RGB[1]=S;
	*RGB[2]=V;
}

void toRGB(double (*HSV)[3]){
	double H,S,V, R,G,B, i,f,p,q,t;
	H=double(*HSV[0]);
	S=double(*HSV[1]);
	V=double(*HSV[2]);
	if(S == 0 ){
		*HSV[0]=V;
		*HSV[1]=V;
		*HSV[2]=V;
		return;
	}
	H/=60;
	i=floor(H);
	f=H-i;
	p=V*(1-S);
	q=V*(1-S*f);
	t=V*(1-S*(1-f));
	if(i == 0){
		R=V;
		G=t;
		B=p;
	}else if( i == 1){
		R=q;
		G=V;
		B=p;
	}else if( i == 2){
		R=p;
		G=V;
		B=t;
	}else if( i == 3){
		R=q;
		G=p;
		B=V;
	}else if( i == 4){
		R=t;
		G=p;
		B=V;
	}else{
		R=V;
		G=p;
		B=q;
	}
	*HSV[0]=R;
	*HSV[1]=G;
	*HSV[2]=B;
}

double toGrey(double (*RGB)[3]){ // Visually correct Greyscale
	return *RGB[0] * 0.2989 + *RGB[1] * 0.5870 + *RGB[2] * .1140;
}
double toGreyPerfect(double (*RGB)[3]){ //Mathematically correct greyscale -- I will probably never use this
	return (*RGB[0] + *RGB[1] + *RGB[2])/3;
}

double toGray(double (*RGB)[3]){
	return toGrey(RGB);
}
	
double todeg(double rad){
	return double(rad*(180/PI));
}

double torad(double deg){
	return double(deg*(PI/180));
}

// Basic Noise Function
// Stacked waves
double psin(double rate, double mult, double off, int ittr){
	double out=0;
	for(int x=0; x<ittr; ++x){
		out=sin(rate*mult*ittr+off+cos(ittr+off ))*.5+.5;
	}
	return out;
}
	
void kalido(double (*arr)[2], double div){
	double x=*arr[0];
	double y=*arr[1];
	x=abs(x-sW/2)/div;
	y=abs(y-sH/2)/div;
	*arr[0]=x;
	*arr[1]=y;
}
void kalido(double (*arr)[5]){
	double x, y, cx, cy, a, s, c, editx,edity;
	x=*arr[0];
	y=*arr[1];
	cx=*arr[2];
	cy=*arr[3];
	a=*arr[4];
	s=sin(a);
	c=cos(a);
	editx=x-cx;
	edity=y-cy;
	editx=editx*c+edity*s;
	edity=-editx*s+edity*c;
	x=editx+cx;
	y=edity+cy;

	*arr[0]=x;
	*arr[1]=y;
}

//--------------------------------------------------------------
//-- State Machine ---------------------------------------------
class PxlStateMachine{
	static PxlStateMachine self;
	public:
		int modeCount=2;
		int curMode=0;
		std::vector<double>valArr;

		int transState=0; // -1 Back;  0 Not Active;  1 Forward
		int transModes[2];
		double transPerc;
		double modeTransSpeed=.05;
		double modeTransModes[2]={};
		double transPercs[2]={};
		double transValues[2]={};
		int curCountEntry=0;
		
	float mode_redWavesX(double rate, double run){
			return (sin(rate+ run + cos(rate + .1) + mouseXNorm) * (mouseXNorm * 2 - 1) + (1. - mouseXNorm * .5));
	}
	float mode_blueWaveX(double rate, double run){
			return (sin(rate*.004+ run*.1*min(1., max(-1., (mouseXNorm - .5)*2.) ) ))*1;
	}

	void setActiveState(int func){
		if(func==2){
			transState=(active+1)%2;
		}else{
			transState=func;
		}
	}
	void pushState(int size){
		curCountEntry+=size;
	}

	void modeTransition(int func){
		int tmpCurMode=curMode;
		curMode=(curMode+1)%(int)modeCount;
		transState=1;
		if(curMode==0){
			modeTransModes[0]=curMode;
			modeTransModes[1]=tmpCurMode;
		}else{
			modeTransModes[0]=tmpCurMode;
			modeTransModes[1]=curMode;
		}
		transPercs[0]=1.-modeTransSpeed;
		transPercs[1]=modeTransSpeed;
	}
	void finish(){
		transState=0;
		transPerc=0;
		modeTransModes[0]=curMode;
		modeTransModes[1]=curMode;
		transPercs[0]=1;
		transPercs[1]=0;
	}
	void runStep(){
		if( transState==1 ){
			//if(func == 1){
				transPerc=transPerc+modeTransSpeed;
				if(transPerc>=1.){
					finish();
				}else{
					transPercs[0]=(1.-transPerc);
					transPercs[1]=(transPerc);
				}
			//}
		}
	}
	float readValue(int rVal){}
	/*
		if(rVal==0){
			if(modeTransActive == 1){
				freqOut=0;
				freqOne=0;
				freqTwo=0;
				transValues[0]=0;
				transValues[1]=0;
				for(int c=0; c<(int)modeCount; ++c){
					if(modeTransModes[c]==0){
						transValues[c]=mode_redWavesX( (rate*xMult*.5), runner * swMult) * transPercs[c];
					}else if(modeTransModes[c]==1){
						transValues[c]=mode_blueWaveX( xx, runner*swMult ) * transPercs[c];
					}
				}
				//freqOne=mode_redWavesX( (rate*xMult*.5), runner * swMult);
				//freqTwo=mode_blueWaveX( xx, runner*swMult );
				//freqTwo = sin(xx*.004 + runner * swMult*.1*min(1., max(-1., (mouseXNorm - .5)*2.))) * 1;
				// ---
				
				//freqOut=( freqOne*(1 - caps) +  freqTwo*caps );
				//freqOut=( transValues[0]* modeTransPercs[order] +  transValues[1]* modeTransPercs[(order+1)%2] );
				freqOut=( transValues[0]* transPercs[0] +  transValues[1]* transPercs[1] );
			}else{
				if(curMode==0){
					freqOut=mode_redWavesX( (rate*xMult*.5), runner * swMult);
				}else if(curMode==1){
					freqOut=mode_blueWaveX( xx, runner*swMult );
				}
			}
		}else if(rVal==1){
			if(modeTransActive == 1){
				freqOutY=0;
				freqOne=0;
				freqTwo=0;
				for(int c=0; c<(int)modeCount; ++c){
					if(modeTransModes[c]==0){
						//freqOne = (cos(rate*xMult + curTimer * swMult*.7 + (mouseYNorm - .5) * 3) + sin(rate*PI + PI) + sin(mouseXNorm*PI) + cos(mouseYNorm*PI));
						transValues[c] = (cos(rate*xMult + runner * swMult*.7 + (mouseYNorm - .5) * 3) + sin(rate*PI + PI) + sin(mouseXNorm*PI) + cos(mouseYNorm*PI))*sH*.1;

					}else if(modeTransModes[c]==1){
						transValues[c] = (sH*.5 * sin(rate + (xx + 15.15)*.001*(.1*(mouseYNorm) * 4 + .7))*.4) ;
					}
				}
				// ---
				
				freqOutY=( transValues[0]* transPercs[0] +  transValues[1]* transPercs[1] );
				//freqOutY=freqOne;
				//freqOutY=freqTwo;
				//freqOutY=( (cos(rate*xMult + curTimer * swMult*.7 + (mouseYNorm - .5) * 3) + sin(rate*PI + PI) + sin(mouseXNorm*PI) + cos(mouseYNorm*PI)) );
				//freqOutY=( sH*.5 * sin(rate + (xx + 15.15)*.001*(.1*(mouseYNorm) * 4 + .7))*.4 );

			}else{
				if(curMode==0){
					//freqOne = (cos(rate*xMult + curTimer * swMult*.7 + (mouseYNorm - .5) * 3) + sin(rate*PI + PI) + sin(mouseXNorm*PI) + cos(mouseYNorm*PI));
					freqOne = (cos(rate*xMult + runner * swMult*.7 + (mouseYNorm - .5) * 3) + sin(rate*PI + PI) + sin(mouseXNorm*PI) + cos(mouseYNorm*PI))*sH*.1;
					freqTwo=0;
				}else if(curMode==1){
					freqOne=0;
					freqTwo = (sH*.5 * sin(rate + (xx + 15.15)*.001*(.1*(mouseYNorm) * 4 + .7))*.4) ;
				}
				freqOutY=(freqOne + freqTwo);
			}
		}
	}
	*/
};
PxlStateMachine sMachine;
// sMachine.finish()
// sMachine.runStep()
// sMachine.modeTransition()
// sMachine.setActiveState()


//--------------------------------------------------------------
//-- pxlVisualizer Python Function------------------------------
/*
double scale = 15.;

double runner = 198.;
int maxMode = 6;
int argvMode = 0;
double mode[2] = { 0,0 };

int modeVal[7][3]={};
double minMax[2];
double blendMode=0;
int curMode=-1;
int prevMode=mode[1];
int modeChange=-1;
int setMode=-1;
double trans=0;


void pxProcess(surf, th){
	double fr=runner;
	
	run=0
	hit=0
	tot=sW*sH
	mode=list(mode)	
	
	if argvMode != None){
		mode=[argvMode,argvMode]
		blend=1
	
	mmCheck=[1,0]
	pp = 1
	r=g=b=0
	
	for x in range(0,len(pix),1){
		checkQuit()
		for y in range(len(pix[x])){
			if 1){
				fx=double(x)
				fy=double(y)
				blend=1-min(1, max(0, (math.sin(fr/15.23*math.sin(fx/20.0-fr/82+13.23)*5+123+min(1,max(0,math.sin(fx/8.0+fr/40.5+fy/43.0)*7+.5))+min(1,max(0,math.sin(-fy/10.0+fx/30.0+(fr)/90.5)*15+.5)))+math.sin(fr/60+244)*7+.5)*5+.5))
				mmCheck[0]=min(mmCheck[0], blend)
				mmCheck[1]=max(mmCheck[1], blend)


				rgb=[0,0,0]
				temprgb=[0,0,0]

				runMode=0
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){
					xy=kalido((fx,fy),6,0)
					fx=xy[0]
					fy=xy[1]
					rgb=rgbMode((0,fy,fr), 1)
					rgb=list(toHSV(rgb))
					rgb[0]=(rgb[0]+fr*3.0+fy*5.0)%360
					rgb=toRGB(rgb)
					modeVal[runMode]=rgb
					hit=1
				runMode=1
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){
					xy=kalido((double(x),double(y)), 4, 0)
					fx=xy[0]
					fy=xy[1]
					rgb=rgbNoiseWave(rgb,(fx,fy,fr), 1)
					modeVal[runMode]=rgb
					hit=1
				runMode=2
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){
					ang=torad(fr/4.24)
					fx=abs(double(x)-double(sW)/2)
					fy=abs(double(y)-double(sH)/2)
					try){
						ang*=(fx-(sW/2))/abs(fx-(sW/2))
					}catch(){
						pass;
					try){
						ang*=(fy-(sH/2))/abs(fy-(sH/2))
					except){
						pass;
					xy=kalido((fx,fy, (double(sW)), (double(sH)), ang+math.sin(fx/231.53+fy/142.3+fr/440.0)*2.3 ),2,1)
					
					scaler=35.0/double(scale)
					fx=double(xy[0])/scaler
					fy=double(xy[1])/scaler
					rgb=rgbKaleido((fx,fy,fr), 1)
					modeVal[runMode]=rgb
					hit=1
				runMode=3
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){
					ang=math.sin(fr/40+math.sin(double(x)/200)*PI-math.cos(double(y)/200)*PI)
					xy=kalido((double(x),double(y)+50.0, (double(sW)), (double(sH)), ang ),0,1)
					
					scaler=20.0/double(scale)
					fx=double(xy[0])/scaler
					fy=double(xy[1])/scaler
					gen=0
					if random.random()>.6 and (runner+run)%10==0){
						gen=1
					rgb=rgbSpray((fx,fy,fr),gen, 1)
					modeVal[runMode]=rgb
					hit=1
				runMode=4
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){
					
					scaler=40.0/double(scale)
					ang=math.sin(fr/130.0+math.sin(double(x)/450.0+fr/230)*PI-math.cos(double(y)/200.0)*PI)
					xy=kalido((double(x),double(y), (double(sW)), (double(sH)), ang ),0,1)
					fx=double(xy[0])/scaler
					fy=double(xy[1])/scaler
					
					gen=0
					if random.random()>.6 and (runner+run)%10==0){
						gen=1
					rgb=rgbRoll((fx,fy,fr),gen, 1)
					rgb=list(toHSV(rgb))
					rgb[0]=(rgb[0]+fr*5.0+fy*5.0+math.sin(fx/2.0+fr/3)*PI*2)%360
					rgb=toRGB(rgb)

					modeVal[runMode]=rgb
					hit=1
				
				runMode=5
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){

					scaler=20.0/double(scale)
					fx=fx/scaler
					fy=fy/scaler
					ang=torad(fr/20.24+math.cos(fx/7.4)*10.4)
					fx=abs(double(x)-double(sW)/2)
					fy=abs(double(y)-double(sH)/2)
					try{
						ang*=(fx-(sw/2))/abs(fx-(sw/2))*-1
					}catch(){
						pass;
					try{
						ang*=(fy-(sH/2))/abs(fy-(sH/2))*-1
					}catch(){
						pass;
					fx=fx/scaler
					fy=fy/scaler
					xy=kalido((fx,fy, (double(sw)), (double(sH)), ang/2),2,1)
					
					fx=double(xy[0])/scaler
					fy=double(xy[1])/scaler
					rgb=rgbKaleido((fx,fy,fr), 1)
					rgb=list(toHSV(rgb))
					rgb[0]=(rgb[0]-math.sin(fr/242.0+fr/5543.3)*58.0+math.cos(fy/224.0+fr/530.24)*58+fr/40.0)%360
					rgb=toRGB(rgb)
					modeVal[runMode]=rgb
					hit=1
				runMode=6
				if runMode in mode and ( (runMode == mode[0] and blend == 0) or (runMode == mode[1] and blend == 1) or (blend > 0 and blend < 1)){

					scaler=20.0/double(scale)
					fx=fx/scaler
					fy=fy/scaler
					ang=torad(fr/20.24+math.cos(fx/7.4)*10.4)
					fx=abs(double(x)-double(sw)/2)
					fy=abs(double(y)-double(sH)/2)
					try{
						mather=math.sin(fx/20.0+fr/5.3)/abs(math.sin(fx/20.0+fr/5.3))
					}catch(){
						mather=1
					fx=abs((fx+math.sin(fx/35.0+fr/15)*3.3+fr)/scaler*mather)
					fy=fy/scaler
					xy=kalido((fx,fy, (double(sw)), (double(sH)), ang/2),2,1)
					
					fx=double(xy[0])/scaler
					fy=double(xy[1])/scaler
					rgb=rgbKaleido((fx,fy,fr), 1)
					rgb=list(toHSV(rgb))
					rgb[0]=(rgb[0]+fx-math.sin(fr/142.0+fr/553.3)*58.0+math.cos(fy/124.0+fr/530.24)*58+fr/40.0)%360
					rgb=toRGB(rgb)
					modeVal[runMode]=rgb
					hit=1

				if hit == 0){
					if(x==0){
						rgb=rgbMode((fx,fy,fr), 1)
					}else{
						rgb=(pix[0][y][0],pix[0][y][1],pix[0][y][2])
					if blend==0){
						modeVal[mode[1]]=rgb
					if blend==1){
						modeVal[mode[0]]=rgb
				if blend > 0 and blend < 1){
					rgb=(modeVal[mode[0]][0]*(1-blend)+modeVal[mode[1]][0]*blend,modeVal[mode[0]][1]*(1-blend)+modeVal[mode[1]][1]*blend,modeVal[mode[0]][2]*(1-blend)+modeVal[mode[1]][2]*blend)
				
				r=int(clamp(rgb[0], 0, 255))
				g=int(clamp(rgb[1], 0, 255))
				b=int(clamp(rgb[2], 0, 255))
				pix[x][y][0]=r
				pix[x][y][1]=g
				pix[x][y][2]=b
			}else{
				pix[x][y][0]=r
				pix[x][y][1]=g
				pix[x][y][2]=b
			run+=1
	if ( (mmCheck[0] == 1 and mmCheck[1] == 1) or (mmCheck[0] == 0 and mmCheck[1] == 0) ){
		trans=0
		if setMode==1 ){
			if mmCheck[0] == 1){
				modeChange=0
			}else{
				modeChange=1
	}else{
		setMode=1
		trans=1
		modeChange=-1

	retQuit=0
	if setMode == 1 and modeChange != -1){
		setMode=0
		mode=list(mode)
		mode[modeChange]=(min(int(random.random()*(maxMode+1)),maxMode))
		if mode[modeChange] == mode[int(1-modeChange)]){
			while True){
				run+=1
				random.seed(run)
				val=min(int(random.random()*(maxMode+1)),maxMode)
				if val != mode[int(1-modeChange)]){
					mode[modeChange]=val
					break
				checkQuit()
		
	if verbose == 1){
		addVerbObj("mode", (str(mode[1])+" - "+str(mode[0])) )
		addVerbObj("trans", trans)
		addVerbObj("setMode", setMode)
		addVerbObj("modeChange", modeChange)

	return 1

*/


//--------------------------------------------------------------
//-- OpenFrameworks Start --------------------------------------
void ofApp::setup(){
	initTimer=clockTick();
	timer=initTimer;
	sW = ofGetWidth();
	sH = ofGetHeight();
    /*
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif
	*/
//shader.load("shader");

	// Update to this --
	// https://forum.openframeworks.cc/t/offbo-as-multiple-texture-container/22705
	//fbo.allocate(sW, sH, GL_RGB);
	//fbo.createAndAttachRenderbuffer(GL_RGB, 0);
	//fbo.createAndAttachRenderbuffer(GL_RGB, 1);
	
	fbo_curMode.allocate(sW, sH, GL_RGB);
	fbo_curMode.begin();
		ofClear(0,0,0);
	fbo_curMode.end();
	fbo_nextMode.allocate(sW, sH, GL_RGB);
	fbo_nextMode.begin();
		ofClear(0,0,0);
	fbo_nextMode.end();
	//////////////////

	ofs_common.load("common");
	ofs_rgbMode.load("00_rgbMode");
	ofs_rgbNoiseWave.load("01_rgbNoiseWave");
	ofs_rgbKaleido.load("02_rgbKaleido");
	ofs_rgbSpray.load("03_rgbSpray");
	ofs_rgbRoll.load("04_rgbRoll");

	ofs_postProcess.load("postProcessCombine");
}

//--------------------------------------------------------------
void ofApp::update(){
	if(active==1){
		clockUpdate();

		if(transitionActive==1){
			transitionTimer+=transitionRate;
			if(transitionTimer>=1.){
				transitionActive=0;
				transitionTimer=0.;
				curShaderMode=nextShaderMode+0;
			}
		}

	}
}
 

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0,0,0);
    ofSetColor(255);
    
	if(active==1 || forceCook==1){
		forceCook=0;
		fbo_curMode.begin();
			switch(curShaderMode){
				case 0:
					ofs_rgbMode.begin();
						ofs_rgbMode.setUniform1f("cW",(float)sW);
						ofs_rgbMode.setUniform1f("cH",(float)sH);
						ofs_rgbMode.setUniform1f("runner",(float)runner);
						ofs_rgbMode.setUniform1i("mode",curShaderMode);
						ofDrawRectangle(0,0,sW,sH);
					ofs_rgbMode.end();
					break;
				case 1:
					ofs_rgbNoiseWave.begin();
						ofs_rgbNoiseWave.setUniform1f("cW",(float)sW);
						ofs_rgbNoiseWave.setUniform1f("cH",(float)sH);
						ofs_rgbNoiseWave.setUniform1f("runner",(float)runner);
						ofs_rgbNoiseWave.setUniform1i("mode",curShaderMode);
						ofDrawRectangle(0,0,sW,sH);
					ofs_rgbNoiseWave.end();
					break;
				case 2:
				case 3:
					ofs_rgbRoll.begin();
						ofs_rgbRoll.setUniform1f("cW",(float)sW);
						ofs_rgbRoll.setUniform1f("cH",(float)sH);
						ofs_rgbRoll.setUniform1f("runner",(float)runner);
						ofs_rgbRoll.setUniform1i("mode",curShaderMode);
						ofDrawRectangle(0,0,sW,sH);
					ofs_rgbRoll.end();
					break;
				case 4:
				case 5:
				case 6:
					ofs_rgbKaleido.begin();
						ofs_rgbKaleido.setUniform1f("cW",(float)sW);
						ofs_rgbKaleido.setUniform1f("cH",(float)sH);
						ofs_rgbKaleido.setUniform1f("runner",(float)runner);
						ofs_rgbKaleido.setUniform1i("mode",curShaderMode);
						ofDrawRectangle(0,0,sW,sH);
					ofs_rgbKaleido.end();
					break;
				case 7:
					ofs_rgbSpray.begin();
						ofs_rgbSpray.setUniform1f("cW",(float)sW);
						ofs_rgbSpray.setUniform1f("cH",(float)sH);
						ofs_rgbSpray.setUniform1f("runner",(float)runner);
						ofs_rgbSpray.setUniform1i("mode",curShaderMode);
						ofDrawRectangle(0,0,sW,sH);
					ofs_rgbSpray.end();
					break;
				default:
					break;
			}
		fbo_curMode.end();
		fbo_curMode.draw(0,0);
		
		if(transitionActive==1){
			fbo_nextMode.begin();
				switch(nextShaderMode){
					case 0:
						ofs_rgbMode.begin();
							ofs_rgbMode.setUniform1f("cW",(float)sW);
							ofs_rgbMode.setUniform1f("cH",(float)sH);
							ofs_rgbMode.setUniform1f("runner",(float)runner);
							ofs_rgbMode.setUniform1i("mode",nextShaderMode);
							ofDrawRectangle(0,0,sW,sH);
						ofs_rgbMode.end();
						break;
					case 1:
						ofs_rgbNoiseWave.begin();
							ofs_rgbNoiseWave.setUniform1f("cW",(float)sW);
							ofs_rgbNoiseWave.setUniform1f("cH",(float)sH);
							ofs_rgbNoiseWave.setUniform1f("runner",(float)runner);
							ofs_rgbNoiseWave.setUniform1i("mode",nextShaderMode);
							ofDrawRectangle(0,0,sW,sH);
						ofs_rgbNoiseWave.end();
						break;
					case 2:
					case 3:
						ofs_rgbRoll.begin();
							ofs_rgbRoll.setUniform1f("cW",(float)sW);
							ofs_rgbRoll.setUniform1f("cH",(float)sH);
							ofs_rgbRoll.setUniform1f("runner",(float)runner);
							ofs_rgbRoll.setUniform1i("mode",nextShaderMode);
							ofDrawRectangle(0,0,sW,sH);
						ofs_rgbRoll.end();
						break;
					case 4:
					case 5:
					case 6:
						ofs_rgbKaleido.begin();
							ofs_rgbKaleido.setUniform1f("cW",(float)sW);
							ofs_rgbKaleido.setUniform1f("cH",(float)sH);
							ofs_rgbKaleido.setUniform1f("runner",(float)runner);
							ofs_rgbKaleido.setUniform1i("mode",nextShaderMode);
							ofDrawRectangle(0,0,sW,sH);
						ofs_rgbKaleido.end();
						break;
					case 7:
						ofs_rgbSpray.begin();
							ofs_rgbSpray.setUniform1f("cW",(float)sW);
							ofs_rgbSpray.setUniform1f("cH",(float)sH);
							ofs_rgbSpray.setUniform1f("runner",(float)runner);
							ofs_rgbSpray.setUniform1i("mode",nextShaderMode);
							ofDrawRectangle(0,0,sW,sH);
						ofs_rgbSpray.end();
						break;
					default:
						break;
				}
			fbo_nextMode.end();
			fbo_nextMode.draw(0,0);
		}
	}
	
	ofTexture curTexture=fbo_curMode.getTexture();
	ofTexture nextTexture=fbo_nextMode.getTexture();
	ofs_postProcess.begin();
		ofs_postProcess.setUniform1f("cW",(float)sW);
		ofs_postProcess.setUniform1f("cH",(float)sH);
		ofs_postProcess.setUniform1f("runner",(float)runner);
		ofs_postProcess.setUniform1i("transitionActive",transitionActive);
		ofs_postProcess.setUniform1f("transitionTimer",transitionTimer);
		ofs_postProcess.setUniformTexture("curPass",curTexture,0);
		ofs_postProcess.setUniformTexture("nextPass",nextTexture,1);
		ofDrawRectangle(0,0,sW,sH); 
	ofs_postProcess.end();

/*		ofSetColor(255, 0, 0);
		ofDrawBitmapString( ofToString( transitionTimer ) + " -- \n" + ofToString( curShaderMode ) + " -- \n" + ofToString( nextShaderMode ) + " -- \n"
			+ " --\n ", 20, 20);
*/
			
}
/*
Space - 32
Tab - 9
Caps - 0
Shift Left - [1,3680]
Shift Right - [1,3681]
Ctrl Left - [2, 3682]
Ctrl Right - [2, 3683]
Alt Left - [4, 3684]
Alt Right - [4, 3685]
Left - 57356
Up - 57357
Right - 57358
Down - 57359
Enter - 13
Backspace - 8
Delete - 127
Insert - 57364
F1 - 57344
F2 - 57345
F3 - 57346
F4 - 57347
F5 - 57348
F6 - 57349
F7 - 57350
F8 - 57351
F9 - 57352
F10 - 57353
F11 - 57354
F12 - 57355

// US Keyboard, 1-0, a-z; (No Shift Held)
1 - 49
2 - 50
3 - 51
4 - 52
5 - 53
6 - 54
7 - 55
8 - 56
9 - 57
0 - 48
- - 45
= - 61
q - 113
w - 119
e - 101
r - 114
t - 116
y - 121
u - 117
i - 105
o - 111
p - 112
[ - 91
] - 93
\ - 92
a - 97
s - 115
d - 100
f - 102
g - 103
h - 104
j - 106
k - 107
l - 108
; - 59
' - 39
z - 122
x - 120
c - 99
v - 118
b - 98
n - 110
m - 109
, - 44
. - 46
/ - 47

// -- //
// US Keyboard, !-), A-Z; (Shift Held)
! - 33
@ - 64
# - 35
$ - 36
% - 37
^ - 94
& - 38
* - 42
( - 40
) - 41
_ - 95
+ - 43
Q - 81
W - 87
E - 69
R - 82
T - 84
Y - 89
U - 85
I - 73
O - 79
P - 80
{ - 123
} - 125
| - 124
A - 65
S - 83
D - 68
F - 70
G - 71
H - 72
J - 74
K - 75
L - 76
: - 58
" - 34
Z - 90
X - 88
C - 67
V - 86
B - 66
N - 78
M - 77
< - 60
> - 62
? - 63
*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//std::cout<<key;
	// Space - 
	if(key==4){ // Any Alt
		shiftDown=1;
	}
	if(key==13){ // Enter
		if(shiftDown==1){
			ofToggleFullscreen();
		}
	}
	if(key==102 || key==70){ // Enter || f || F
		ofToggleFullscreen();
	}
	
	if(key==32){ // Space
		active=(active+1)%2;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key==4){
		shiftDown=0;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	mouseX = min(sWd, (mouseX + double(x))*.5 );
	mouseY = (mouseY + double(y))*.5;
	mouseY = max(0., sHd - mouseY);
	mouseXNorm = (mouseX / sWd);
	mouseYNorm = (mouseY / sHd);
	if(mouseDown==0){
		mouseDrag+=1;
	}

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//std::cout<<"\n "+button;
	//std::cout<<"\n dragged!";
	mouseDown=0;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouseDown=0;
	mouseDrag=0;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//std::cout<<"\n drag button - "+button;
	//std::cout<<"\n Dragged"+mouseDrag;
	//modeTransition(1);
	mouseDown=0;
	if(transitionActive==0){
		nextShaderMode=(nextShaderMode+1)%(int)colorModes;
		transitionActive=1;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	sW = ofGetWidth();
	sH = ofGetHeight();

	fbo_curMode.allocate(sW, sH, GL_RGB);
	fbo_curMode.begin();
		ofClear(0,0,0);
	fbo_curMode.end();
	fbo_nextMode.allocate(sW, sH, GL_RGB);
	fbo_nextMode.begin();
		ofClear(0,0,0);
	fbo_nextMode.end();

	forceCook=1;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}