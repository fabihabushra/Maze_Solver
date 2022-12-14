//things to change each bot: motorpins, kp, kd, maxSpeed, rotationSpeed,turnDelay,buzzer,contiBlack,contiWhite;
#define sensorNum 8
#define maxSpeed 100
#define rotationSpeed 70

int blackLimit[sensorNum];

const int motorPin1 = 9,motorPin2 = 10;        //right motor
const int motorPin3 = 6,motorPin4 = 5;       //left motor

float error, prevError=0;
float mappedValue, targetValue = 7;     

float safety=0.35;

float kp=45;                         //40
float kd=50;                       //10
                              

int motorResponse;
float correction;

int nodeDirection[100]; //(where bot went while mapping, will be opposite while coming back) 0 for right; 1 for front; 2 for left;
int nodeOptions[100]; 
int currentNode=0;
int currentNodeType=0; //1 for RFL; 2 for RF; 3 for FL; 4 for RL;
int turnDelay=700,forwardDelay=150;
int buzzer=12;

int prevNode=0;

int digitalReading[sensorNum];
int leftSpeed,rightSpeed;
int pidAllWhite=0,turnAllWhite=0,allWhite=0,fSpeed=150 ;
int leftIR,rightIR,frontIR;
int contiBlack=40,resetIRCounter=0;
int contiWhite=1000,mappingEndCounter=0,stopCounter=0;

int incomingByte;

float time=2.8;

int prev, curr, diff;


void setup()
{
  //initialize IR pins
  for(int i = 0; i < sensorNum; i++)
  {
    pinMode(A0 + i, INPUT);
  }

  //initialize motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT); 
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(buzzer,OUTPUT);

  
  delay(1000);
    Serial.begin(38400);
calibration();
  

}



