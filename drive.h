#ifndef DRIVE_H
#define DRIVE_H

#include <math.h>

void setMotorSpeed(float fSpeed);	//Sets speed for both motors from the parameter
void setMotorSpeed(float fLeftSpeed, float fRightSpeed);	//Sets speed for each motor based on the two parameters
float rampSpeed(float fCurrentClicks, float fTargetClicks, float fMaxSpeed, float fMinSpeed);  //Calculates the power ramping for the motors

void driveUntilPressed(float fSpeed);
void followUntilBumped(float fLeftVoltageThreshold, float fMiddleVoltageThreshold, float fRightVoltageThreshold, float fSpeed, bool bLineColor);
void driveUntilLine(float fLeftVoltageThreshold, float fMiddleVoltageThreshold, float fRightVoltageThreshold, float fSpeed, bool lineColor);    //Drive until a line is seen
void driveUntilCDS(float fSpeed);   //Drives until any sensor sees the line

void driveTime(int nTime, float fSpeed); //Drive a specified number of milliseconds
void driveClicks(float fInches, float fMaxSpeed, float fMinSpeed = 0);  //Drive a specified number of inches
void driveInches(float fInches, float fMaxSpeed, float fMinSpeed = 0);  //Drive a specified number of inches
void turnToAngle(float fAngle, float fMaxSpeed, float fMinSpeed = 0); //Turns to a specified angle


void setMotorSpeed(float fSpeed) //Set both motor speeds
{
    leftMotor.SetPercent(fSpeed);
    rightMotor.SetPercent(fSpeed);
}

void setMotorSpeed(float fLeftSpeed, float fRightSpeed)   //Sets the drive motor speeds per motor
{
    leftMotor.SetPercent(fLeftSpeed);
    rightMotor.SetPercent(fRightSpeed);
}

float rampSpeed(float fCurrentClicks, float fTargetClicks, float fMaxSpeed, float fMinSpeed)    //Return a speed calculated based on the distance traveled by the robot
{
    float fCalculatedSpeed = sin(PI * fabs(fCurrentClicks) / fabs(fTargetClicks)) * fMaxSpeed;   //Calculate the speed based on the percent completion of the drive
    if(abs(fCalculatedSpeed) < abs(fMinSpeed)) //Do not let the calculated speed fall below the minimum speed
        fCalculatedSpeed = fMinSpeed;
    return fCalculatedSpeed;
}

void driveUntilPressed(float fSpeed)
{
    LCD.Clear();    //Clear the screen to prepare for incoming telemetry
    if(fSpeed < 0)	//If the robot is going backwards, make sure to only take input from the back buttons
    {
        while(rightBackButton.Value() || leftBackButton.Value())
        {
            setMotorSpeed(fSpeed);
        }
    }
    else if(fSpeed > 0)	//If the robot is going forwards, make sure to only take input from the front buttons
    {
        while(rightFrontButton.Value() || leftFrontButton.Value())	//Wait for both buttons to be pressed to break the loop
        {
            setMotorSpeed(fSpeed);	//Set the speed
        }
    }
    setMotorSpeed(0);	//Stop the motors
}

void smartDriveUntilPressed(float fSpeed)
{
    LCD.Clear();    //Clear the screen to prepare for incoming telemetry
    float fLeftSpeed;
    float fRightSpeed;
    if(fSpeed < 0)	//If the robot is going backwards, make sure to only take input from the back buttons
    {
        while(rightBackButton.Value() || leftBackButton.Value())
        {
            fLeftSpeed = fSpeed * leftBackButton.Value();
            fRightSpeed = fSpeed * rightBackButton.Value();
            setMotorSpeed(fLeftSpeed, fRightSpeed);

            char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Left Motor", "Right Motor", "Left Front", "Right Front"};
            float telemetryData[] = {Battery.Voltage(), fLeftSpeed, fRightSpeed, leftBackButton.Value(), rightBackButton.Value()};
            displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
        }
    }
    else if(fSpeed > 0)	//If the robot is going forwards, make sure to only take input from the front buttons
    {
        while(rightFrontButton.Value() || leftFrontButton.Value())	//Wait for both buttons to be pressed to break the loop
        {
            fLeftSpeed = fSpeed * leftFrontButton.Value();
            fRightSpeed = fSpeed * rightFrontButton.Value();
            setMotorSpeed(fLeftSpeed, fRightSpeed);

            char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Left Motor", "Right Motor", "Left Front", "Right Front"};
            float telemetryData[] = {Battery.Voltage(), fLeftSpeed, fRightSpeed, leftFrontButton.Value(), rightFrontButton.Value()};
            displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
        }
    }
    setMotorSpeed(0);	//Stop the motors
}

