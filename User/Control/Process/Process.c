#include "Process.h"

uint8_t pass = 0;
uint8_t aimShape = 0;

float lastMotorMove = 0;
float lastServoAngel = 0;
float nowMotorMove = 0;
float nowServoAngel = 0;
float kd = 0.1; 

void testProcess(void)
{   
    uint8_t aimcolor = (strcmp(aimColor, "_red") == 0) ?1:0;
    usart3Print("convery", 7);
    WAIT (OpennMvData.distanse != 0);
    beepBeep(1);
    aimShape = OpennMvData.shape;
    clawStateChange(OPEN);
    waitS(motor4);
    HAL_Delay(1000); //调试用句
    motorMove(&motor2, 6400, 0.3, 0.3, 6400);
    waitS(motor2);
    clawStateChange(CIRCLE);
    waitS(motor4);
    servoAction(SPIN, 1100, 5000);
    motorMove(&motor2, 6400, 0.3, 0.3, -6400);
    waitS(motor2);
    HAL_Delay(4000);
    beepBeep(1);
    usart3Print("disk", 4);
    WAIT(OpennMvData.color == aimcolor && OpennMvData.shape == aimShape);
    HAL_Delay(4000);
    lastMotorMove = nowMotorMove = controlData.delteDistanse * DISTOSTEP;
    lastServoAngel = nowServoAngel = (controlData.deltaTheta * 180.0f / 3.14f)*ANGELTOPWM;
    for(uint8_t i = 0; i < 7; i++){
        getAnD(&controlData,OpennMvData.distanse,OpennMvData.cx,OpennMvData.cy);
        nowMotorMove = controlData.delteDistanse * DISTOSTEP;
        nowServoAngel = (controlData.deltaTheta * 180.0f / 3.14f)*ANGELTOPWM;
        if (controlData.delteDistanse && controlData.deltaTheta){
            servoAction(SPIN, (int)(1100 - nowServoAngel + kd * (nowServoAngel - lastServoAngel)), 2000);
            motorMoveFast(&motor1, 1600, nowMotorMove + kd * (nowMotorMove - lastMotorMove));
            HAL_Delay (2000);
            waitS(motor1);
            beepBeep(1);
        }
    }/*迭代七次，让爪子运动至槽的上方*/
    
    motorMove(&motor2, 6400, 1, 1, 16000);
    waitS(motor2);
    clawStateChange(OPEN);
    waitS(motor4);
    motorMove(&motor2, 6400, 1, 1, -16000);
    waitS(motor2);
    
    beepBeep(1);
//    servoAction(ARM, 2150, 5000);  舵机运动，控制一二号舵机运动
//    clawStateChange(OPEN);  爪子抓放
//    motorMove(&motor1, 9600, 2, 2, 25500);  电机移动
//    waitS(motor1);  等待电机移动结束
//    motorMove(&motor2, 12800, 1.5, 1.5, 27200);
//    waitS(motor2);
//    clawStateChange(CIRCLE);
//    waitS(motor5);
//    motorMove(&motor2, 12800, 1.5, 2.5, -27200);
//    waitS(motor2);
//    servoAction(SPIN, 1830, 5000);
//    HAL_Delay(5000);
//    motorMove(&motor1, 9600, 2, 2, -25500);
//    waitS(motor1)
//    motorMove(&motor2, 9600, 1.5, 1.5, 17600);
//    waitS(motor2);
//    HAL_Delay(2000);
//    motorMove(&motor2, 3200, 0.5, 0.5, 9600);
//    waitS(motor2);
//    clawStateChange(OPEN);
//    waitS(motor5);
//    motorMove(&motor2, 12800, 1.5, 2.5, -27200);
//    clawStateChange(CLOSE);
//    waitS(motor2);
//    servoAction(SPIN, 580, 5000);
//    HAL_Delay(5000);
//    
//    
//    servoAction(ARM, 1200, 5000);
//    HAL_Delay(5000);
}

void resetProcess(void)
{
    motorMove2Pos(&motor2, 3200, 0.5, 0.5, 0);
    clawStateChange(CLOSE);
}

void testDuiZhun(void)
{
    servoAction(SPIN, 1100, 5000);
    usart3Print("disk", 4);
    HAL_Delay(1000);
    for(uint8_t i = 0; i < 7; i++){
        getAnD(&controlData,OpennMvData.distanse,OpennMvData.cx,OpennMvData.cy);
        if (controlData.delteDistanse && controlData.deltaTheta){
            servoAction(SPIN, (int)(1100 - (controlData.deltaTheta * 180.0f / 3.14f)*ANGELTOPWM), 2000);
            motorMoveFast(&motor1, 1600, controlData.delteDistanse * DISTOSTEP);
            HAL_Delay (2000);
            waitS(motor1);
            beepBeep(1);
        }
    }/*迭代七次，让爪子运动至槽的上方*/
    
    
    
    
}
