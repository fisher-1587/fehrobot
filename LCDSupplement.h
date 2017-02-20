#ifndef LCDSUPPLEMENT_H
#define LCDSUPPLEMENT_H

#include <string.h>
#include <stdio.h>

#define MAX_X 319   //Define the maximum display dimensions
#define MAX_Y 239

#define CHARACTER_WIDTH 12  //Define the character dimensions
#define CHARACTER_HEIGHT 17

int nGetCenteredXPos(char[], int, int);  //Find the x coordinate to print text to make it appear centered onscreen
int nGetCenteredYPos(int, int);  //Find the y coordinate to print text to make it appear centered onscreen

void writeCenteredTextLine(char[], int);    //Writes centered text at a given y coordinate
void writeCenteredTextLine(char[], int, int, int);  //Writes centered text between two x coordinates at a given y coordinate
void writeCenteredTextLine(char[], int, int, int, int); //Writes centered text between two x coordinates and between two y coordinates

void writeCenteredTextLine(int, int);    //Writes centered int at a given y coordinate
void writeCenteredTextLine(int, int, int, int);  //Writes centered int between two x coordinates at a given y coordinate
void writeCenteredTextLine(int, int, int, int, int); //Writes centered int between two x coordinates and between two y coordinates

void writeCenteredTextLine(int, int);    //Writes centered float at a given y coordinate
void writeCenteredTextLine(int, int, int, int);  //Writes centered float between two x coordinates at a given y coordinate
void writeCenteredTextLine(int, int, int, int, int); //Writes centered float between two x coordinates and between two y coordinates

void eraseTextLine(int, int, int);  //Erase the line of text at a given y position
void waitForTouchScreenRelease();   //Wait for the touchscreen to stop being pressed

void sprintfloat(char[], float);    //Converts a floating point to string for printing

int nGetCenteredXPos(char str[], int nMinX, int nMaxX)
{
    int nCharCount = strlen(str);
    return nMinX + (nMaxX - nMinX - (CHARACTER_WIDTH * nCharCount)) / 2;
}

int nGetCenteredYPos(int nMinY, int nMaxY)
{
    return nMinY + (nMaxY - nMinY - CHARACTER_HEIGHT) / 2;
}

void writeCenteredTextLine(char str[], int nCoordY)
{
    int nCoordX = nGetCenteredXPos(str, 0, MAX_X);  //Calculate the x coordinate
    LCD.WriteAt(str, nCoordX, nCoordY); //Display the centered string
}

void writeCenteredTextLine(char str[], int nMinX, int nMaxX, int nCoordY)
{
    int nCoordX = nGetCenteredXPos(str, nMinX, nMaxX);  //Calculate the x coordinate
    LCD.WriteAt(str, nCoordX,nCoordY);  //Display the centered string
}

void writeCenteredTextLine(char str[], int nMinX, int nMaxX, int nMinY, int nMaxY)
{
    int nCoordX = nGetCenteredXPos(str, nMinX, nMaxX);  //Calculate the x coordinate
    int nCoordY = nGetCenteredYPos(nMinY, nMaxY);   //Calculate the y coordinate
    LCD.WriteAt(str, nCoordX,  nCoordY);    //Display the centered string
}

void writeCenteredTextLine(int num, int nCoordY)
{
    char str[20];
    sprintf(str, "%d", num);
    writeCenteredTextLine(str, nCoordY);
}

void writeCenteredTextLine(int num, int nMinX, int nMaxX, int nCoordY)
{
    char str[20];
    sprintf(str, "%d", num);
    writeCenteredTextLine(str, nMinX, nMaxX, nCoordY);
}

void writeCenteredTextLine(int num, int nMinX, int nMaxX, int nMinY, int nMaxY)
{
    char str[20];
    sprintf(str, "%d", num);
    writeCenteredTextLine(str, nMinX, nMaxX, nMinY, nMaxY);
}

void writeCenteredTextLine(float num, int nCoordY)
{
    char str[20];
    sprintfloat(str, num);
    writeCenteredTextLine(str, nCoordY);
}

void writeCenteredTextLine(float num, int nMinX, int nMaxX, int nCoordY)
{
    char str[20];
    sprintfloat(str, num);
    writeCenteredTextLine(str, nMinX, nMaxX, nCoordY);
}

void writeCenteredTextLine(float num, int nMinX, int nMaxX, int nMinY, int nMaxY)
{
    char str[20];
    sprintfloat(str, num);
    writeCenteredTextLine(str, nMinX, nMaxX, nMinY, nMaxY);
}

void eraseTextLine(int nCoordY, int nEraserColor, int nCurrentColor)
{
    LCD.SetFontColor(nEraserColor); //Set the color to be the same as the background
    LCD.FillRectangle(0, nCoordY, MAX_X, CHARACTER_HEIGHT + nCoordY);  //Draw a rectangle over the text
    LCD.SetFontColor(nCurrentColor);    //Set the color back to the default
}

void waitForTouchScreenRelease()    //Loops while the touch screen is pressed
{
    float fCoordX, fCoordY;
    while(LCD.Touch(&fCoordX, &fCoordY)){}
}

void sprintfloat(char str[], float fValue)
{
    int nBeforeDecimal = fValue;  //Holds the number before the decimal point (note conversion from float to int)
    float fRemainder = fValue - nBeforeDecimal; //Get the part of the number after the decimal point
    int nAfterDecimal = 1000 * fRemainder;  //Multiply by 1000 to find the first 3 digits after the decimal point and store to an int
    char sFormatText[25] = "%d.%d"; //Formatting for putting the the two values together surrounding the decimal point
    sprintf(str, sFormatText, nBeforeDecimal, nAfterDecimal);   //Create a string from the formatted floating point value
}

#endif
