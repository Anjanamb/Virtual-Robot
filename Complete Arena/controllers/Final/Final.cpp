#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <windows.h>
#include <webots/PositionSensor.hpp>
#include <webots/Camera.hpp>

#define TIME_STEP 16
double Last_erro = 0;
double Final_erro = 0;
double Total_erro = 0;
using namespace webots;

void Right_turn(Motor *Motor_right, Motor *Motor_left, Robot *robot, DistanceSensor *ps[12], int limet, int forwerd = 10, int turn =2){
  int count =1;
  while (robot->step(TIME_STEP) != -1){
  double Values =  ps[3]->getValue();
    count = count + 1; 
    if (count < forwerd){Motor_right->setVelocity(-3); Motor_left->setVelocity(-3);}
    else if ( count % turn == 0){Motor_right->setVelocity(-3); Motor_left->setVelocity(-3);}    
    else{Motor_right->setVelocity(3); Motor_left->setVelocity(-3);}
    if (count>limet && Values>50 ){return;}}
  }

void Left_turn(Motor *Motor_right, Motor *Motor_left, Robot *robot, DistanceSensor *ps[12], int limet, int forwerd = 10, int turn =2){
  int count =1;
  while (robot->step(TIME_STEP) != -1){
  double Values =  ps[3]->getValue();
    count = count + 1; 
    if (count < forwerd){Motor_right->setVelocity(-3); Motor_left->setVelocity(-3);}
    else if ( count % turn == 0){Motor_right->setVelocity(-3); Motor_left->setVelocity(-3);}    
    else{Motor_right->setVelocity(-3); Motor_left->setVelocity(3);}
    if (count>limet && Values>50){return;}}
  }


void linefollow(Motor *Motor_right, Motor *Motor_left, DistanceSensor *ps[12], int Error1){
  double Kd = 2;
  double Error = 0;
  double weight[7] = {-8., -4., -2., 0., 2., 4., 8.};
  for (int i = 0; i < 7; i++){
    double Values =  ps[i]->getValue();
    if (Values>50){Error = Error + weight[i];}}
    
  Final_erro = Error +0.5*(Error-Last_erro) + 0*Total_erro;
  Last_erro = Error;
  Total_erro = Total_erro + Error; 
  if(Final_erro>=-Error1 && Final_erro <Error1 ){Motor_right->setVelocity(-4); Motor_left->setVelocity(-4);} 
  else{ 
  if(Final_erro*Kd>=10) {Final_erro = 10/Kd;} 
  else if(Final_erro*Kd<=-10 ) {Final_erro = -10/Kd;} 
  else {Final_erro = Final_erro;}  
  Motor_right->setVelocity(Final_erro*Kd);
  Motor_left->setVelocity(-Final_erro*Kd);}}
 
    
void Boxcatch(Motor *Motor_right, Motor *Motor_left, Robot *robot, PositionSensor *ds[4], DistanceSensor *ps[12]){
  std::cout << "Strat arm";
  if(ps[7]->getValue() < 2){
  while (robot->step(TIME_STEP) != -1) {
  Motor_right->setVelocity(1); Motor_left->setVelocity(1);
  if(ps[7]->getValue() > 5.5){break;}
  }}
  //if ( ds[0]->getValue()* -1 < 0 ) {return;} // do nothing
  Motor *Motor_arm = robot->getMotor("arm_motor1");
  Motor *slider1 = robot->getMotor("slider1");
  Motor *slider2 = robot->getMotor("slider2"); 
  Motor_arm->setPosition(0);slider1->setPosition(0.0233);slider2->setPosition(0.0233);
  Motor_arm->setVelocity(0.0);slider1->setVelocity(0);slider2->setVelocity(0);  
  
  Motor_right->setVelocity(0.0);Motor_left->setVelocity(0.0);
  Motor_arm->setVelocity(0.0);Motor_arm->setVelocity(2);   
  
  while (robot->step(TIME_STEP) != -1) {
    if ( ds[2]->getValue() < 0.00000001  ) {break;}}
  
  slider1->setVelocity(2);slider2->setVelocity(2);
  
  while (robot->step(TIME_STEP) != -1) {
  if ( ds[3]->getValue() > 0.022   ) {break;}}
  
  while (robot->step(TIME_STEP) != -1) {
    Motor_arm->setPosition(3.14);Motor_arm->setVelocity(2.0);
    if ( ds[2]->getValue() > 3.13  ) {return;}}
  }
  
