#ifndef OVERHEAD_H
#define OVERHEAD_H

void displayTelemetry(char[][TELEMETRY_TEXT_LENGTH], float[], int);    //Displays telemetry with a title and value at every available screen line
void updateRunState(int nProgramState); //Updates the top line of the display with the function the program is running
void waitForConfirmation(); //Waits for robot set up role to press the touchscreen to confirm it is set up
void waitForStart();    //Waits for go signal from the RPS

void updateRunState(int nProgramState)  //Rewrite the first line of the display to show the program state
{
    eraseTextLine(0, 0x000000, 0xffffff);
    switch(nProgramState)
    {
        case 0:
            writeCenteredTextLine("Waiting for setup", 0);
        break;

        case 1:
            writeCenteredTextLine("Waiting for start", 0);
        break;

        case 2:
            writeCenteredTextLine("Running", 0);
        break;

        case 3:
            writeCenteredTextLine("Stopped", 0);
        break;

    }
}

void displayTelemetry(char telemTitles[][TELEMETRY_TEXT_LENGTH], float telemData[], int numData)   //Write up to 10 lines of telemetry to the screen
{
    for(int i = 0; i < numData; i++)
    {
        writeCenteredTextLine(telemTitles[i], 0, 160, (i+1) * CHARACTER_HEIGHT);
        writeCenteredTextLine(telemData[i], 160, 320, (i+1) * CHARACTER_HEIGHT);
    }
}

void waitForConfirmation() //Waits for the confirmation from the runner
{
    LCD.Clear();
    updateRunState(0);
    float x, y;
    while(!LCD.Touch(&x, &y))
    {
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery"};
        float telemetryData[] = {Battery.Voltage()};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
    while(LCD.Touch(&x, &y)){};
}

void waitForStart() //Waits for the start from the RPS
{
    LCD.Clear();
    //updateRunState(1);
    while(cdsCell.Value() > CDS_START_TRIGGER)
    {
        char telemetryTitle[][TELEMETRY_TEXT_LENGTH] = {"Battery", "CdS Cell", "Trigger Val"};
        float telemetryData[] = {Battery.Voltage(), cdsCell.Value(), CDS_START_TRIGGER};
        displayTelemetry(telemetryTitle, telemetryData, sizeof(telemetryData)/sizeof(telemetryData[0]));
    }
}

#endif