void followUntilBumped(float fLeftVoltageThreshold, float fMiddleVoltageThreshold, float fRightVoltageThreshold, float fSpeed, bool lineColor)
{
    LCD.Clear();
    bool bLeftSensorTriggered, bMidSensorTriggered, bRightSensorTriggered;
    float fLeftSpeed, fRightSpeed;
    while(leftFrontButton.Value() || rightFrontButton.Value())
    {
        if(lineColor)
        {
            bLeftSensorTriggered = (leftSensor.Value() > fLeftVoltageThreshold);
            bMidSensorTriggered = (middleSensor.Value() > fMiddleVoltageThreshold);
            bRightSensorTriggered = (rightSensor.Value() > fRightVoltageThreshold);
        }
        else
        {
            bLeftSensorTriggered = (leftSensor.Value() < fLeftVoltageThreshold);
            bMidSensorTriggered = (middleSensor.Value() < fMiddleVoltageThreshold);
            bRightSensorTriggered = (rightSensor.Value() < fRightVoltageThreshold);
        }

        if(!bLeftSensorTriggered && bMidSensorTriggered && !bRightSensorTriggered)  //Middle sensor sees the line, drive straight
        {
            fLeftSpeed = fSpeed;
            fRightSpeed = fSpeed;
        }
        else if(bLeftSensorTriggered && bMidSensorTriggered && !bRightSensorTriggered) //Both the left and middle sensor see the line, turn gently left
        {
            fLeftSpeed = fSpeed / 2;
            fRightSpeed = fSpeed;
        }
        else if(bLeftSensorTriggered && !bMidSensorTriggered && !bRightSensorTriggered) //The left sensor sees the line, turn aggressively left
        {
            fLeftSpeed = 0;
            fRightSpeed = fSpeed;
        }
        else if(!bLeftSensorTriggered && bMidSensorTriggered && bRightSensorTriggered) //Both the right and middle sensor see the line, turn gently right
        {
            fLeftSpeed = fSpeed;
            fRightSpeed = fSpeed / 2;
        }
        else if(!bLeftSensorTriggered && !bMidSensorTriggered && bRightSensorTriggered) //The right sensor sees the line, turn aggressively right
        {
            fLeftSpeed = fSpeed;
            fRightSpeed = 0;
        }
        else
        {
            fLeftSpeed = fSpeed / 2;
            fRightSpeed = fSpeed / 2;
        }
        setMotorSpeed(fLeftSpeed, fRightSpeed);
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Left Opto", "Mid Opto", "Right Opto", "Left Motor", "Right Motor", "Left Thresh", "Mid Thresh", "Right Thresh" , "Left Button", "Right Button"};
        float telemetryData[] = {Battery.Voltage(), leftSensor.Value(), middleSensor.Value(), rightSensor.Value(), fLeftSpeed, fRightSpeed, fLeftVoltageThreshold, fMiddleVoltageThreshold, fRightVoltageThreshold, leftFrontButton.Value(), rightFrontButton.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    setMotorSpeed(0);   //Stop the motors
}

void singleSensorFollow(float fVoltageThreshold, float fSpeed)
{
    LCD.Clear();
    float fLeftSpeed, fRightSpeed;
    while(leftFrontButton.Value() && rightFrontButton.Value())
    {
        if(middleSensor.Value() < fVoltageThreshold)
        {
            fLeftSpeed = 0;
            fRightSpeed = fSpeed;
        }
        else
        {
            fLeftSpeed = fSpeed;
            fRightSpeed = 0;
        }
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Optosensor", "Thresh" , "Left Speed", "Right Speed","Left Button", "Right Button"};
        float telemetryData[] = {Battery.Voltage(), middleSensor.Value(), fVoltageThreshold, fLeftSpeed, fRightSpeed, leftFrontButton.Value(), rightFrontButton.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
        setMotorSpeed(fLeftSpeed, fRightSpeed);
    }
    setMotorSpeed(0);
}

void driveUntilLine(float fLeftVoltageThreshold, float fMiddleVoltageThreshold, float fRightVoltageThreshold, float fSpeed, bool lineColor)   //Drives until any sensor sees the line
{
    LCD.Clear();    //Clear LCD for telemetry data
    bool bLeftSensorTriggered = false, bMidSensorTriggered = false, bRightSensorTriggered = false;  //Containers determine whether each sensor has been triggered
    while(!bLeftSensorTriggered && !bMidSensorTriggered && !bRightSensorTriggered)
    {
        if(lineColor)
        {
            bLeftSensorTriggered = (leftSensor.Value() > fLeftVoltageThreshold);
            bMidSensorTriggered = (middleSensor.Value() > fMiddleVoltageThreshold);
            bRightSensorTriggered = (rightSensor.Value() > fRightVoltageThreshold);
        }
        else
        {
            bLeftSensorTriggered = (leftSensor.Value() < fLeftVoltageThreshold);
            bMidSensorTriggered = (middleSensor.Value() < fMiddleVoltageThreshold);
            bRightSensorTriggered = (rightSensor.Value() < fRightVoltageThreshold);
        }

        setMotorSpeed(fSpeed);

        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Left Opto", "Mid Opto", "Right Opto", "Motor", "Left Thresh", "Mid Thresh", "Right Thresh" , "Left Button", "Right Button"};
        float telemetryData[] = {Battery.Voltage(), leftSensor.Value(), middleSensor.Value(), rightSensor.Value(), fSpeed, fLeftVoltageThreshold, fMiddleVoltageThreshold, fRightVoltageThreshold, leftFrontButton.Value(), rightFrontButton.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    setMotorSpeed(0);
}

void driveUntilCDS(float fSpeed)   //Drives straight searching with the cds cell
{
    LCD.Clear();
    bool bCDSTriggered = false;
    float minCDSValue = 3.3;    //Holds minimum voltage detected by the sensor
    while(!bCDSTriggered)   //Wait for the CDS cell to be triggered
    {
        bCDSTriggered = (cdsCell.Value() < CDS_START_TRIGGER);  //Update the flag
        setMotorSpeed(fSpeed);  //Set the motor speed as defined by the function caller

        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "CDS Volt"}; //Push telemetry data
        float telemetryData[] = {Battery.Voltage(), cdsCell.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    while(bCDSTriggered)    //Continue driving while the CDS is over the light
    {
        bCDSTriggered = (cdsCell.Value() < CDS_START_TRIGGER);
        if(cdsCell.Value() < minCDSValue)   //If a value detected by the sensor is the lowest seen so far, record it
            minCDSValue = cdsCell.Value();
        setMotorSpeed(fSpeed);  //Set the motor speed as defined by the function caller

        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "CDS Volt"}; //Push telemetry data
        float telemetryData[] = {Battery.Voltage(), cdsCell.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    setMotorSpeed(0);   //Stop the motors

    if(minCDSValue < CDS_RED_BLUE_TRIGGER)  //Determine whether the CDS sensor detected a blue or red light, updating the global color flag to match
    {
        gLightColor = 'r';
    }
    else
    {
        gLightColor = 'b';
    }
}

void driveClicks(float fClicks, float fMaxSpeed, float fMinSpeed)   //Drive specified number of encoder ticks
{
    LCD.Clear();    //Clear screen for telemetry data
    rightEncoder.ResetCounts();
    fMaxSpeed *= (fClicks / fabs(fClicks));
    fMinSpeed *= (fClicks / fabs(fClicks));
    float fCurrentSpeed;
    while(fabs(rightEncoder.Counts()) < fabs(fClicks))
    {
        if(fMinSpeed == 0)
            fCurrentSpeed = fMaxSpeed;
        else
            fCurrentSpeed = rampSpeed(rightEncoder.Counts(), fClicks, fMaxSpeed, fMinSpeed);
        setMotorSpeed(fCurrentSpeed, fCurrentSpeed);
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Clicks", "Total Clicks", "Speed", "Max Speed", "Min Speed"};
        float telemetryData[] = {Battery.Voltage(), rightEncoder.Counts(), fClicks, fCurrentSpeed, fMaxSpeed, fMinSpeed};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    setMotorSpeed(0);
}

void driveInches(float fDistance, float fMaxSpeed, float fMinSpeed)
{
    float fClicks = CLICKS_PER_ROTATION * fDistance / (WHEEL_DIAMETER * PI);
    driveClicks(fClicks, fMaxSpeed, fMinSpeed);
}

void driveTime(int nTime, float fSpeed)
{
    LCD.Clear();
    setMotorSpeed(fSpeed);   //Start the motors
    Sleep(nTime);    //Wait a specified amount of time
    setMotorSpeed(0);   //Stop the motors
}

void turnToAngle(float fAngle, float fMaxSpeed, float fMinSpeed)
{
    LCD.Clear();
    rightEncoder.ResetCounts(); //Reset the encoder
    float fInches = fAngle / 360 * PI * WHEEL_SPACING;  //Calculate the distance the wheels need to travel to get to the correct angle
    float fClicks = CLICKS_PER_ROTATION * fInches / (WHEEL_DIAMETER * PI); //Calculate the number of clicks the motor needs to rotate through
    float fCurrentSpeed;
    fMaxSpeed *= (fAngle / fabs(fAngle));
    while(fabs(rightEncoder.Counts()) < fabs(fClicks))
    {
        if(fMinSpeed == 0)
            fCurrentSpeed = fMaxSpeed;
        else
            fCurrentSpeed = rampSpeed(rightEncoder.Counts(), fClicks, fMaxSpeed, fMinSpeed);
        setMotorSpeed(-fCurrentSpeed, fCurrentSpeed);

        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Current Dist", "Total Dist", "Speed", "Max Speed", "Min Speed"};    //Push telemetry data to the screen
        float telemetryData[] = {Battery.Voltage(), rightEncoder.Counts(), fClicks, fCurrentSpeed, fMaxSpeed, fMinSpeed};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    setMotorSpeed(0);
}

#endif