int getColorFrontFace(Robot*robot,int value1){
  Camera*Cam1=robot->getCamera("camera_front");
  Cam1->enable(TIME_STEP);
  const unsigned char*img=Cam1->getImage();
  int imgW=Cam1->getWidth();
  
  int r=Cam1->imageGetRed(img,imgW,32,32);
  int b=Cam1->imageGetBlue(img,imgW,32,32);
  int g=Cam1->imageGetGreen(img,imgW,32,32);
  
  using namespace std;
  
  if(r>g && r>b){
    cout<<"RED"<<endl;
    value1=1;
  }
  else if(g>r && g>b){
    cout<<"GREEN"<<endl;
    value1=2;
  }
  else{
    cout<<"BLUE"<<endl;
    value1=3;
  }  
  return value1;
}

int getColorBottomFace(Robot*robot,int value2){
  Camera*Cam2=robot->getCamera("camera_top");
  Cam2->enable(TIME_STEP);
  const unsigned char*img=Cam2->getImage();
  int imgW=Cam2->getWidth();
  
  int r=Cam2->imageGetRed(img,imgW,32,32);
  int b=Cam2->imageGetBlue(img,imgW,32,32);
  int g=Cam2->imageGetGreen(img,imgW,32,32);
  
  using namespace std;
  
  if(r>g && r>b){
    cout<<"RED"<<endl;
    value2=1;
  }
  else if(g>r && g>b){
    cout<<"GREEN"<<endl;
    value2=2;
  }
  else{
    cout<<"BLUE"<<endl;
    value2=3;
  }  
   return value2; 
}

void wallfollowe(int avoidObstacleCounter, Motor *Motor_right, Motor *Motor_left, DistanceSensor *ps[12]){  
  double leftSpeed = -4.0;
  double rightSpeed = -4.0;
  if (avoidObstacleCounter > 0) {avoidObstacleCounter--;} 
  else {
        if ((ps[8]->getValue() < 1200.0) && (ps[8]->getValue() < ps[9]->getValue()) ){
	  leftSpeed = 3;
	  rightSpeed = -3;
             avoidObstacleCounter = 10;
          }
	else if ((ps[9]->getValue() < 1200.0) && (ps[8]->getValue() > ps[9]->getValue())){
	  leftSpeed = -3;
	  rightSpeed = 3;
             avoidObstacleCounter = 10;
          }	
	else if ((ps[8]->getValue() > 1200.0) && (ps[8]->getValue() > 140.0)){
	  avoidObstacleCounter = 10;
	  leftSpeed = -0.5;
	  rightSpeed = -0.5;
          }	}
    Motor_right->setVelocity(rightSpeed);
    Motor_left->setVelocity(leftSpeed);
    }
   
