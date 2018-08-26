#include <Servo.h>
/* pos[SERBO_NO][Posishon memory] */
// Posistion: 0 = get new analog data
//            1 = Prebias
//            2 = Now.

int pos[5][5];

// Servo
Servo servo[5];
int rev[5]; // reverse

//volume angle
int vL[5];
int vH[5];


// Repeat Memory
int rs[50][5];
int no=0;

/* 動作記録用配列*/
int b1,b2;
/*ボタン*/

// Analog Read
int a[5];

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
  servo[0].attach(9);
  servo[1].attach(10);
  servo[2].attach(11);
  servo[3].attach(12);
  pos[0][2]=90;
  pos[1][2]=90;
  pos[2][2]=90;
  pos[3][2]=90;
  vL[0] = 153;
  vH[0] = 875;
  vL[1] = 153;
  vH[1] = 875;
  vL[2] = 153;
  vH[2] = 875;
  vL[3] = 153;
  vH[3] = 875;

  rev[0]=0;
  rev[1]=0;
  rev[2]=1;
  rev[3]=0;
  

  //led
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(6, INPUT);
  pinMode(7, INPUT);
  
  pinMode(13, OUTPUT);

// Serial.begin(9600);
}

void loop()
{
    digitalWrite(13, LOW);
  	readPos();
  // recode switch 
  	b1 = digitalRead(6); // exec
    b2 = digitalRead(7); // recode
    if(b1 == 1 || no>=30) {  //exec
       digitalWrite(13, 0); 
       automove();
          /* break; */
    }
    if(b2==1){
       digitalWrite(13, 1); // recode
       no++;
       for(int i=0;i<=3;i++){
          rs[no][i]=pos[i][2];
       }
       binout(no);
       b2=0;
       delay(200);
    }
}

void readPos(){
  for(int i=0;i<=3;i++){
    a[i]=analogRead(i);
//**********************************  
/*   Serial.print(i);
    Serial.print("=");
    Serial.print(pos[i][2]);
    if(i==3) Serial.println(" ");
      else  Serial.print(",");
*/
//****************************************** 
    pos[i][0] = rimit_servo(map(a[i], vL[i], vH[i], 0, 179));
    if(rev[i] == 1) pos[i][0]=180-pos[i][0];
    
    if(abs(pos[i][0]-pos[i][2])>2){
      if(pos[i][0]>pos[i][2]){
      	servo[i].write(++pos[i][2]);
      }else{
      	servo[i].write(--pos[i][2]);
      }
    }
  }
}


void automove(){
    // Repeat robot
    int chack[4];
    digitalWrite(13, 1); 
    while(1){
        for(int i=1;i<=no;i++){
		    binout(i);
          for(int i=0;i<=3;i++) chack[i]=0;          
    	    for(int r=1;r<180;r++){ 
        	    if(chack[0]==1 && chack[1]==1 && chack[2]==1 && chack[3]==1) break;
            	for(int s=0;s<=3;s++){
            	  if(pos[s][2]==rs[i][s]){
            	    chack[s]=1;
            	  }else{
          	      if(pos[s][2]>rs[i][s]){
             		    servo[s].write(--pos[s][2]);
                  }else{
             		    servo[s].write(++pos[s][2]);
                  }
          	    }
          	    delay(5); //speed
        	    }
          }
          delay(1000);
        }
    }
}

int rimit_servo(int angle){
  if(angle >=179) return(179);
  if(angle <=0) return(0);
  return(angle);
}

void binout(int y){
//all off LEDs
for(int i=1;i<=5;i++) digitalWrite(i,LOW);
// Plot Y on LED with BINALY
int n=1;
int x;
  while(y>0){
    x=y%2;
    y=y/2;
    if(x == 1){
    	digitalWrite(n,HIGH);
    }else{
    	digitalWrite(n,LOW);
    }  
    n++;
  }
}