void loop()
{
sensorMapping();

// send data only when you receive data:
if (Serial.available() > 0) 
{
        // read the incoming byte:
        incomingByte = Serial.read();

        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
}  
 
//sensorRead();

if(currentNode!=prevNode)
{
  Serial.print("CurrentNode: ");
  Serial.print(currentNode);
}

prevNode=currentNode;
/*
// For IR Reading
for(int i = 0; i < sensorNum; i++)


{
  //Serial.print(analogRead(A0+i));
  if(analogRead(A0+i)<blackLimit[i])
 Serial.print("B");
 else Serial.print("W");
 Serial.print(" ");
} 
Serial.println();
 
 Serial.print(analogRead(A0+i));
 Serial.print(" && ");
 Serial.print(blackLimit[i]);
 Serial.print(" ||| ");
}
Serial.println();
Serial.print(leftIR);
Serial.print(" & ");
Serial.println(rightIR);
*/

/*
//movement
if(mappingEndCounter>contiWhite)
{
  while(mappingEndCounter>0)
  {
  brake();
  sensorMapping();
  }
}
*/


//return from wrong path
if(mappedValue==111)
      {
        motor(maxSpeed,maxSpeed);
        delay(400);
        while(mappedValue!=targetValue)
      {
        
        
        plannedACRotate();
        sensorMapping();
      }}
/*
{ 
  //return for RFL
  if(currentNodeType==1)
  {
    { nodeDirection[currentNode]++;
     nodeOptions[currentNode]--;
      while(mappedValue==111)
      {
        plannedACRotate();
        sensorMapping();
      } pid(); motor(leftSpeed,rightSpeed);
      if (rightIR==0 && frontIR==0 && leftIR==0 ) 
      {
        if(nodeOptions[currentNode]!=0)
        {
        plannedCRotate();
        delay(turnDelay);
        } else 
        {
        currentNode-=2;
        plannedCRotate();
        delay(turnDelay); 
        }
      }
    }
  }     

    //return for RF
  if(currentNodeType==2)
  {
    { nodeDirection[currentNode]++;
     nodeOptions[currentNode]--;
      while(mappedValue==111)
      {
        plannedACRotate();
        sensorMapping();
      } pid(); motor(leftSpeed,rightSpeed);
      if ((rightIR==0 && leftIR==0) || (frontIR==0 && leftIR==0 ) )
      {
        if(nodeOptions[currentNode]!=0)
        {
        plannedCRotate();
        delay(turnDelay);
        } else 
        {
        currentNode-=2;
        motor(leftSpeed,rightSpeed);
        }
      }
    }
  }    

    //return for FL
  if(currentNodeType==3)
  {
    { nodeDirection[currentNode]++;
     nodeOptions[currentNode]--;
      while(mappedValue==111)
      {
        plannedACRotate();
        sensorMapping();
      } pid(); motor(leftSpeed,rightSpeed);
      if ((rightIR==0 && frontIR==0) || (rightIR==0 && leftIR==0 ) )
      {
        if(nodeOptions[currentNode]!=0)
        {
        plannedCRotate();
        delay(turnDelay);
        } else 
        {
        currentNode-=2;
        plannedCRotate();
        delay(turnDelay); 
        }
      }
    }
  }     

    //return for RL
  if(currentNodeType==4)
  {
    { nodeDirection[currentNode]+=2;
     nodeOptions[currentNode]--;
      while(mappedValue==111)
      {
        plannedACRotate();
        sensorMapping();
      } pid(); motor(leftSpeed,rightSpeed);
      if ((leftIR==0 && frontIR==0) || (rightIR==0 && leftIR==0 ) )
      {
        if(nodeOptions[currentNode]!=0)
        {
        motor(leftSpeed,rightSpeed);
        } else 
        {
        currentNode-=2;
        plannedCRotate();
        delay(turnDelay); 
        }
      }
    }
  }   

  
}
*/
    

if(digitalReading[0]==0 && digitalReading[7]==0)
{
  Serial.print("AllWhiteFound");
  
  Serial.println();
  motor(fSpeed,fSpeed);
  delay(forwardDelay);
  brake();
  sensorMapping();
  if (mappedValue==111)
  {
    
    Serial.print("RL ");
    currentNode++;
    nodeDirection[currentNode]=0;
    nodeOptions[currentNode]=2;
    currentNodeType=4;
    
    printValues();
    Serial.println();
    while(mappedValue!=targetValue-1)
    {
    plannedCRotate();
    sensorMapping();
    } 

    
  } 
  else
  {
    if(digitalReading[0]==1 && digitalReading[7]==1 )
    {
      Serial.print("RFL ");
      currentNode++;
      nodeDirection[currentNode]=0;
      nodeOptions[currentNode]=3;
      currentNodeType=1;
    
      printValues();
      Serial.println();
      while(mappedValue==targetValue)
      {
      plannedCRotate();
      sensorMapping();
      } 
      while(mappedValue!=targetValue)
      {
      plannedCRotate();
      sensorMapping();
      }
       
    }
    else 
    {
      Serial.println("End");
      brake();
      delay(3000);
    }

  }
} 
else if(digitalReading[0]==0 && digitalReading[1]==0 &&  digitalReading[7]==1 )
{
  motor(fSpeed,fSpeed);
  delay(forwardDelay);
  brake();
  sensorMapping();
  if (mappedValue==111)
  {
    Serial.print("LeftTurn ");
    printValues();
    Serial.println();
    while(mappedValue!=targetValue+2)
    {
    plannedACRotate();
    sensorMapping();
    } 

    
  } 
  else
  {
    Serial.print("FL ");
    currentNode++;
    nodeDirection[currentNode]=1;
    nodeOptions[currentNode]=2;
    currentNodeType=3;
    
    printValues();
    Serial.println();
    leftIR=1;
    motor(maxSpeed,maxSpeed);
    sensorMapping();
  }
} 
else if(digitalReading[0]==1 && digitalReading[6]==0 &&  digitalReading[7]==0 )
{
  motor(fSpeed,fSpeed);
  delay(forwardDelay);
  brake();
  sensorMapping();
  if (mappedValue==111)
  {
    Serial.print("RightTurn ");
    printValues();
    Serial.println();
    while(mappedValue!=targetValue-1)
    {
    plannedCRotate();
    sensorMapping();
    } 

    
  } 
  else
  {
    
    Serial.print("RF ");
    currentNode++;
    nodeDirection[currentNode]=0;
    nodeOptions[currentNode]=2;
    currentNodeType=2;
    
    
    printValues();
    Serial.println();
    rightIR=1;
    while(mappedValue>=targetValue-1)
    {
    plannedCRotate();
    sensorMapping();
    
    } 
    while(mappedValue!=targetValue)
    {
    plannedCRotate();
    sensorMapping();
    } 
  }
}

  
 else
   {  
      pid();
      motor(leftSpeed,rightSpeed);
   }
   

 } 
  

  


 