int linefollowe2(Motor *Motor_right, Motor *Motor_left, DistanceSensor *ps[12], PositionSensor *ds[4], Robot *robot, double Distance ){
    int Q=0;
    double count = 20;
    if (Distance<1.0) { 
      while (robot->step(TIME_STEP) != -1){
        count = count + 1; 
        Motor_right->setVelocity(-3); Motor_left->setVelocity(-3);
        if (count>20){return 0;}}}
    
    
    if (Distance>20.0 && Distance<200.0){Right_turn(Motor_right,Motor_left,robot, ps, 50,10,4); Q=1;}        
    
    while (robot->step(TIME_STEP) != -1){
      double Distance_now = -(ds[0]->getValue()+ ds[1]->getValue())/2 - Distance;            
      double Error = 0;
      double weight[7] = {-8., -4., -2., 0., 2., 4., 8.};
      for (int i = 0; i < 7; i++){
      double Values =  ps[i]->getValue();
      if (Values<400){Error = Error + weight[i];}}      
      double Values1 =  ps[0]->getValue();
      double Values2 =  ps[6]->getValue();
      
      if(ps[11]->getValue()<100 && Distance_now < 15 && Distance_now > 5){
      return 3;
      }
          
      else{if (Q==1){std::cout << "[ Quadrant = 1 ]";} 
      
      if (Values1 < 50 && Values2 > 50 && Distance_now>5 && Distance_now<25 ){Left_turn(Motor_right, Motor_left, robot, ps, 60,15,3);Q=0;} 
      
      else if(ps[10]->getValue()<250 && Distance_now> 25){
      Right_turn(Motor_right,Motor_left,robot, ps, 50, 15, 5);
      return 1; }
      
      else if (ps[11]->getValue()<250 && Distance_now> 15){
      Left_turn(Motor_right,Motor_left,robot, ps, 50, 5, 6);
      return 2; }
      
      else if(ps[7]->getValue()<1 && Distance_now>30) {
      return 4;}
      
      else{linefollow(Motor_right, Motor_left, ps, 2);}
      }}
    return 0;}

int linefollowe3(Motor *Motor_right, Motor *Motor_left, PositionSensor *ds[4], Robot *robot, double Distance, DistanceSensor *ps[12], int stage ){
      int Q=0;
      if(stage == 2 || stage == 5){Right_turn(Motor_right, Motor_left, robot, ps, 60, 5, 2  );
        if (stage==2){Q=2;}
        if (stage==5){Q=1;}
        }
      else if (stage == 1 || stage == 4) {Left_turn(Motor_right, Motor_left, robot, ps, 60, 5, 2);
        if (stage==1){Q=3;}
        if (stage==4){Q=2;}
        }
      while (robot->step(TIME_STEP) != -1){      
      double Error = 0;
      double weight[7] = {-8., -4., -2., 0., 2., 4., 8.};
      for (int i = 0; i < 7; i++){
      double Values =  ps[i]->getValue();
      if (Values<50){Error = Error + weight[i];}}          
      double Distance_now = -(ds[0]->getValue()+ ds[1]->getValue())/2 - Distance;   
      
      if (Q==1){std::cout << "[ Quadrant = 1 ]";}
      if (Q==2){std::cout << "[ Quadrant = 2 ]";}
      if (Q==3){std::cout << "[ Quadrant = 3 ]";} 
      std::cout << "\n";        
          
      
      if(stage== 1){          
          if ((Error > 10 || Error < -10) && Distance_now>42 ){Right_turn(Motor_right, Motor_left, robot, ps, 55, 15, 2);return 6;}          
          else if((Error > 10 || Error < -10) && Distance_now <42 ){
          Q=2;
          int c =1;
          while (robot->step(TIME_STEP) != -1){
          c = c+1;
          if (c<35){Motor_right->setVelocity(-1); Motor_left->setVelocity(-1);} else{break;}}}          
          else{linefollow(Motor_right, Motor_left, ps, 3);}
      }
      
      else if(stage== 5){
          if ((Error > 10 || Error < -10) && Distance_now> 20 ){Left_turn(Motor_right, Motor_left, robot, ps, 55, 15, 2);return 6;}          
          else if(Error > 10 || Error < -10 ){
          if (Distance_now >7){Q=2;}
          int c =1;
          while (robot->step(TIME_STEP) != -1){
          c = c+1;
          if (c<120){Motor_right->setVelocity(-1); Motor_left->setVelocity(-1);} else{break;}}}          
          else{linefollow(Motor_right, Motor_left, ps, 3);}
      }
      
      else if(stage== 4){
              
          if ((Error > 10 || Error < -10) && Distance_now>5 ){Right_turn(Motor_right, Motor_left, robot, ps, 55, 15, 2);return 6;}          
          else{linefollow(Motor_right, Motor_left, ps, 3);}      
      }
      
      else{
                               
          if ((Error > 10 || Error < -10) && Distance_now>0 ){Left_turn(Motor_right, Motor_left, robot, ps, 55, 15, 2);return 6;}          
          else{linefollow(Motor_right, Motor_left, ps, 3);}
      }
}
return 6;
}


