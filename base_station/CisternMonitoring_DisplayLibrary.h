/*
  CisternMonitoring_DisplayLibrary - Library for Display Control for Cistern Monitoring.
  Created by Florian Merz, 21.11.2020
  Just for internal purpose
*/
#ifndef CISTERNMONITORING_DISPLAYLIBRARY_H
#define CISTERNMONITORING_DISPLAYLIBRARY_H

#include <TFT_eSPI.h> // Hardware-specific library
//#include <SPI.h>
//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <SPI.h>

//new row


class CisternMonitoringDisplay
{
  public:
         CisternMonitoringDisplay( byte yStartPositionCistern,byte xStartPositionCistern,byte SpaceToBottomOfDisplay,byte borderWidth, String AP_Name_ConfigMode, String AP_PW_ConfigMode);
    void initializeDisplay();
    void DrawResetView();
    void DrawMainView(bool loraConnected, String OnlineMode);
    void updateWaterLevel(byte percentage, int litre, int measuredDistance,String ConnectionMode, bool OnlineMode);
    void setOffset(byte offset);
    void DrawConfigView();
    void DrawSavedConfigView();
    void DrawFailedOnlineModeConfigView();

  private:
    byte _yStartPositionCistern;
    byte _xStartPositionCistern;  // same like space to edge of display
    byte _SpaceToBottomOfDisplay;
    byte _borderWidth;
    byte _offset;
    String _AP_Name_ConfigMode;
    String _AP_PW_ConfigMode;
    TFT_eSPI _tft;
};

#endif
