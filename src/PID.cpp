#include "mbed.h"
#include "PID.h"
#include "constants.h"


PID::PID(
        float Kp, 
        float Ki, 
        float Kd, 
        float limMin, 
        float limMax, 
        float limMinInt, 
        float limMaxInt,
        float setposition
    ): 
        Kp(0), 
        Ki(0), 
        Kd(0), 
        limMin(0), 
        limMax(0), 
        limMinInt(0), 
        limMaxInt(0),
        integrator(0), 
        prevError(0), 
        differentiator(0), 
        prevposition(0), 
        output(0), 
        setposition(0) {}


void PID::set(   
        float Kp1, 
        float Ki1, 
        float Kd1, 
        float limMin1, 
        float limMax1, 
        float limMinInt1, 
        float limMaxInt1)
{
    Kp = Kp1;
    Ki = Ki1;
    Kd = Kd1;
    limMin = limMin1; 
    limMax = limMax1; 
    limMinInt = limMinInt1; 
    limMaxInt = limMaxInt1;
}


void PID::reset(void) 
{
    Kp = 0;
    Ki = 0;
    Kd = 0;
    integrator = 0;
    prevError = 0;
    differentiator = 0;
    prevposition = 0;
    output = 0;
    limMin = 0; 
    limMax = 0; 
    limMinInt= 0; 
    limMaxInt = 0; 
    setposition = 0;
}


float PID::getoutput(float setposition, float position) 
{
    float error = setposition - position;

    float proportional = Kp * error;

    integrator = integrator + error;  
    if (integrator > limMaxInt) {
        integrator = limMaxInt;
    } else if (integrator < limMinInt) {
        integrator = limMinInt;
    }

    differentiator = error - prevError;

    output = proportional + Ki*integrator + Kd*differentiator;

    if (output > limMax) {
        output = limMax;
    } else if (output < limMin) {
        output = limMin;
    }
    
    prevError = error;
    prevposition = position;
    
    return output;
}