int linefollowe2_2(Motor *Motor_right, Motor *Motor_left, PositionSensor *ds[4], Robot *robot, double Distance, DistanceSensor *ps[12], int stage ){
  
  int Q=0;
  while (robot->step(TIME_STEP) != -1){
  
  double Distance_now = -(ds[0]->getValue()+ ds[1]->getValue())/2 - Distance;            
  double Error = 0;
  double weight[7] = {-8., -4., -2., 0., 2., 4., 8.};
  for (int i = 0; i < 7; i++){
  double Values =  ps[i]->getValue();
  if (Values<50){Error = Error + weight[i];}}
      
  int Count2 = 0;  
  for (int i = 0; i < 7; i++){
  double Values =  ps[i]->getValue();
  if (Values<50){Count2 = Count2 + 1;}}
    
    if(ps[7]->getValue()<1 && Distance_now >45) {
    return 5;}
          
    else{
      if ((Error > 10 || Error < -10) && Distance_now < 30 ){
      Q=4;
      int c =1;
      while (robot->step(TIME_STEP) != -1){
      c = c+1;
      if (c<100){Motor_right->setVelocity(-1); Motor_left->setVelocity(-1);} else{break;}
      }}
            
      else if ((Error > 10 || Error < -10) && Distance_now < 45){Left_turn(Motor_right, Motor_left, robot, ps, 55, 15, 3);Q=0;}
            
      else if (Count2 == 0 && Distance_now < 90){Left_turn(Motor_right, Motor_left, robot, ps, 55, 20, 3);}
            
      else{linefollow(Motor_right, Motor_left, ps, 3);}
      if (Q==4){std::cout << "[ Quadrant = 4 ]";}
      std::cout << "\n";   
        }}
        return 0;
        }
        
        
void linefollowe5(Motor *Motor_right, Motor *Motor_left, PositionSensor *ds[4], Robot *robot, double Distance, DistanceSensor *ps[12] ){  
  int Situation = 1;
  double dis_n = 0;
  double dis_o = 0;
  while (robot->step(TIME_STEP) != -1){ 
      //double Distance_now = -(ds[0]->getValue()+ ds[1]->getValue())/2 - Distance;            
      double Error = 0;
      double weight[7] = {-8., -4., -2., 0., 2., 4., 8.};
      for (int i = 0; i < 7; i++){
      double Values =  ps[i]->getValue();      
      if (Values>50){Error = Error + weight[i];}}
      
      int Count2 = 0;  
      for (int i = 1; i < 6; i++){
      double Values =  ps[i]->getValue();
      if (Values<50){Count2 = Count2 + 1;}}
      
      dis_n =  ps[7]->getValue();
      
      if(Count2 == 0) {
      int c =1;
      while (robot->step(TIME_STEP) != -1){
      c = c+1;
      if (c<100){Motor_right->setVelocity(-2); Motor_left->setVelocity(-2);} else{break;}}
      Situation = 0;
      }       
      else if (dis_n > 600 &&  dis_n - 500 >= dis_o  ){ Situation = 1; }
      
      
      
      if (Situation == 0 ){
      Motor_right->setVelocity(0);
      Motor_left->setVelocity(0);
      }   
      else{linefollow(Motor_right, Motor_left, ps, 3);}
      dis_o = ps[7]->getValue();
      }}
      
              
