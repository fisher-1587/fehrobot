#ifndef GLOBALS_H
#define GLOBALS_H

#define WHEEL_DIAMETER 2.5
#define WHEEL_SPACING 7
#define CLICKS_PER_ROTATION 318
#define PI 3.141592653589793238

#define CDS_START_TRIGGER 1
#define CDS_RED_BLUE_TRIGGER 0.35

#define SCREEN_FONT_COLOR 0xFFFFFF
#define SCREEN_BACKGROUND_COLOR 0x000000

#define TELEMETRY_TEXT_LENGTH 13

//Analog pin declerations
AnalogInputPin leftSensor(FEHIO::P0_2);	//CDS Sensor on the first block, 7th pin
AnalogInputPin middleSensor(FEHIO::P0_1);	//CDS Sensor on the first block, 7th pin
AnalogInputPin rightSensor(FEHIO::P0_0);	//CDS Sensor on the first block, 7th pin
AnalogInputPin cdsCell(FEHIO::P0_7);

//Digital pin declerations
DigitalInputPin leftFrontButton(FEHIO::P0_4);	//Touch Sensor on the second block, 3rd pin
DigitalInputPin rightFrontButton(FEHIO::P0_3);	//Touch Sensor on the second block, 4th pin
DigitalInputPin rightBackButton(FEHIO::P2_0);	//Touch Sensor on the second block, 4th pin
DigitalInputPin leftBackButton(FEHIO::P2_1);	//Touch Sensor on the second block, 4th pin

DigitalEncoder rightEncoder(FEHIO::P0_5);
DigitalEncoder leftEncoder(FEHIO::P0_6);

//Servo declerations
FEHServo servo(FEHServo::Servo0);	//Test servo on the 1st servo port

FEHMotor rightMotor(FEHMotor::Motor1, 12);	//Left motor on the 1st motor port
FEHMotor leftMotor(FEHMotor::Motor0, 12);	//Right motor on the 2nd motor port

char gLightColor = 'c';

#endif
