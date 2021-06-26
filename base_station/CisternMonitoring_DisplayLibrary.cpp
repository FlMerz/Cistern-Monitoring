#include "CisternMonitoring_DisplayLibrary.h"
#include "infoIcon.h"
#include "WifiIcon.h"
#include "WifiIconDisabled.h"
#include "LoraIcon.h"
#include "LoraIconDisabled.h"
#include "DistanceIcon.h"
#include "OffsetIcon.h"

//Here you can define the size of the tank
byte CisternBorderHeight;
byte CisternContentHeight;
byte CisternContentWidth;
byte CisternBorderWidth;
//Constructor
CisternMonitoringDisplay::CisternMonitoringDisplay(byte yStartPositionCistern,byte xStartPositionCistern,byte SpaceToBottomOfDisplay,byte borderWidth,  String AP_Name_ConfigMode, String AP_PW_ConfigMode)
{
   _yStartPositionCistern = yStartPositionCistern;
    _xStartPositionCistern = xStartPositionCistern;
    _SpaceToBottomOfDisplay = SpaceToBottomOfDisplay;
     _borderWidth = borderWidth;
     _AP_PW_ConfigMode = AP_PW_ConfigMode;
     _AP_Name_ConfigMode=AP_Name_ConfigMode;

     //Calculate height, width,...
    // Cistern Border:
   CisternBorderHeight=  _tft.height()-(_SpaceToBottomOfDisplay + _yStartPositionCistern);
   CisternBorderWidth = _tft.width()-(2 * _xStartPositionCistern);
   //Cistern Content
   CisternContentHeight = CisternBorderHeight - _borderWidth;
   CisternContentWidth = CisternBorderWidth - 2 * _borderWidth;
}





void CisternMonitoringDisplay::initializeDisplay(){
      // Use this initializer if using a 1.8" TFT screen:
 Serial.println("Display will be initilizied");
  //_tft.initR(INITR_BLACKTAB);  
  _tft.init();      // TFT_eSPI

  Serial.println("Initialized");
}
void CisternMonitoringDisplay::DrawResetView(){
  String title="Reset";
   //_tft.init();  
    _tft.fillScreen(TFT_BLACK);
      _tft.setTextColor(TFT_WHITE);
    _tft.setCursor( ( ( 128 - (title.length() * 12)) / 2 ), 1);
    _tft.setTextSize(2);
  _tft.println(title);
  _tft.setTextSize(1);
  _tft.println("");
  _tft.println("ESP will be Reset");
  _tft.println("Please wait...");
}
void CisternMonitoringDisplay::DrawMainView(bool loraConnected, String OnlineMode){
    _tft.fillScreen(TFT_BLACK); 
  //print Title
  _tft.setTextSize(1);
  _tft.setTextColor(TFT_WHITE);
  _tft.setTextWrap(true); //automatically makes breaklines

//Distance
 _tft.drawBitmap(0, 1, DistanceIcon, DistanceIconWidth, DistanceIconHeight, TFT_DARKGREY);

if(String(OnlineMode)=="Online"){
_tft.drawBitmap(_tft.width()-20, 1, WifiIcon, WifiIconWidth, WifiIconHeight, TFT_WHITE);
}else{
 _tft.drawBitmap(_tft.width()-20, 1, WifiIconDisabled, WifiIconDisabledWidth, WifiIconDisabledHeight, TFT_DARKGREY);
}

//Show Description for Lora
    if(loraConnected){
      _tft.drawBitmap(_tft.width()-45, 1, LoraIcon, LoraIconWidth, LoraIconHeight, TFT_GREEN);
   }else{
    _tft.drawBitmap(_tft.width()-45, 1, LoraIconDisabled, LoraIconDisabledWidth, LoraIconDisabledHeight, TFT_DARKGREY);
    }
   _tft.setTextColor(TFT_DARKGREY);


   //Update Offset
  // _tft.drawBitmap(_tft.width()-75, 1, OffsetIcon, OffsetIconWidth, OffsetIconHeight, TFT_DARKGREY);
  //_tft.setCursor(_tft.width()-60,5);
  //_tft.println(String(_offset));
 _tft.setTextColor(TFT_WHITE);

  //Draw Cistern visualisation
  //Part1: Borders of cistern
  _tft.fillRect(_xStartPositionCistern, _yStartPositionCistern,CisternBorderWidth , CisternBorderHeight, TFT_WHITE);

  //Part2: Interiour of cistern
  _tft.fillRect(_xStartPositionCistern + _borderWidth, _yStartPositionCistern , CisternContentWidth ,CisternContentHeight , TFT_BLACK);
  
  String waitInfo = "Waiting for";
  _tft.setCursor( ( ( 130 - (waitInfo.length() * 6)) / 2 ), 80);
  _tft.println(waitInfo);

   String waitInfo2 = "Sensor Data...";
   _tft.setCursor( ( ( 130 - (waitInfo2.length() * 6)) / 2 ), 90);
  _tft.print(waitInfo2);

}

