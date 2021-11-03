/*
   Zumo, the new rover, using functions
   Carlos Gomez Cubero

   This code implements a list of commands that Zumo has to execute to reach a desired position
   It uses two arrays, one with the comands, and other with two dimenions containing the speed
   and the execution time

   Zumo waits for the A button to be pressed to run the commands.
   Once the commands are done Zumo must stop

   Comand list:
      f -> move forward
      b -> move backwards
      l -> turn left
      r -> turn right
      s -> stop

*/

#include <Wire.h>         // libraries needed to use zumo functions​
#include <Zumo32U4.h>​
Zumo32U4Motors motors;    // instanciate the motors

struct RoverControl{
  char command;
  int speed;
  int time;

  int getSpeed(){
    return speed;
  }

  void setSp(int s){
    speed = s;
  }

  void setTime(int t){
    time = t;
  }

  int getTime(){
    return time;
  }
  
  void wait(){
    delay(time);
  }
};

int randNumber;
int const nCommands = 11;
RoverControl rcArray[nCommands];
/*
char commands[8] =   {'f' , 'l' , 'f' , 'r' , 'b' , 'r' , 'f', 's'   };           // commands
int  params[2][8] = {{100 , 100 , 100 , 100 , 100 , 150 , 200, 0     },           // speeds
                     {1000, 1500, 1000, 1000, 1500, 1000, 750, 1000  }};          // time of execution
*/

int buttonA = 14;
int YLED = 13;

void setup() {
  Serial.begin(9600);
  pinMode(buttonA,INPUT);
  pinMode(YLED   ,OUTPUT);
  randomSeed(analogRead(0));  // initialize the seed of the random generator
//  createCommands();
}

void loop() {
  createCommands();
  digitalWrite(YLED,HIGH); // turn on Yellow LED to shot that the robot is waiting
  //when pressing the A button the commands are triggered
  if(digitalRead(buttonA) == 0){
    digitalWrite(YLED,LOW); // turn off Yellow LED to shot that the robot is not waiting
    
    for (int i = 0; i < nCommands; i++) {
      Serial.println("Command-> " + (String)rcArray[i].command + " Params-> speed: " + (String)rcArray[i].getSpeed() +  " time: " + rcArray[i].getTime());
      
      switch (rcArray[i].command) { // switch for the different possibilities
  
        case 'f':
          moveForward(rcArray[i].getSpeed(),rcArray[i].getTime());
          break;
        
        case 'b':
          moveBackwards(rcArray[i].getSpeed(),rcArray[i].getTime());
          break;
        
        case 'l':
          turn('l',rcArray[i].getSpeed(),rcArray[i].getTime());
          break;
        
        case 'r':
          turn('r',rcArray[i].getSpeed(),rcArray[i].getTime());
          break;
        
        default:
          Stop();
          break;
  
      }
      delay(500);
    }
  }  
}

void createCommands(){
  for(int i=0; i<nCommands-1; i++){
    RoverControl rc;
    randNumber = random(1, 5);
    switch (randNumber){
      case 1: rc.command = 'f';break;
      case 2: rc.command = 'b';break;
      case 3: rc.command = 'l';break;
      default: rc.command = 'r';break;
    }
    rc.setSp(random(50,400));
    if (rc.command == 'l' || rc.command == 'r')
      rc.setTime(random(200,500));
      else rc.setTime(random(1000,3000)); 
    rcArray[i]=rc;
  }
  rcArray[nCommands-1].command = 's';
  rcArray[nCommands-1].setSp(0);
  rcArray[nCommands-1].setTime(random(1000,3000));
}

void Stop(){
   motors.setSpeeds(0,0);
}

void moveForward(int speed, int millisec){
  motors.setSpeeds(speed,speed);
  delay(millisec);
  Stop();
}

void moveBackwards(int speed, int millisec){
  motors.setSpeeds(-speed,-speed);
  delay(millisec);
  Stop();
}

void turn(char direction, int speed, int millisec){
  switch(direction){
      case 'l':
        motors.setSpeeds(-speed,speed);
        break;
        
      case 'r':
        motors.setSpeeds(speed,-speed);
        break;
  }
  delay(millisec);
  Stop();
}
