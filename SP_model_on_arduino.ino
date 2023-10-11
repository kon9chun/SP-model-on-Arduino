#include "InterpolationLib.h"

double socp,socn,dcsen,dcsep,Vcell;
double scsp=0,scsn=0;
int i;
double qaven=0,qavep=0,dqaven,dqavep;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //double xValue[3]={10,20,30};
  double Dsn=3.9e-14;
  double Dsp=1e-13;

  double Rsn=12.5e-6;
  double Rsp=8e-6;
  double epssn=0.471;
  double epssp=0.297;
  
  double Ln=100e-6;
  double Ls=52e-6;
  double Lp=183e-6;

  double Lsum=Ln+Lp+Ls;

  double csmaxneg=26390;
  double csmaxpos=22860;

  double cs0neg=14870;
  double cs0pos=3900;

  double Rohm=6e-3, A=1,F=96485.33289;
  
  double soc0n=cs0neg/csmaxneg,soc0p=cs0pos/csmaxpos;
  double Jn=-17.5*Rsn/3/epssn/F/A/Ln, Jp=17.5*Rsp/3/epssp/F/A/Lp;

  for(i=1;i<=3549;i++){
    double aven=Jn*3/Rsn;
    double avep=Jp*3/Rsp;
    scsn=scsn+aven;
    scsp=scsp+avep;
    dqaven=-30*Dsn*qaven/Rsn/Rsn+45*Jn/2/Rsn/Rsn;
    qaven=qaven+dqaven;
    dcsen=8*Rsn*qaven/35+Rsn*Jn/35/Dsn;

    dqavep=-30*Dsp*qavep/Rsp/Rsp+45*Jp/2/Rsp/Rsp;
    qavep=qavep+dqavep;
    dcsep=8*Rsp*qavep/35+Rsp*Jp/35/Dsp;

    socn=soc0n+scsn/csmaxneg+dcsen/csmaxneg;
    socp=soc0p+scsp/csmaxpos+dcsep/csmaxpos;

    Vcell=Eeqp(socp)-Eeqn(socn)-17.5*Rohm;

    //delay(100);millis()

    Serial.println(Vcell,6);

    //socn=soc0n+scsn/csmaxneg+dcsen/csmaxneg;
    //socp=soc0p+scsp/csmaxpos+dcsep/csmaxpos;
    
  }

  
}

void loop() {
  // put your main code here, to run repeatedly:
  //for(i=1;i<=3549;i++){
    //ssocn=Jn*3/Rsn;
    //ssocp=Jp*3/Rsp;

  //}

}

double Eeqn(double val){
  double un=-0.16+1.32*exp(-3*val)+10*exp(-2000*val);
  return un;
}

double Eeqp(double val){
  double up=4.19829+0.0565661*tanh(-14.5546*val+8.60942)-0.0275479*(1/pow((0.998432-val),0.492465)-1.90111)-0.157123*exp(-0.04738*pow(val,8))+0.810239*exp(-40*(val-0.133875));
  return up;
}