void linefollowe4(Motor *Motor_right, Motor *Motor_left, PositionSensor *ds[4], Robot *robot, double Distance, DistanceSensor *ps[12], int VALUE ){
  //ps[10]->disable(); ps[11]->disable();
  if (VALUE%2 == 0) {Left_turn(Motor_right, Motor_left, robot, ps, 50, 0, 2 );}
  else{Right_turn(Motor_right, Motor_left, robot, ps, 50, 0, 2 );}
 // ps[10]->enable(TIME_STEP); ps[11]->enable(TIME_STEP);
  
  int count_piler = 0;
  double distanse_piler = 1000;
  int k1 = 0; //For count piller
  int k2 = 0; //For identyfly tern 180
  while (robot->step(TIME_STEP) != -1){ 
      double Distance_now = -(ds[0]->getValue()+ ds[1]->getValue())/2 - Distance;            
      double Error = 0;
      double weight[7] = {-8., -4., -2., 0., 2., 4., 8.};
      for (int i = 0; i < 7; i++){
      double Values =  ps[i]->getValue();
      if (Values<50){Error = Error + weight[i];}}
      
      int Count2 = 0;  
      for (int i = 0; i < 7; i++){
      double Values =  ps[i]->getValue();
      if (Values>50){Count2 = Count2 + 1;}}
      
      if((Error > 10 || Error < -10) && k2 ==0 && count_piler != 0 ) {ps[10]->disable(); ps[11]->disable();k2 = 1;
        if (count_piler%2 == 1){
         if(VALUE%2 == 1){Left_turn(Motor_right, Motor_left, robot, ps, 100, 18, 5 );}
         else{Right_turn(Motor_right, Motor_left, robot, ps, 100, 18, 5 );}}}
      
      else if((Error > 10 || Error < -10) && Distance_now<50 && Distance_now>30 && k2 ==1){
      int c =1;
      while (robot->step(TIME_STEP) != -1){
      c = c+1;
      if (c<25){Motor_right->setVelocity(-1); Motor_left->setVelocity(-1);} else{break;}}
      }
      
      
      else if((Error > 10 || Error < -10) && Distance_now>65 && k2 ==1){
      if(VALUE%2 == 1){Left_turn(Motor_right, Motor_left, robot, ps, 60, 10, 5 );
      linefollowe5(Motor_right, Motor_left, ds, robot, Distance, ps);
      }
      else{Right_turn(Motor_right, Motor_left, robot, ps, 60, 10, 5 );
      linefollowe5(Motor_right, Motor_left, ds, robot, Distance, ps);
      }}
      
      else if((Error > 10 || Error < -10) && Distance_now<30 && Distance_now>10 && k2 ==1){
      if(VALUE%2 == 0){Left_turn(Motor_right, Motor_left, robot, ps, 60, 15, 5 );
      linefollowe5(Motor_right, Motor_left, ds, robot, Distance, ps);
      }
      else{Right_turn(Motor_right, Motor_left, robot, ps, 60, 15, 5 );
      linefollowe5(Motor_right, Motor_left, ds, robot, Distance, ps);
      }}
         
      
      
 
      else{linefollow(Motor_right, Motor_left, ps, 3);}
      
      
      if (VALUE%2 == 1 && k2 ==0){distanse_piler = ps[10]->getValue();}
      else if(k2 == 0){distanse_piler = ps[11]->getValue();}
      if (distanse_piler <70.0 && k1 ==0){ count_piler = count_piler + 1; k1 = 1;}
      else if(distanse_piler <70.0){k1 = 1;}
      else{k1 = 0;}
        }}


