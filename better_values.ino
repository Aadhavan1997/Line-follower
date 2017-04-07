/*a0d-left sensor
  a1d-mid sensor
  a2d-right sensor
  a3d-front sensor
  e-error
  pe-previous error
  prope-error in proportional
  pprope-previous error in proportional
  Have to set value for kp and for base_value
*/
int a0d,a1d,a2d,a3d,value,prope,pprope,e,pe,kp=35,base_value=160;
int corr,lmspeed,rmspeed;
int derve,kd=60,ki=0,inte=0;
void prop(){
  /*if(a3d==0&&value==7){
    while(a3d==0)
    {
      getsensorvalue();
      digitalWrite(8,HIGH);
      digitalWrite(7,LOW);
      analogWrite(11,0);
      digitalWrite(4,HIGH);
      digitalWrite(2,LOW);
      analogWrite(10,200);
    }
  }*/
  switch(value)
  {
    case 2:prope=0;
           break;
    case 3:prope=-2;
           break;
    case 1:prope=-4;
           break;
    case 0:if(pprope==-2||pprope==-4||pprope==-8)
             prope=-8;
           else if(pprope==2||pprope==4||pprope==8)
             prope=8;
           else if(pprope==0)
             prope=0;
           break;
    case 6:prope=2;
           break;
    case 4:prope=4;
           break;
  }
  if(a3d==1&&value==0)
    prope=0;
  else if(a3d==0&&value==7)
    prope=2;
  else if(a3d==1&&value==7)
    prope=0;
    
}
void getsensorvalue(){
  a0d=analogRead(A0)>300?1:0;
  a1d=analogRead(A1)>300?1:0;
  a2d=analogRead(A2)>300?1:0;
  a3d=analogRead(A3)>250?1:0;
  value=4*a0d+2*a1d+a2d;
}
void calc_corr(){
  corr=kp*prope+kd*derve+ki*inte;
}
void derv(){
  derve=(prope-pprope);
}
void integral(){
  inte+=prope;
}
void drive_motor(){
  lmspeed=base_value-corr;
  rmspeed=base_value+corr;
  if(rmspeed>255)
    rmspeed=255;
  else if(rmspeed<-255)
    rmspeed=-255;
  if(lmspeed>255)
    lmspeed=255;
  else if(lmspeed<-255)
    lmspeed=-255;
  
  if(lmspeed>=0)
  {
    digitalWrite(8,HIGH);
    digitalWrite(7,LOW);
    analogWrite(11,lmspeed);
  }
  else if(lmspeed<0)
  {
    digitalWrite(8,LOW);
    digitalWrite(7,HIGH);
    analogWrite(11,-lmspeed);
  }
  if(rmspeed>=0)
  {
    digitalWrite(4,HIGH);
    digitalWrite(2,LOW);
    analogWrite(10,rmspeed);
  }
  else if(rmspeed<0)
  {
    digitalWrite(4,LOW);
    digitalWrite(2,HIGH);
    analogWrite(10,-rmspeed);
  }
  delay(50);
  
}
  
void setup(){
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  pprope=0;
  prope=0;
  e=0;
  pe=0;
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(12,HIGH);
  digitalWrite(13,LOW);
  digitalWrite(9,HIGH);  
  Serial.begin(9600);
}
void loop(){
  getsensorvalue();
  pprope=prope;
  prop();
  derv();
  integral();
  calc_corr();
  drive_motor();
}
