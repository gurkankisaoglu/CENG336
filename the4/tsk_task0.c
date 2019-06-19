#include "common.h"
#include "LCD.h"

// We simply used the algorithm which is given to us with a little changing in order.

/*
Receive sensor response
Update map
Receive encoder response
Update your current position and rotation
if you have explored all the cells
	then END
else 
	followRightBorder
	

followRightBorder:
	if there are no obstacles on your right 
		then TURN RIGHT and MOVE FORWARD
	else if there are no obstacles in front of you 
		then MOVE FORWARD
	else
		TURN LEFT
*/ 

char way[4][4] = {"$R:", "$F:", "$L:", "$S:"};
char update[7] = "$MXYS:";
char endCommand[5] = "$END:";
unsigned int angle = 180;
unsigned int temp_angle = 0;
unsigned int x = 25;
unsigned int y = 25;
unsigned int notFinished = 1;
char response[5];
unsigned int obstacle[4] = {1,1,1,1};
unsigned int dir = 0; //front = 0; right = 1; rear = 2; left = 3
unsigned char mapData[4][16];
unsigned char wholeMap[4][16];
unsigned int summation_map = 2;
unsigned char flag = 0, tmpC;
unsigned int hebleee;
unsigned int size;




TASK(TASK0) 
{    
    
    PIE1bits.RCIE = 1;
    //at the beginning, these two location is already known that
    //they have no obstacle
    
    mapData[0][0] = 1;
    mapData[0][15] = 1;

	SetRelAlarm(ALARM_TSK0, 100, 50);

    //to handle GO command
    while(1){
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);        
        receiveBuffer_pop(response);
        if(response[0]=='G'){
            break;
        }
    }
    
    //when all 64 location is known (obstacle or no obstacle),
    //this loop will end. Otherwise, the robot would have gone
    //to the beginning of the map for no purpose.
    while(summation_map < 64) {
        transmitBuffer_push(way[3],4);
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);        
        size=receiveBuffer_pop(response);
        
        //it assigns the obstacle info to an array which will be used in later
        for(hebleee = 0; hebleee<4; hebleee=hebleee+1){
            obstacle[hebleee] = response[hebleee+1];
        }
        
        
        //this long conditional branch is for updating the map in simulator and
        //hold the information of how many location is known (summation_map).
        
        //dir variable determines the relative way of the robot.
        //we listed all cases and we found a similarity between right and left,
        //and between front and rear. therefore we used conditional branch
        if(dir == 1 || dir == 3){
            if(((y+25)/50)-1 != 0){
                update[3] = ((x+25)/50)-1;  
                update[2] = ((y+25)/50)-2;
                update[4] = (int)obstacle[dir];
                transmitBuffer_push(update,7);
                //mapData element is 0 when it is not known,
                //1 when it has no obstacle, and 2 when it has an obstacle
                if(mapData[((y+25)/50)-2][((x+25)/50)-1] == 0){
                    if((int)obstacle[dir]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)-2][((x+25)/50)-1] = tmpC;
                    summation_map = summation_map + 1;
                }
                
            }
            if(((x+25)/50)-1 != 15){
                update[3] = ((x+25)/50);
                update[2] = ((y+25)/50)-1;
                update[4] = (int)obstacle[(dir+1)%4];
                transmitBuffer_push(update,7);    
              
                if(mapData[((y+25)/50)-1][((x+25)/50)] == 0){
                    if((int)obstacle[(dir+1)%4]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)-1][((x+25)/50)] = tmpC;
                    summation_map = summation_map + 1;
                }

            }
            if(((y+25)/50)-1 != 3){
                update[3] = ((x+25)/50)-1;
                update[2] = ((y+25)/50);
                update[4] = (int)obstacle[(dir+2)%4];
                transmitBuffer_push(update,7);  
               
                if(mapData[((y+25)/50)][((x+25)/50)-1] == 0){
                    if((int)obstacle[(dir+2)%4]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)][((x+25)/50)-1] = tmpC;
                    summation_map = summation_map + 1;
                }
               
            }
            if(((x+25)/50)-1 != 0){
                update[3] = ((x+25)/50)-2;
                update[2] = ((y+25)/50)-1;
                update[4] = (int)obstacle[(dir+3)%4];
                transmitBuffer_push(update,7);  
               
                if(mapData[((y+25)/50)-1][((x+25)/50)-2] == 0){
                    if((int)obstacle[(dir+3)%4]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)-1][((x+25)/50)-2] = tmpC;
                    summation_map = summation_map + 1;
                }

            }        
        }
        else{
            if(((y+25)/50)-1 != 0){
                update[3] = ((x+25)/50)-1;  
                update[2] = ((y+25)/50)-2;
                update[4] = (int)obstacle[(dir+2)%4];
                transmitBuffer_push(update,7);
                
                if(mapData[((y+25)/50)-2][((x+25)/50)-1] == 0){
                    if((int)obstacle[(dir+2)%4]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)-2][((x+25)/50)-1] = tmpC;
                    summation_map = summation_map + 1;
                }

            }
            if(((x+25)/50)-1 != 15){
                update[3] = ((x+25)/50);
                update[2] = ((y+25)/50)-1;
                update[4] = (int)obstacle[(dir+3)%4];
                transmitBuffer_push(update,7);
               
                if(mapData[((y+25)/50)-1][((x+25)/50)] == 0){
                    if((int)obstacle[(dir+3)%4]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)-1][((x+25)/50)] = tmpC;
                    summation_map = summation_map + 1;
                }

            }
            if(((y+25)/50)-1 != 3){
                update[3] = ((x+25)/50)-1;
                update[2] = ((y+25)/50);
                update[4] = (int)obstacle[dir];
                transmitBuffer_push(update,7);
              
                if(mapData[((y+25)/50)][((x+25)/50)-1] == 0){
                    if((int)obstacle[dir]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)][((x+25)/50)-1] = tmpC;
                    summation_map = summation_map + 1;
                }
          
            }
            if(((x+25)/50)-1 != 0){
                update[3] = ((x+25)/50)-2;
                update[2] = ((y+25)/50)-1;
                update[4] = (int)obstacle[(dir+1)%4];
                transmitBuffer_push(update,7);
               
                if(mapData[((y+25)/50)-1][((x+25)/50)-2] == 0){
                    if((int)obstacle[(dir+1)%4]){
                        tmpC = 2;
                    }else{
                        tmpC = 1;
                    }
                    mapData[((y+25)/50)-1][((x+25)/50)-2] = tmpC;
                    summation_map = summation_map + 1;
                }
                
            }                 
        }
        
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);        
        
        //this part of the code is for traversing. we used the algorithm which
        //is given to us.
        
        if(obstacle[1] == 0 || obstacle[0] == 0){ //right, forward is available
            if(obstacle[1] == 0){
                while(flag == 0 || angle % 90 != 0 ){
                    flag = 1;
                    transmitBuffer_push(way[0],4);
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);         
                    size=receiveBuffer_pop(response);
                    if(angle == 0){
                        angle += 360;
                    }                    
                    angle = angle - response[1];

                }
                dir++;
                if(dir > 3){
                    dir = 0;
                }            
                flag = 0;
            }
        
            while(x%50 != 25 || y%50 != 25 || flag == 0){
                flag = 1;
                transmitBuffer_push(way[1],4);
                WaitEvent(ALARM_EVENT);
                ClearEvent(ALARM_EVENT);                
                size=receiveBuffer_pop(response);
                if(angle==90){ //right
                    x -= response[1];
                }
                else if(angle==270){ //left
                    x += response[1];
                }
                else if(angle==180){ //down
                    y += response[1];
                }
                else{ //up
                    y -= response[1];
                }
            }
            flag = 0;
        }
        else{
            while(angle % 90 != 0 || flag == 0){
                flag = 1;
                transmitBuffer_push(way[2],4);
                WaitEvent(ALARM_EVENT);
                ClearEvent(ALARM_EVENT);                
                size=receiveBuffer_pop(response);
                angle += response[1];
                if(angle > 360){
                    angle -= 360;
                }
            }

            if(dir == 0){
                dir = 4;
            }            
            dir--;
            flag = 0;
        }           
	}
    
    //this while loop is for saving the remaining time as much as possible.
    //we changed the algorithm for that purpose and the robot turns to left
    //instead of right in this part. the remaining part of the algorithm is
    //the same.
    while(((y+25)/50)-1 != 0 || ((x+25)/50)-1 != 15){
        transmitBuffer_push(way[3],4);
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);        
        size=receiveBuffer_pop(response);
        for(hebleee = 0; hebleee<4; hebleee=hebleee+1){
            obstacle[hebleee] = response[hebleee+1];
        }
    
        if(obstacle[3] == 0 || obstacle[0] == 0){ //left, forward is available
            if(obstacle[3] == 0){

                
                while(angle % 90 != 0 || flag == 0){
                    flag = 1;
                    transmitBuffer_push(way[2],4);
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);                
                    size=receiveBuffer_pop(response);
                    angle += response[1];
                    if(angle > 360){
                        angle -= 360;
                    }
                }
                flag = 0; 
            }
        
            while(x%50 != 25 || y%50 != 25 || flag == 0){
                flag = 1;
                transmitBuffer_push(way[1],4);
                WaitEvent(ALARM_EVENT);
                ClearEvent(ALARM_EVENT);                
                size=receiveBuffer_pop(response);
                if(angle==90){ //right
                    x -= response[1];
                }
                else if(angle==270){ //left
                    x += response[1];
                }
                else if(angle==180){ //down
                    y += response[1];
                }
                else{ //up
                    y -= response[1];
                }
            }
            flag = 0;
        }
        else{
            while(flag == 0 || angle % 90 != 0 ){
                flag = 1;
                transmitBuffer_push(way[0],4);
                WaitEvent(ALARM_EVENT);
                ClearEvent(ALARM_EVENT);         
                size=receiveBuffer_pop(response);
                if(angle == 0){
                    angle += 360;
                }                    
                angle = angle - response[1];

            }       
            flag = 0;
        }    
        
    }
    WaitEvent(ALARM_EVENT);
    ClearEvent(ALARM_EVENT);
    //when the task finishes, end command is sent.
    transmitBuffer_push(endCommand,6);
    TerminateTask();
}


/* End of File : tsk_task0.c */