int main(int argc, char **argv) {
  Robot *robot = new Robot();
  Motor *Motor_right = robot->getMotor("Motor_1");
  Motor *Motor_left = robot->getMotor("Motor_2");
  
  Motor_right->setPosition(INFINITY);Motor_left->setPosition(INFINITY);
  Motor_right->setVelocity(0.0);Motor_left->setVelocity(0.0);  
  DistanceSensor *ps[12];
  char psNames[12][15] = {"ds_left3", "ds_left2", "ds_left1", "ds_middle", "ds_right1", "ds_right2", "ds_right3", "box_det", "ds_fright", "ds_fleft", "piller_sensor1" , "piller_sensor2"};
  for (int i = 0; i < 12; i++) {ps[i] = robot->getDistanceSensor(psNames[i]); ps[i]->enable(TIME_STEP);}
  PositionSensor *ds[4];
  char dsNames[4][12] = {"Encoder_1", "Encoder_2", "arm_ps", "slider1_pos"};
  for (int i = 0; i < 4; i++) {ds[i] = robot->getPositionSensor(dsNames[i]); ds[i]->enable(TIME_STEP);}

  int avoidObstacleCounter = 0;
  int stage = 0;
  int VALUE = 0;
  int value1 = 0;
  int value2 = 0;
  
  Camera*Cam1=robot->getCamera("camera_front");
  Cam1->enable(TIME_STEP);
  Camera*Cam2=robot->getCamera("camera_top");
  Cam2->enable(TIME_STEP);
  
  while (robot->step(TIME_STEP) != -1) { 
    
    int Count = 0;  
    for (int i = 0; i < 7; i++){
    double Values =  ps[i]->getValue();
    if (Values>50){Count = Count + 1;}}
    
    int Count2 = 0;  
    for (int i = 0; i < 7; i++){
    double Values =  ps[i]->getValue();
    if (Values<50){Count2 = Count2 + 1;}}
    
    double Distance = -1*(ds[0]->getValue()+ ds[0]->getValue())/2;
    



    
    

    if (Count == 0 ){wallfollowe(avoidObstacleCounter, Motor_right, Motor_left, ps);} 
    else if (Count2 == 0 && Distance < 170.0 && Distance > 150.0 && stage ==0 ){stage = linefollowe2(Motor_right, Motor_left, ps, ds, robot, Distance);}
    else if  (Distance < 250.0 && stage == 3){stage = linefollowe2_2(Motor_right, Motor_left,ds, robot, Distance, ps, stage);}
    else if (Distance > 200.0 && Count2 == 0 && stage != 6){stage = linefollowe3(Motor_right, Motor_left,ds, robot, Distance, ps, stage);}
    else if(Count2 == 0 && stage == 6){linefollowe4(Motor_right, Motor_left,ds, robot, Distance, ps, VALUE);}
    
    else{
    if (ps[7]->getValue()<=0.7){
    value1 = getColorFrontFace(robot,0);
    Boxcatch(Motor_right, Motor_left, robot, ds, ps);
    value2 = getColorBottomFace(robot,0);
    VALUE = abs(value1 - value2); 
    }
    else{linefollow(Motor_right, Motor_left, ps, 7);}
    }
    

    
   // linefollowe4(Motor_right, Motor_left,ds, robot, Distance, ps, VALUE);
    std::cout << "   Count =";
    std::cout << Count; 
    
    std::cout <<  "   Box dis =";   
    std::cout << ps[7]->getValue(); 
    
    std::cout <<  "   Right piller =";   
    double var = ps[10]->getValue();
    std::cout << var;
    std::cout <<  "   Left piller =";   
    double var1 = ps[11]->getValue();
    std::cout << var1;
    
    std::cout <<  "   Left IR =";   
    double var2 = ps[0]->getValue();
    std::cout << var2;
    std::cout <<  "   Right IR =";   
    double var3 = ps[6]->getValue();
    std::cout << var3;
      
    std::cout << "   VALUE =";
    std::cout << VALUE; 
    std::cout <<  "   Front value = "; 
    std::cout << value1;
    std::cout <<  "   Bottom value = "; 
    std::cout << value2;
    std::cout << "\n";
  
  }
  delete robot;
  return 0; //EXIT_SUCCESS
}