void pid()
{
  
  error=targetValue-mappedValue;
  correction=(kp*error)+(kd*(error-prevError));
  prevError=error;
  motorResponse=(int)correction;
 
 if(motorResponse>maxSpeed) motorResponse=maxSpeed;
 
if(motorResponse<-maxSpeed) motorResponse=-maxSpeed;

   if(motorResponse>0)
  {
    rightSpeed=maxSpeed;
    leftSpeed=maxSpeed-motorResponse;
  }
  else 
  {
    rightSpeed=maxSpeed+ motorResponse;
    leftSpeed=maxSpeed;
  }

}

void motor(int left, int right)
{
  
  if(right>0)
  {
  analogWrite(motorPin1,right);
  analogWrite(motorPin2,0);
  }
  else
  {
    analogWrite(motorPin1,0);
    analogWrite(motorPin2,-right);
  }

  if(left>0)
  {
  analogWrite(motorPin3,left);
  analogWrite(motorPin4,0);
  }
  else
  {
   analogWrite(motorPin3,0);
   analogWrite(motorPin4,-left); 
  }

 }



void brake(void)
{
  analogWrite(motorPin1, 0);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
}

void plannedACRotate()
{
  analogWrite(motorPin1,rotationSpeed);
  analogWrite(motorPin2, 0);
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4,rotationSpeed);

}

void plannedCRotate()
{
  analogWrite(motorPin1,0);
  analogWrite(motorPin2, rotationSpeed);
  analogWrite(motorPin3, rotationSpeed);
  analogWrite(motorPin4,0);

}



void sensorMapping()
{
int sum=0,pidDetect=0, mappingEndDetect=0; 
 
 for (int i = 0; i <sensorNum; i++)
  { 
    
    if (analogRead(A0+i) > blackLimit[i])           
     { 

      if(i>0 && i<sensorNum-1){sum += i*2; pidDetect++;}
      mappingEndDetect++;
      digitalReading[i]= 0;
    } else digitalReading[i]= 1;
    

  }
   if(pidDetect!=0 ){  
  mappedValue = sum / pidDetect;
   }

   else mappedValue=111;

if(digitalReading[0]==0 || digitalReading[7]==0)
{
  leftIR=digitalReading[0];
  rightIR=digitalReading[7];
  resetIRCounter=0;
} 

if(digitalReading[0]==1 && digitalReading[sensorNum-1]==1)
{
 resetIRCounter++;
 if(resetIRCounter>contiBlack)
 {
 leftIR=1;
 rightIR=1;
 }
}

if(mappingEndDetect==sensorNum)
{
mappingEndCounter++;

} else {mappingEndCounter=0;}

}

void printValues()
{
  Serial.print(mappedValue);
  Serial.println();
  
  
  
  for(int i = 0; i < sensorNum; i++)
  {
  //Serial.print(analogRead(A0+i));
  if(analogRead(A0+i)<blackLimit[i])
   Serial.print("1");
   else Serial.print("0");
   Serial.print(" ");
  } 
Serial.println();

}

//auto calibration 
void calibration()
{
  plannedCRotate();
  float upSum = 0,lowSum = 0;
  int sensorArray[sensorNum][2];

  for(int i = 0; i < sensorNum; i++)
    {
      sensorArray[i][0] = analogRead(A0+i);
      sensorArray[i][1] = analogRead(A0+i);
    }
 

  int loopCounter = (int)(time * 1000 / 2.5);  
  while(loopCounter)
  {
    for(int i = 0; i < sensorNum; i++)
    {
      if(analogRead(A0+i)<sensorArray[i][0]) sensorArray[i][0]=analogRead(A0+i);
      if(analogRead(A0+i)>sensorArray[i][1]) sensorArray[i][1]=analogRead(A0+i);
    }
  loopCounter--;

  }

 for(int i=0; i < sensorNum; i++)
  blackLimit[i] = (int)(sensorArray[i][0] + safety * (sensorArray[i][1] - sensorArray[i][0]));
prev = millis();
sensorMapping ();
curr= millis();
diff = curr - prev;

  brake();
  delay(1000);

}