void CisternMonitoringDisplay::updateWaterLevel(byte percentage, int litre, int measuredDistance,String ConnectionMode, bool OnlineMode){  

//Update WIFI Icon 
if(String(ConnectionMode) == "Online"){
  Serial.println("Display Connection Mode: " + String(ConnectionMode));
  if(OnlineMode){
    _tft.drawBitmap(_tft.width()-20, 1, WifiIcon, WifiIconWidth, WifiIconHeight, TFT_GREEN);
  }else{
    _tft.drawBitmap(_tft.width()-20, 1, WifiIcon, WifiIconWidth, WifiIconHeight, TFT_RED);
  }
}

//Update Distance
 _tft.drawBitmap(0, 1, DistanceIcon, DistanceIconWidth, DistanceIconHeight, TFT_WHITE);
  _tft.setCursor(15,5);
  _tft.setTextSize(1);
  _tft.setTextColor(TFT_WHITE);
  _tft.println(String(measuredDistance));

  _tft.setTextColor(TFT_WHITE);
//clean waterlevel before
  _tft.fillRect(_xStartPositionCistern + _borderWidth, _yStartPositionCistern , CisternContentWidth ,CisternContentHeight , TFT_BLACK);

  
  //avoid overloading and underloading warelevel
   if(percentage > 100){
    percentage = 100;
   }
   if(percentage < 0 ){
    percentage = 0;
   }
   
   //Update Watelevel
   //calculate height of waterlevel based on percentage
     Serial.println("UI:CisternContentMaxHeigt: " + String(CisternContentHeight) );
     Serial.println("UI:Percentage: " + String(percentage));
   byte waterlevel = round(CisternContentHeight/100.0*percentage);
   Serial.println("Waterlevel: "+String(waterlevel));
   Serial.println("TFT Height: "+String(_tft.height()));
 // _tft.fillRect(_xStartPositionCistern + _borderWidth, _tft.height()-(_SpaceToBottomOfDisplay + _yStartPositionCistern + _borderWidth) + _yStartPositionCistern , _tft.width()- (2 * _xStartPositionCistern + 2 * _borderWidth),waterlevel*(-1), TFT_BLUE);
_tft.fillRect(_xStartPositionCistern + _borderWidth, _tft.height()-(_SpaceToBottomOfDisplay + _yStartPositionCistern + _borderWidth + waterlevel) + _yStartPositionCistern  , _tft.width()- (2 * _xStartPositionCistern + 2 * _borderWidth),waterlevel, TFT_BLUE);
//Update Percentage information
   _tft.setTextSize(3);
    _tft.setTextColor(TFT_WHITE);
    String concatpercentage= String(percentage) + "%";
   _tft.setCursor( ( ( 128 - (concatpercentage.length() * 18)) / 2 ), 70);  //Center position
   _tft.println(concatpercentage);
  
    //Write litre information
    _tft.setTextSize(2);
    String concatlitre= String(litre) + " l";
   _tft.setCursor( ( ( 128 - (concatlitre.length() * 12)) / 2 ), 110);  //Center position
   _tft.println(concatlitre);


}
void CisternMonitoringDisplay::setOffset(byte offset){
  _offset=offset;
}
void CisternMonitoringDisplay::DrawConfigView(){
  String title="Config";
  // _tft.initR(INITR_BLACKTAB);  
    _tft.fillScreen(TFT_BLACK);
      _tft.setTextColor(TFT_WHITE);
    _tft.setCursor( ( ( 128 - (title.length() * 12)) / 2 ), 1);
    _tft.setTextSize(2);
  _tft.println(title);
  _tft.setTextSize(1);
  _tft.println("");
  _tft.println("1. Take a Smartphone");
  _tft.println("");
  _tft.println("2. Connect to WIFI:");
   _tft.println("");
  _tft.println("   Name:"+_AP_Name_ConfigMode);
  _tft.println("   Password:" + _AP_PW_ConfigMode);
  _tft.println("");
  _tft.println("3. Open a Browser"); 
  _tft.println("");
  _tft.println("4. Open IP 10.0.1.1");
  _tft.println("");
  _tft.println("5. Configure");

  }

  void CisternMonitoringDisplay::DrawSavedConfigView(){
 String title="Config";
  // _tft.initR(INITR_BLACKTAB);  
    _tft.fillScreen(TFT_BLACK);
      _tft.setTextColor(TFT_WHITE);
    _tft.setCursor( ( ( 128 - (title.length() * 12)) / 2 ), 1);
    _tft.setTextSize(2);
  _tft.println(title);
  _tft.setTextSize(1);
  _tft.println("");
  _tft.println("Save Configuration...");
  _tft.println("");
  _tft.println("Please wait...");

    
  }

  void CisternMonitoringDisplay::DrawFailedOnlineModeConfigView(){
 String title="Config";
   //_tft.initR(INITR_BLACKTAB);  
    _tft.fillScreen(TFT_BLACK);
      _tft.setTextColor(TFT_WHITE);
    _tft.setCursor( ( ( 128 - (title.length() * 12)) / 2 ), 1);
    _tft.setTextSize(2);
  _tft.println(title);
  _tft.setTextSize(1);
    _tft.setTextColor(TFT_RED);
  _tft.println("");
  _tft.println("Could not connect to internet!");
  _tft.println("");
  _tft.println("Please check your WIFI password!");
    _tft.println("");
          _tft.setTextColor(TFT_WHITE);
  _tft.println("Controller will be restart... ");
      _tft.println("");
  _tft.println("Please try it again!");

    
  }
