#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHBattery.h>

#include <globals.h>
#include <LCDSupplement.h>
#include <overhead.h>
#include <drive.h>

void readLightValues()	//Display light values
{
    LCD.Clear();
    float x, y;
    while(!LCD.Touch(&x, &y))
    {
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "CdS Cell", "Left Opto", "Mid Opto", "Right Opto"};
        float telemetryData[] = {Battery.Voltage(), cdsCell.Value(), leftSensor.Value(), middleSensor.Value(), rightSensor.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
}

void readButtonValues()
{
    LCD.Clear();    //Clear the LCD to initate the telemetry
    float x, y;
    while(!LCD.Touch(&x, &y))
    {
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Left Front", "Right Front", "Left Back", "Right Back"};
        float telemetryData[] = {Battery.Voltage(), leftFrontButton.Value(), rightFrontButton.Value(), leftBackButton.Value(), rightBackButton.Value()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
}

void readEncoderValues()
{
    LCD.Clear();    //Clear the LCD to initate the telemetry
    float x, y;
    while(!LCD.Touch(&x, &y))
    {
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "Left Enc", "Right Enc"};
        float telemetryData[] = {Battery.Voltage(), leftEncoder.Counts(), rightEncoder.Counts()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
}

int main(void)
{
    LCD.Clear();
    waitForConfirmation();
	driveTime(1000, 100);
    //waitForStart();
    //driveUntilLine(1.7, 1.7, 1.7, 15, true);
    //followUntilBumped(1.7, 1.7, 1.7, 20, true);
    //driveTime(1000, -15);
    //followUntilBumped(1.7, 1.7, 1.7, 15, true);
    //driveInches(-3, 25, 15);
    //turnToAngle(90, 30);
    //driveUntilCDS(10);

    //dispOptoValues();
    //driveInchesPowerRamp(12, 50, 15);
    //turnToAngle(90, 30);
    /*driveInches(30, 40, 30);
    turnToAngle(90, 20);
    smartDriveUntilPressed(-35);
    driveInches(9.25, 30, 15);
    turnToAngle(-90, 20);
    driveUntilPressed(30);
    driveInches(-4, 30, 15);
    turnToAngle(90, 25);
    smartDriveUntilPressed(-35);
    driveUntilLine(2, 40, -1);
    followLightUntilBumped(2, 2, 2, 30);
    driveTime(500, -15);
    followLightUntilBumped(2, 2, 2, 15);
    Sleep(1000);
    driveInches(-19, 80, 30);
    turnToAngle(45, 20);
    driveInches(9, 50, 15);*/
}

