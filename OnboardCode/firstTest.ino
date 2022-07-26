#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

using namespace std;

//Notes
//CHANGE
//For auto switch time to 10-15. Time 4 digit stored with 
//NULL at end and no colon. OR only store time once like
//in program


//Memory on 1-8 address are zones
//0: inactive, 0: 1 active/waiting, 2: running.

//Memory on 9 address: off mode

//Auto time stored:
//Reduce Space ?
//Zone 1: 10 - 19 Start Time: 10: hr, 11: minute, 12: second, 13: month, 14: day, 15-16: year, 17: runtime
//Zone 2: 20 - 29
//Zone 3: 30 - 39
//Zone 4: 40 - 49
//...
//Zone 8: 80 - 89

//Program info stored:
//Program Number 1: 100 (Activity) = 1 Active, 0 Inactive, 0xFF if null
//Days: 101-107
//End Time: 108-109
//Zone 1 Info: 110-111(Start Time) 112-113(Runtime)
//Zone 2 Info: 115-116(Start Time) 117-118(Runtime)
//Zone 3 Info: 120-121(Start Time) 122-123(Runtime)
//...
//Zone N

//A: Auto mode. Set zones to run.
//B: Off mode. Select modes or set All off.
//C: Program mode.
//D: Enter button.
//*: ALL button (for use in modes)
//#: EXIT button (for use to exit modes)

//WORK ON
//Save space by rewriting auto zones. Make time bytes
//ADD DATE TO AUTO TIMES/SWITCH TIME AND DATE TO BYTES (year as int)
//Ask: when making zone does it tset active by default?

//Constants
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

//Declare and initialize Keys
char keys[ROW_NUM][COLUMN_NUM] = 
{
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

//Setup pins
byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//Declare and initialize Keys
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//Declare and initialize LCD
//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

RTC_DS3231 rtc;

//Variables
char tempTime[4], currProg[3], progStart[5], progRuntime[4], prevKey;
int idx = 0, tempIdx = 0, progNumber = 0, zoneRunning = -1, progMode = -1, prevMinute = 0, progRunning = 0, trackIdx = 0;
bool noTest = true, disableAutoCheck = true, flag = false, editing = false, zonesChanged = false;

enum autoMode{wait, getZones, getTimes, getDelay, setAuto};
enum programMode{pickProg, pickMode, pickZone, pickDay, pickStartTime, pickRuntime, pickActive, set, del};
autoMode at = wait;
programMode pt = pickProg;

//System State Enumeration
//Run: Default
//AUTO: Automatic start mode
//OFF: Off mode to stop zones
//PROG: Program mode for programs (1-20)
enum state{RUN, AUTO, OFF, PROG};
//System state variable, st
state st = RUN;

int prevTime = 0;
bool print = true;

//Pin Mapping Array
//Using Analog Pins
int zonesMap[8] = {A0, A1, A2, A3, 10, 11, 12, 13};
//Store zones current modes
int zonesMode[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//Track Keys in order
int trackKeys[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//Store days to use in given program
int progDays[7] = {0, 0, 0, 0, 0, 0, 0};
//Store zones used in given program
int currZones[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//Store day char identifiers
char days[7] = {'U', 'M', 'T', 'W', 'R', 'F', 'S'};
int storeTimes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//Setup Function (Runs once, at beginning)
void setup()
{
  //Start serial communication with keypad
  Serial.begin(9600);
  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  //Initialize interface and set size
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  //Enable pins for zones
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  //Make zones active (for simulation)
  setUpZones();
  //Set up programs
  setEEPROM();
  keepTime(print);
  prevMinute = rtc.now().minute();
}

//Loop Function (Runs continuously)
void loop()
{
  //Milliseconds code has run since board powered on
  int currentTime = millis();
  //Get time from RTC
  DateTime now = rtc.now();
  //Get off mode is enabled
  byte checkOff = EEPROM.read(9);
  //Check for second pass
  if(currentTime - prevTime >= 1000)
  {
    prevTime = currentTime;
    //Reset prevTime when millis() will reset to 0
    if(currentTime == 4294967295)
    {
      prevTime = -295;
    }
    //Print current time
    keepTime(print);
    //Check Running Times Each Second
    if(!disableAutoCheck && checkOff != 1)
    {
      checkAuto(now);
    }
  }  
  if(prevMinute == 59)
  {
    prevMinute = -1;
  }
  //Check for programs each minute
  if(now.minute() - prevMinute == 1)
  {
    //Make sure off mode is disabled
    if(checkOff != 1)
    {
      checkProg(now);
    }
    //Set previous minute to current
    prevMinute = now.minute();
  }
  //Get keypad entry and store in key
  char key = keypad.getKey();
  //Check key is valid
  if(key)
  {
    //Check for mode entry if system running
    if(st == RUN)
    {
      //Switch to decide system mode
      switch(key)
      {
        case 'A':
      	  st = AUTO;
          at = getZones;
      	  break;
        case 'B':
      	  st = OFF;
      	  break;
        case 'C':
      	  st = PROG;
       	  break;
      }
    }
    if(key == '#')
    {
      //Exit
        if(st == AUTO)
        {
          at = wait;
          memset(tempTime, 0, 4);
          for(int i = 0; i < 8; i++)
          {
            storeTimes[i] = 0;
            currZones[i] = 0;
            trackKeys[i] = 0;
          }
          idx = 0;
          noTest = true;
          tempIdx = 0;
          prevKey = '\0';
          trackIdx = 0;
        }
        else if(st == PROG)
        {
          resetProg();
        }
        printToLcdFF(F("Exiting..."), F(""));
        delay(1000);
        st = RUN;
    }
    //Check state for AUTO mode
    else if(st == AUTO)
    {
      //Check if off mode is enabled
      if(EEPROM.read(9) == 1)
      {
        st = RUN;
        printToLcdFF(F("Can't set while"), F("Off Mode enabled"));
        delay(2000);
      }
      else
      {
        if(at == getZones)
        {
          //Print default mode message
          printToLcdFF(F("Manual Mode"), F("Select Zones:"));
          //Prevent specific keys
          if(key != 'A' && key != 'D')
          {
            //Validate key is 1-8
            if(key > '0' && key < '9')
            {
              //Get mode of desired zone. 
              int mode = zonesMode[((int)key) - 48 - 1];
              //Make sure zone is active
              if(mode == 1)
              {
                lcd.clear();
                printToLcdFF(F("Selected Zone(s)"), F(""));
                lcd.setCursor(0, 1);
                currZones[((int)key) - 48 - 1] = 1;
                for(int i = 0; i < 8; i++)
                {
                  if(currZones[i] == 1)
                  {
                    lcd.print(i + 1);
                    lcd.print(F(" "));
                  }
                }
                if(trackIdx == 0)
                {
                  trackKeys[trackIdx] = ((int)key) - 48;
                  trackIdx++;
                }
                else
                {
                  bool noAdd = false;
                  for(int i = 0; i < trackIdx; i++)
                  {
                    if(((int)key - 48) == trackKeys[trackIdx])
                    {
                      noAdd = true;
                      break;
                    }
                  }
                  if(!noAdd)
                  {
                    trackKeys[trackIdx] = ((int)key) - 48;
                    trackIdx++;
                  }
                }
              }
              //Check if zone is running
              else if(mode == 2)
              {
                char buf[15] = "Zone ";
                char tempKey[2];
                tempKey[0] = key;
                tempKey[1] = '\0';
                strcat(buf, tempKey);
                printToLcdCF(strcat(buf, " Running"), F("Try Another Zone"));
              }
              //Check if zone is inactive
              else if(mode == 0)
              {
                char buf[15] = "Zone ";
                char tempKey[2];
                tempKey[0] = key;
                tempKey[1] = '\0';
                strcat(buf, tempKey);
                printToLcdCF(strcat(buf," Inactive"), F("Try Another Zone"));
              }
            }
            else if(key == 'B')
            {
              if(trackIdx != 0)
              {
                currZones[trackKeys[trackIdx - 1] - 1] = 0;
                trackKeys[trackIdx - 1] = 0;
                trackIdx--;
              }
              lcd.clear();
              printToLcdFF(F("Selected Zone(s)"), F(""));
              lcd.setCursor(0, 1);
              for(int i = 0; i < 8; i++)
              {
                if(currZones[i] == 1)
                {
                  lcd.print(i + 1);
                  lcd.print(F(" "));
                }
              }
            }
            //Check if entry is a zone
            else
            {
              char buf[15];
              char tempKey[2];
              tempKey[0] = key;
              tempKey[1] = '\0';
              strcat(buf, tempKey);
              printToLcdCF(strcat(buf, " is not a zone."), F(""));
            }
          }
          //Check if Enter key hit
          if(key == 'D')
          {
            while(currZones[idx] != 1 && idx < 8)
            {
              idx++;
            }
            char buf[15] = "Enter Zone ";
            char tempKey[2];
            tempKey[0] = (idx + 1) + '0';
            tempKey[1] = '\0';
            strcat(buf, tempKey);
            printToLcdCF(buf, F("Duration"));
            at = getTimes;
          }
        }
        if(at == getTimes)
        {
          if(!noTest)
          {
            if(key == 'D')
            {
              storeTimes[idx] = atoi(tempTime);
              idx++;
              if(idx < 8)
              {
                while(currZones[idx] != 1 && idx < 8)
                {
                  idx++;
                }
                if(idx >= 8)
                {
                  at = getDelay;
                  noTest = true;
                  tempIdx = 0;
                  memset(tempTime, 0, 4);
                  printToLcdFF(F("Enter delay: "), F(""));
                }
                else
                {
                  memset(tempTime, 0, 4);
                  tempIdx = 0;
                  char buf[15] = "Enter Zone ";
                  char tempKey[2];
                  tempKey[0] = (idx + 1) + '0';
                  tempKey[1] = '\0';
                  strcat(buf, tempKey);
                  printToLcdCF(buf, F("Duration"));
                }
              }
            }
            else if(key == 'B')
            {
              if(tempIdx != 0)
              {
                tempTime[tempIdx - 1] = '\0';
                tempIdx--;
                printToLcdCF(tempTime, F(""));
              }
              if(tempIdx == 0)
              {
                char buf[15] = "Enter Zone ";
                char tempKey[2];
                tempKey[0] = (idx + 1) + '0';
                tempKey[1] = '\0';
                strcat(buf, tempKey);
                printToLcdCF(buf, F("Duration"));
              }
            }
            else
            {
              tempTime[tempIdx] = key;
              tempIdx++;
              printToLcdCF(tempTime, F(""));
            }
          }
          else
          {
            noTest = false;
          }
        }
        if(at == getDelay)
        {
          if(!noTest)
          {
            if(key == 'D')
            {
              at = setAuto;
              noTest = true;
            }
            else if(key == 'B')
            {
              if(tempIdx != 0)
              {
                tempTime[tempIdx - 1] = '\0';
                tempIdx--;
                printToLcdCF(tempTime, F(""));
              }
              if(tempIdx == 0)
              {
                char buf[15] = "Enter Zone ";
                char tempKey[2];
                tempKey[0] = (idx + 1) + '0';
                tempKey[1] = '\0';
                strcat(buf, tempKey);
                printToLcdCF(buf, F("Duration"));
              }
            }
            else
            {
              tempTime[tempIdx] = key;
              tempIdx++;
              printToLcdCF(tempTime, F(""));
            }
          }
          else
          {
            noTest = false;
          }
        }
        if(at == setAuto)
        {
          //Print message, set cursor to second row, print zones
          printToLcdFF(F("Auto Set. Zones:"), F(""));
          lcd.setCursor(0, 1);
          for(int i = 0; i < 8; i++)
          {
            if(currZones[i] == 1)
            {
              lcd.print(i + 1);
              lcd.print(F(" "));
            }
          }
          int del = atoi(tempTime);
          int min = del;
          DateTime curr = rtc.now();
          for(int i = 0; i < 8; i++)
          {
            if(storeTimes[i] != 0)
            {
              if(i != 0)
              {
                min += storeTimes[i - 1];
              }
              DateTime start (curr + TimeSpan(0, 0, min, 0));
              //10: hr, 11: minute, 12: second, 13: month, 14: day, 15-16: year, 17: runtime
              //WRITE HOUR
              EEPROM.write((i + 1)* 10, (byte)start.hour());
              //WRITE MINUTE
              EEPROM.write(((i + 1)* 10) + 1, (byte)start.minute());
              //WRITE SECOND
              EEPROM.write(((i + 1)* 10) + 2, (byte)start.second());
              //WRITE DAY
              EEPROM.write(((i + 1)* 10) + 3, (byte)start.day());
              //WRITE MONTH
              EEPROM.write(((i + 1)* 10) + 4, (byte)start.month());
              //WRITE YEAR
              writeIntToEEPROM(((i + 1)* 10) + 5, (int)start.year());
              //Write Runtime
              writeIntToEEPROM(((i + 1)* 10) + 7, storeTimes[i]);
            }
          }
          for(int i = 0; i < 8; i++)
          {
            storeTimes[i] = 0;
            currZones[i] = 0;
            trackKeys[i] = 0;
          }
          at = wait;
          memset(tempTime, 0, 4);
          idx = 0;
          tempIdx = 0;
          lcd.setCursor(0, 0);
          st = RUN;
          noTest = true;
          disableAutoCheck = false;
          trackIdx = 0;
          DateTime temp = rtc.now();
          checkAuto(temp);
          delay(2000);
        }
      }
    }
    //Check if system state is OFF
    else if(st == OFF)
    {
      byte offMode = EEPROM.read(9);
      if(offMode == 1)
      {
        //Print default OFF mode message
        printToLcdFF(F("OFF Mode Enabled"), F("Enter to Disable"));
      }
      else
      {
        //Print default OFF mode message
        printToLcdFF(F("OFF Mode. Press"), F("Enter to Enable"));
      }
      //Check if key is * (All button)
      if(!noTest)
      {
        if(key == 'D')
        {
          if(offMode == 1)
          {
            EEPROM.write(9, 0);
            printToLcdFF(F("Disabling Off"), F("Mode..."));
          }
          else
          {
            printToLcdFF(F("Turning Zones"), F("Off..."));
            EEPROM.write(9, 1);
            //Loop through all zones
            for(int i = 0; i < 8; i++)
            {
              digitalWrite(zonesMap[i], LOW);
              if(zonesMode[i] == 2)
              {
                zonesMode[i] = 1;
              }
              //Erase Auto Mode Data
              for(int j = 0; j < 10; j++)
              {
                EEPROM.write((10 * (i + 1)) + j, 0xFF);
              }
            }
          }
          delay(2000);
          //Change system state to default RUN
          st = RUN;
          noTest = true;
        }
        //If key isn't recognized as zone
        else
        {
          printToLcdFF(F("Incorrect Entry."), F("Try Again."));   
        }
      }
      else
      {
        noTest = false;
      }
    }
    //If system state is Program mode
    else if(st == PROG)
    {
      //Print Program mode message
      printToLcdFF(F("Program Mode"), F("Select Program:"));
      //If pick program is true
      if(pt == pickProg)
      {
        //If noTest is false (Ensures first key is not used)
        if(!noTest)
        {
          //Get current program number to edit
          if(key >= '0' && key <= '9')
          {
            currProg[tempIdx] = key;
            tempIdx++;
            printToLcdCF(currProg, F(""));
          }
          //If Enter button clicked
          else if(key == 'D')
          {
            progNumber = atoi(currProg);
            //Check progNumber is between 1-20
            if(progNumber < 1 || progNumber > 18)
            {
              //Print invalid program number message
              printToLcdFF(F("Invalid Program#"), F("Try Again."));
              memset(currProg, 0, 3);
              tempIdx = 0;
            }
            else
            {
              //Enable zone pick
              memset(currProg, 0, 3);
              tempIdx = 0;
              pt = pickMode;
              noTest = true;
            }
          }
          else if(key == 'B')
          {
            if(tempIdx != 0)
            {
              currProg[tempIdx - 1] = '\0';
              tempIdx--;
              printToLcdCF(currProg, F(""));
            }
            if(tempIdx == 0)
            {
              printToLcdFF(F("Program Mode"), F("Select Program:"));
            }
          }
        }
        //Disable noTest so next key entries are used
        else
        {
          noTest = false;
        }
      }
      if(pt == pickMode)
      {
        byte currActivity = EEPROM.read(100 + (50 * (progNumber - 1)));
        bool exists = false;
        if(currActivity == 0xFF)
        {
          printToLcdFF(F("Mode: Create(A)"), F(""));
        }
        else
        {
          printToLcdFF(F("Mode: Edit(A)"), F("Delete(B) Set(C)"));
          exists = true;
          editing = true;
        }
        if(!noTest)
        {
          //Edit, Delete, or set active
          //A: Edit
          //B: Delete
          //C: Activity
          if(key == 'A' || flag)
          {
            if(currActivity == 0xFF)
            {
              printToLcdFF(F("Select Zone(s):"), F(""));
              pt = pickZone;
              noTest = true;
            }
            else
            {
              if(!flag)
              {
                printToLcdFF(F("Program Exists."), F("Displaying data."));
                loadProgInputs();
                delay(2000);
              }
              flag = true;
              printToLcdFF(F("Current Zone(s):"), F(""));
              lcd.setCursor(0, 1);
              for(int i = 0; i < 8; i++)
              {
                if(currZones[i] != 0)
                {
                  lcd.print(i + 1);
                  lcd.print(" ");
                }
              } 
              if(key == '1')
              {
                pt = pickZone;
                printToLcdFF(F("Editing Zones..."), F(""));
                delay(1000);
                printToLcdFF(F("Select Zone(s):"), F(""));
                noTest = true;
                flag = false;
                zonesChanged = true;
                for(int i = 0; i < 8; i++)
                {
                  currZones[i] = 0;
                }
              }
              else if(key == '0')
              {
                pt = pickDay;
                printToLcdFF(F("Skipping Zones..."), F(""));
                delay(1000);
                noTest = true;
                flag = false;
              }
            }
          }
          else if(key == 'B' && exists)
          {
            pt = del;
            noTest = true;

          }
          else if(key == 'C' && exists)
          {
            pt = set;
            noTest = true;
          }
          else
          {
            printToLcdFF(F("Not mode. Please"), F("Try Again."));
          }
        }
        else
        {
          noTest = false;
        }
      }
      if(pt == set)
      {
        byte checkAct = EEPROM.read(100 + (50 * (progNumber - 1)));
        if(checkAct == 1)
        {
          printToLcdFF(F("Program active"), F("Deactivate (0)?"));
        }
        else if(checkAct == 0)
        {
          printToLcdFF(F("Program inactive"), F("Activate (1)?"));
        }
        if(!noTest)
        {
          if(key == '1' && checkAct != 1)
          {
            setProgram(progNumber, true);
            char buf[15] = "Program ";
            printToLcdCF(strcat(buf, currProg), F("Activated."));
            delay(2000);
            if(checkConflict(progNumber))
            {
              delay(3000);
            }
            resetProg();
          }
          else if(key == '0' && checkAct == 1)
          {
            setProgram(progNumber, false);
            char buf[15] = "Program ";
            printToLcdCF(strcat(buf, currProg), F("Deactivated."));
            delay(2000);
            resetProg();
          }
          else
          {
            if(key == '0')
            {
              printToLcdFF(F("Program already"), F("inactive."));
              resetProg();
            }
            else
            {
              printToLcdFF(F("Program already"), F("active."));
              resetProg();
            }
            delay(2000);
          }
          if(key != '1' && key != '0')
          {
            printToLcdFF(F("Improper input."), F("Try Again."));
          }
        }
        else
        {
          noTest = false; 
        }
      }
      if(pt == del)
      {
        printToLcdFF(F("Are you sure you"), F("want to delete?"));
        if(!noTest)
        {
          if(key == 'D')
          {
            deleteProgram(progNumber);
            st = RUN;
            pt = pickProg;
            noTest = true;
            char buf[15] = "Program ";
            printToLcdCF(strcat(buf, currProg), F("Deleted."));
            delay(2000);
          }
        }
        else
        {
          noTest = false;
        }
      }
      //If pickZone is true, get zones from user
      if(pt == pickZone)
      {
        //If noTest is false continue
        if(!noTest)
        {
          //Check key is from 1-8
          if(key > '0' && key < '9')
          {
            //Get zones for programming
            int currZone = (int)key - 48;
            //Check zone activiy
            if(zonesMode[currZone - 1] != 1 && zonesMode[currZone - 1] != 2)
            {
              char buf[15];
              char tempKey[2];
              tempKey[0] = key;
              tempKey[1] = '\0';
              strcat(buf, tempKey);
              printToLcdCF(strcat(buf, " Not Active."), F("Try Again."));
            }
            //If zone is active
            else
            {
              currZones[currZone - 1] = 1;
              //print selected zones message
              printToLcdFF(F("Zone(s) Selected"), F(""));
              lcd.setCursor(0, 1);
              //Loop through zones
              for(int i = 0; i < 8; i++)
              {
                //If zone is active print
                if(currZones[i] == 1)
                {
                  lcd.print(i + 1);
                  lcd.print(F(" "));
                }
              }
              lcd.setCursor(0, 0);
              if(trackIdx == 0)
              {
                trackKeys[trackIdx] = ((int)key) - 48;
                trackIdx++;
              }
              else
              {
                bool noAdd = false;
                for(int i = 0; i < trackIdx; i++)
                {
                  if(((int)key - 48) == trackKeys[trackIdx])
                  {
                    noAdd = true;
                    break;
                  }
                }
                if(!noAdd)
                {
                  trackKeys[trackIdx] = ((int)key) - 48;
                  trackIdx++;
                }
              }
            }
          }
          //If key is not from 1-8
      	  else
          {
            //If key is * select All zones
            if(key == '*')
            {
              for(int i = 0; i < 8; i++)
              {
                currZones[i] = 1;
              }
              //Print selected zones message
              printToLcdFF(F("Zone(s) Selected"), F("ALL"));
            }
            //If key is Enter switch to day selection
            else if(key == 'D')
            {
              pt = pickDay;
              noTest = true;
              trackIdx = 0;
              for(int i = 0; i < 8; i++)
              {
                trackKeys[i] = 0;
              }
            }
            else if(key == 'B')
            {
              if(trackIdx != 0)
              {
                currZones[trackKeys[trackIdx - 1] - 1] = 0;
                trackKeys[trackIdx - 1] = 0;
                trackIdx--;
              }
              lcd.clear();
              printToLcdFF(F("Selected Zone(s)"), F(""));
              lcd.setCursor(0, 1);
              for(int i = 0; i < 8; i++)
              {
                if(currZones[i] == 1)
                {
                  lcd.print(i + 1);
                  lcd.print(F(" "));
                }
              }
            }
            //If zone is not valid
            else
            {
              //Print zone validity message
              printToLcdFF(F("Not Valid Zone."), F("Try Again."));
            }
          }
        }
        //Change NoTest to allow for next key entries
        else
        {
          noTest = false;
        }
      }
      //If pickDay is true
      if(pt == pickDay)
      {
        byte currActivity = EEPROM.read(100 + (50 * (progNumber - 1)));
        if(!flag)
        {
          if(currActivity == 0xFF)
          {
            //Print day selection message
            printToLcdFF(F("Pick Day(s):"), F(""));
            flag = true;
          }
          else
          {
            printToLcdFF(F("Current Day(s):"), F(""));
            lcd.setCursor(0, 1);
            for(int i = 0; i < 7; i++)
            {
              if(progDays[i] == 1)
              {
                lcd.print(days[i]);
                lcd.print(" ");
              }
            }  
            if(!noTest)
            {
              if(key == '1')
              {
                flag = true;
                noTest = true;
                printToLcdFF(F("Editing Days..."), F(""));
                delay(1000);
                printToLcdFF(F("Pick Day(s):"), F(""));
                for(int i = 0; i < 7; i++)
                {
                  progDays[i] = 0;
                }
              }
              else if(key == '0')
              {
                printToLcdFF(F("Skipping Days..."), F(""));
                delay(1000);
                pt = pickStartTime;
                noTest = true;
                flag = false;
              }
            }
            else
            {
              noTest = false;
            }
          }
        }
        if(flag)
        {
          //Prevent first character
          if(!noTest)
          {
            //Check if key is from 0-6
            if(key >= '0' && key < '7')
            {
              //Store days used in this program #
              progDays[(int)key - 48] = 1;
              //Print message about days selected
              printToLcdFF(F("Day(s) Selected:"), F(""));
              lcd.setCursor(0, 1);
              for(int i = 0; i < 7; i++)
              {
                if(progDays[i] == 1)
                {
                  lcd.print(days[i]);
                  lcd.print(F(" "));
                }
              }
              lcd.setCursor(0, 0);
              if(trackIdx == 0)
              {
                trackKeys[trackIdx] = ((int)key) - 48;
                trackIdx++;
              }
              else
              {
                bool noAdd = false;
                for(int i = 0; i < trackIdx; i++)
                {
                  if(((int)key - 48) == trackKeys[trackIdx])
                  {
                    noAdd = true;
                    break;
                  }
                }
                if(!noAdd)
                {
                  trackKeys[trackIdx] = ((int)key) - 48;
                  trackIdx++;
                }
              }
            }
            else
            {
              //Check if key is *, if so then set all days to 1
              if(key == '*')
              {
                for(int i = 0; i < 7; i++)
                {
                  progDays[i] = 1;
                }
                printToLcdFF(F("Day(s) Selected:"), F("ALL"));
              }
              //Check if key is Enter, move to picking start time
              else if(key == 'D')
              {
                pt = pickStartTime;
                noTest = true;
                flag = false;
              }
              else if(key == 'B')
              {
                if(trackIdx != 0)
                {
                  progDays[trackKeys[trackIdx - 1]] = 0;
                  trackKeys[trackIdx - 1] = 0;
                  trackIdx--;
                }
                lcd.clear();
                printToLcdFF(F("Day(s) Selected:"), F(""));
                lcd.setCursor(0, 1);
                for(int i = 0; i < 7; i++)
                {
                  if(progDays[i] == 1)
                  {
                    lcd.print(days[i]);
                    lcd.print(F(" "));
                  }
                }
              }
              //Else, must not have entered a day
              else
              {
                printToLcdFF(F("Not a day."), F("Try Again."));
              } 
            }
          }
          //Set noTest to allow for key input after first key
          else
          {
            noTest = false;
          }
        }
      }
      //If true, proceed getting start time
      if(pt == pickStartTime)
      {
        //Print default message for start time
        byte currActivity = EEPROM.read(100 + (50 * (progNumber - 1)));
        if(!flag)
        {
          if(currActivity == 0xFF)
          {
            printToLcdFF(F("Enter Start Time"), F(""));
            flag = true;
          }
          else
          {
            printToLcdFF(F("Current Start:"), F(""));
            lcd.setCursor(0, 1);
            int j = 0;
            for(int i = 0; i < 5; i++)
            {
              if(i == 2)
              {
                lcd.print(F(":"));
              }
              else
              {
                lcd.print(progStart[j]);
                j++;
              }
            }
            if(!noTest)
            {
              if(key == '1')
              {
                flag = true;
                noTest = true;
                printToLcdFF(F("Editing Time..."), F(""));
                delay(1000);
                printToLcdFF(F("Enter Start Time:"), F(""));
                memset(progStart, 0, 5);
              }
              else if(key == '0')
              {
                printToLcdFF(F("Skipping Time..."), F(""));
                delay(1000);
                pt = pickRuntime;
                noTest = true;
                flag = false;
              }
            }
            else
            {
              noTest = false;
            }
          }
        }
        if(flag)
        {
          //Skip first key (Enter from previous decision)
          if(!noTest)
          {
            //Ensure key is valid number, 0-9
            if(key >= '0' && key <= '9')
            {
              //Get key as long as progStart is less than 4
              //Time wont be over 4 values, always needs leading 0
              if(progStart[3] == NULL)
              {
                progStart[tempIdx] = key;
                tempIdx++;
              }
              //Clear LCD
              lcd.clear();
              //Print time as being entered
              for(int i = 0; i < 4; i++)
              {
                lcd.print(progStart[i]); 
                if(i == 1)
                {
                  lcd.print(F(":"));
                }
              }
            }
            //If Enter, check time for validity
            else if(key == 'D')
            {
              bool validTime = checkValidTime(progStart);
              //If time not valid print error message
              if(!validTime)
              {
                printToLcdFF(F("Time Invalid."), F("Try Again."));
                memset(progStart, 0, 5);
                tempIdx = 0;
              }
              //Else, time valid, move to picking end time
              else
              {
                pt = pickRuntime;
                noTest = true;
                flag = false;
                //Find first active zone
                while(currZones[idx] != 1 && idx < 8)
                {
                  idx++;
                } 
                tempIdx = 0;
              }
            }
            else if(key == 'B')
            {
              if(tempIdx != 0)
              {
                progStart[tempIdx - 1] = '\0';
                tempIdx--;
                lcd.clear();
                //Print time as being entered
                for(int i = 0; i < 4; i++)
                {
                  lcd.print(progStart[i]); 
                  if(i == 1)
                  {
                    lcd.print(F(":"));
                  }
                }
              }
              if(tempIdx == 0)
              {
                printToLcdFF(F("Enter Start Time:"), F(""));
              }
            }
            //Else, value entered must be invalid, reset start string
            else
            {
              printToLcdFF(F("Value Invalid."), F("Try Again."));
              memset(progStart, 0, 5);
            }
          }
          //Reset noTest to allow for next keys entered
          else
          {
            noTest = false;
          }
        }
      }
      //If true, proceed with getting end time
      if(pt == pickRuntime)
      { 
        byte currActivity = EEPROM.read(100 + (50 * (progNumber - 1)));
        if(!flag)
        {
          if(currActivity == 0xFF)
          {
            char buf[15] = "Zone ";
            char tempKey[2];
            tempKey[0] = (idx + 1) + '0';
            tempKey[1] = '\0';
            printToLcdFC(F("Enter Runtime"), strcat(buf, tempKey));
            flag = true;
          }
          else
          {
            if(zonesChanged)
            {
              noTest = false;
            }
            else
            {
              printToLcdFF(F("Edit Runtimes?"), F(""));
            }
            if(!noTest && !flag)
            {
              if(key == '1' || zonesChanged)
              {
                flag = true;
                noTest = true;
                printToLcdFF(F("Editing"), F("Runtimes..."));
                delay(1000);
                printToLcdFF(F("Runtime Zone"), F(""));
                while(currZones[idx] != 1 && idx < 8)
                {
                  idx++;
                }
                lcd.setCursor(13, 0);
                lcd.print(idx + 1);
                lcd.print(F(":"));
                for(int i = 0; i < 8; i++)
                {
                  storeTimes[i] = 0;
                }
              }
              else if(key == '0')
              {
                printToLcdFF(F("Skipping"), F("Runtimes..."));
                delay(1000);
                printToLcdFF(F("Editing"), F("Program..."));
                delay(2000);
                writeProgInfoToEEPROM((int)EEPROM.read(100 + (50 * (progNumber - 1))));
                if(checkConflict(progNumber))
                {
                  delay(3000);
                }
                resetProg();
              }
            }
            else
            {
              noTest = false;
            }
          }
        }
        if(flag)
        {
          //Prevent first key
          if(!noTest)
          {
            //Ensure key is valid number, 0-9
            if(key >= '0' && key <= '9')
            {
              //Get keys while progEnd is less than 4
              if(progRuntime[3] == NULL)
              {
                progRuntime[tempIdx] = key;
                tempIdx++;
              }
              //Clear and print time as entered
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(progRuntime);
            }
            else if(key == 'B')
            {
              if(tempIdx != 0)
              {
                progRuntime[tempIdx - 1] = '\0';
                tempIdx--;
                printToLcdCF(progRuntime, F(""));
              }
              if(tempIdx == 0)
              {
                char buf[15] = "Zone ";
                char tempKey[2];
                tempKey[0] = (idx + 1) + '0';
                tempKey[1] = '\0';
                printToLcdFC(F("Enter Runtime"), strcat(buf, tempKey));
              }
            }
            else if(key == 'D')
            {
              if(atoi(progRuntime) == 0)
              {
                printToLcdFF(F("Runtime can't be"), F("0. Try Again."));
              }
              else
              {
                storeTimes[idx] = atoi(progRuntime);
                idx++;
                tempIdx = 0;
                memset(progRuntime, 0, 4);
                if(idx < 8)
                {
                  while(currZones[idx] != 1 && idx < 8)
                  {
                    idx++;
                  }
                  if(idx < 8)
                  {
                    char buf[15] = "Zone ";
                    char tempKey[2];
                    tempKey[0] = (idx + 1) + '0';
                    tempKey[1] = '\0';
                    printToLcdFC(F("Enter Runtime"), strcat(buf, tempKey));
                  }
                }
                if(idx == 8)
                {
                  if(zonesChanged)
                  {
                    writeProgInfoToEEPROM((int)EEPROM.read(100 + (50 * (progNumber - 1))));
                    printToLcdFF(F("Editing"), F("Program..."));
                    delay(2000);
                    if(checkConflict(progNumber))
                    {
                      delay(3000);
                    }
                    resetProg();
                  }
                  else
                  {
                    pt = pickActive;
                    noTest = true;
                    printToLcdFF(F("Set Active(1) or"), F("Inactive(0)."));
                  }
                }
              }            
            }
            //Else, values must be invalid
            else
            {
              printToLcdFF(F("Value Invalid."), F("Try Again."));
              memset(progRuntime, 0, 4);
            }
          }
          //Allow for keys to be used
          else
          {
            noTest = false;
          }
        }
      }
      if(pt == pickActive)
      {
        if(!noTest)
        {
          if(key == '0' || key == '1')
          {
            progMode = (int)key - 48;
            char temp[2];
            temp[0] = key;
            temp[1] = '\0';
            printToLcdCF(temp, F(""));
          }
          else if(key == 'B')
          {
            printToLcdFF(F("Set Active(1) or"), F("Inactive(0)."));
            progMode = -1;
          }
          else if(key == 'D')
          {
            if(progMode == 1 || progMode == 0)
            {
              if(progMode == 1)
              {
                printToLcdFF(F("Program Set"), F("Active."));
              }
              else if(progMode == 0)
              {
                printToLcdFF(F("Program Set"), F("Inactive."));
              }
              writeProgInfoToEEPROM(progMode);
              delay(2000);
              if(checkConflict(progNumber))
              {
                delay(3000);
              }
              resetProg();
            }
            else
            {
              printToLcdFF(F("Improper Entry."), F("Try Again."));
            }
          }
          else
          {
            printToLcdFF(F("Value Invalid."), F("Try Again."));
          }
        }
        else
        {
          noTest = false;
        }
      }
    }
    //If system state is run, print default message
    if(st == RUN)
    {
      print = true;
    }
    else
    {
      print = false;
    }
    prevKey = key;
  }
}

//For simulation
//Setting 1 through 3, 4 inactive
void setUpZones()
{
  for(int i = 0; i < 8; i++)
  {
    EEPROM.write(i + 1, 1);
    zonesMode[i] = 1;
  }
}

//Print to LCD formatting firstLine to be first row and
//secondLine to be second row
void printToLcdFF(const __FlashStringHelper *firstLine, const __FlashStringHelper *secondLine)
{
  lcd.clear();
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
  lcd.setCursor(0, 0);
}

void printToLcdFC(const __FlashStringHelper *firstLine, char secondLine[15])
{
  lcd.clear();
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
  lcd.setCursor(0, 0);
}

void printToLcdCF(char firstLine[15], const __FlashStringHelper *secondLine)
{
  lcd.clear();
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
  lcd.setCursor(0, 0);
}

//Checks str to see if time is valid
bool checkValidTime(char str[5])
{
  //Get substring of str, first two characters (hour), then cast to int
  //Do same for minutes, last two characters
  char locHour[3], locMinute[3];
  int hour, minute; 
  locHour[0] = str[0];
  locHour[1] = str[1];
  locHour[2] = '\0';
  locMinute[0] = str[2];
  locMinute[1] = str[3];
  locMinute[2] = '\0';
  hour = atoi(locHour);
  minute = atoi(locMinute);
  //Check hour for valid number in 24 hour format
  if(hour < 0 || hour > 24)
  {
    return false;
  }
  //Check minute for valid number
  if(minute < 0 || minute > 59)
  {
    return false;
  }
  //Return true if valid
  return true;
}

//Resets global variables used in program mode
void resetProg()
{
  //Reset variables used in programming 
  pt = pickProg;
  noTest = true;
  flag = false;
  editing = false;
  zonesChanged = false;
  memset(progStart, 0, 5);
  memset(progRuntime, 0, 4);
  memset(currProg, 0, 3);
  tempIdx = 0;
  idx = 0;
  for(int i = 0; i < 8; i++)
  {
    progDays[i] = 0;
    storeTimes[i] = 0;
    if(i < 7)
    {
      currZones[i] = 0;
    }
  }
  st = RUN;
}

void writeProgInfoToEEPROM(int mode)
{
  //Base Address
  int baseAddr = 100 + ((progNumber - 1) * 50);
  //Write activity
  EEPROM.write(baseAddr, (byte)mode);
  //Write Days
  for(int i = 0; i < 7; i++)
  {
    EEPROM.write(baseAddr + 1 + i, (byte)progDays[i]);
  }
  baseAddr += 10;
  byte locHour, locMinute;
  char temp[3];
  temp[0] = progStart[0];
  temp[1] = progStart[1];
  temp[2] = '\0';
  locHour = atoi(temp);
  temp[0] = progStart[2];
  temp[1] = progStart[3];
  temp[2] = '\0';
  locMinute = atoi(temp);
  //Write Runtime Info
  for(int i = 0; i < 8; i++)
  {
    if(storeTimes[i] != 0)
    {
      //Write Start Time
      EEPROM.write(baseAddr, locHour);
      EEPROM.write(baseAddr + 1, locMinute);
      locMinute += storeTimes[i];
      while(locMinute >= 60)
      {
        locMinute -= 60;
        locHour++;
      }
      //Write Runtime
      writeIntToEEPROM(baseAddr + 2, storeTimes[i]);
    }
    else
    {
      EEPROM.write(baseAddr, 0xFF);
      EEPROM.write(baseAddr + 1, 0xFF);
      EEPROM.write(baseAddr + 2, 0xFF);
      EEPROM.write(baseAddr + 3, 0xFF);
    }
    baseAddr += 5;
  }
  //Write Total End Time
  baseAddr = 108 + ((progNumber - 1) * 50);
  EEPROM.write(baseAddr, locHour);
  EEPROM.write(baseAddr + 1, locMinute);
}

void deleteProgram(int deleteNum)
{
  //Base Address
  int baseAddr = 100 + ((deleteNum - 1) * 50);
  //Write activity
  EEPROM.write(baseAddr, 0xFF);
  //Write Days
  for(int i = 0; i < 7; i++)
  {
    EEPROM.write(baseAddr + 1 + i, 0xFF);
  }
  EEPROM.write(baseAddr + 8, 0xFF);
  EEPROM.write(baseAddr + 9, 0xFF);
  baseAddr += 10;
  //Write Runtime Info
  for(int i = 0; i < 8; i++)
  {
    //Write Start Time
    EEPROM.write(baseAddr, 0xFF);
    EEPROM.write(baseAddr + 1, 0xFF);
    //Write Runtime
    EEPROM.write(baseAddr + 2, 0xFF);
    EEPROM.write(baseAddr + 3, 0xFF);
    baseAddr += 5;
  }
}

void setProgram(int setNumber, bool activate)
{
  int baseAddr = 100 + ((setNumber - 1) * 50);
  if(activate)
  {
    EEPROM.write(baseAddr, 1);
  }
  else
  {
    EEPROM.write(baseAddr, 0);
  }
}

void keepTime(bool pr)
{
  if(pr)
  {
    DateTime loc = rtc.now();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(loc.month(), DEC);
    lcd.print(F("/"));
    lcd.print(loc.day(), DEC);
    lcd.print(F("/"));
    lcd.print(loc.year(), DEC);
    lcd.setCursor(0, 1);
    if((int)loc.hour() < 10)
    {
      lcd.print(F("0"));
    }
    lcd.print(loc.hour(), DEC);
    lcd.print(F(":"));
    if((int)loc.minute() < 10)
    {
      lcd.print(F("0"));
    }
    lcd.print(loc.minute(), DEC);
    lcd.print(F(":"));
    if((int)loc.second() < 10)
    {
      lcd.print(F("0"));
    }
    lcd.print(loc.second(), DEC);
  }
}

void writeIntToEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}

void checkAuto(DateTime t)
{
  //Check if zone should turn off
  if(zoneRunning == -1)
  {
    for(int i = 0; i < 8; i++)
    {
      //Check for Auto Zone i data
      byte check = EEPROM.read((i + 1) * 10);
      if(check != 0xFF)
      {
        zoneRunning = i + 1;
        break;
      }
    }
  }
  //Else zone running, check for auto information
  if(zoneRunning != -1)
  {
    bool zoneWipe = false, found = false;
    byte hr, min, sec, day, month;
    int year, rt;
    hr = EEPROM.read(zoneRunning * 10);
    min = EEPROM.read((zoneRunning * 10) + 1);
    sec = EEPROM.read((zoneRunning * 10) + 2);
    day = EEPROM.read((zoneRunning * 10) + 3);
    month = EEPROM.read((zoneRunning * 10) + 4);
    year = readIntFromEEPROM((zoneRunning * 10) + 5);
    rt = readIntFromEEPROM((zoneRunning * 10) + 7);
    DateTime curr(year, month, day, hr, min, sec);
    DateTime future (curr + TimeSpan(0, 0, rt, 0));
    if(t.hour() == future.hour() && t.minute() == future.minute() && t.second() == future.second() && t.day() == future.day() && t.month() == future.month() && t.year() == future.year())
    {
      //Turn off zone
      digitalWrite(zonesMap[zoneRunning - 1], LOW);
      zonesMode[zoneRunning - 1] = 1;
      zoneWipe = true;
    }
    if(zoneWipe)
    {
      for(int i = 0; i < 10; i++)
      {
        EEPROM.write((zoneRunning * 10) + i, 0xFF); 
      }
      zoneRunning = -1;
    }
    if(zoneRunning == -1)
    {
      for(int i = 0; i < 8; i++)
      {
        //Check for Auto Zone i data
        byte check = EEPROM.read((i + 1) * 10);
        if(check != 0xFF)
        {
          zoneRunning = i + 1;
          found = true;
          break;
        }
      }
    }
    if(zoneRunning != -1)
    {
      hr = EEPROM.read(zoneRunning * 10);
      min = EEPROM.read((zoneRunning * 10) + 1);
      sec = EEPROM.read((zoneRunning * 10) + 2);
      day = EEPROM.read((zoneRunning * 10) + 3);
      month = EEPROM.read((zoneRunning * 10) + 4);
      year = readIntFromEEPROM((zoneRunning * 10) + 5);
      if(hr == (byte)t.hour() && min == (byte)t.minute() && sec == (byte)t.second() && day == (byte)t.day() && month == (byte)t.month() && year == (int)t.year())
      {
        //Turn on zone
        digitalWrite(zonesMap[zoneRunning - 1], HIGH);
        zonesMode[zoneRunning - 1] = 2;
      }
    }
  }
}

int checkRunning()
{
  for(int i = 0; i < 8; i++)
  {
    if(zonesMode[i] == 2)
    {
      return i + 1;
    }
  }
  return -1;
}

void setEEPROM()
{
  for(int i = 0; i < 1023; i++)
  {
    EEPROM.write(i, 0xFF);
  }
}

void loadProgInputs()
{
  //Base Address
  int baseAddr = 100 + ((progNumber - 1) * 50);
  //Read Days
  for(int i = 0; i < 7; i++)
  {
    progDays[i] = EEPROM.read(baseAddr + 1 + i);
  }
  baseAddr += 10;
  byte locHour;
  for(int i = 0; i < 8; i++)
  {
    locHour = EEPROM.read(baseAddr);
    if(locHour != 0xFF)
    {
      break;
    }
    baseAddr += 5;
  }
  byte locMin = EEPROM.read(baseAddr + 1);
  char bufHour[3], bufMin[3];
  itoa(locHour, bufHour, 10);
  itoa(locMin, bufMin, 10);
  //Read Start Time
  progStart[0] = bufHour[0];
  progStart[1] = bufHour[1];
  progStart[2] = bufMin[0];
  progStart[3] = bufMin[1];
  //Read Runtime Info
  baseAddr = 110 + ((progNumber - 1) * 50);
  for(int i = 0; i < 8; i++)
  {
      //Read Runtime
      int temp = EEPROM.read(baseAddr + 2);
      if(temp != 0xFF)
      {
        storeTimes[i] = readIntFromEEPROM(baseAddr + 2);
        currZones[i] = 1;
      }
      else
      {
        currZones[i] = 0;
      }
    baseAddr += 5;
  }
}

void checkProg(DateTime t)
{
  int stop = 0;
  while(true)
  {
    int baseAddr = 100;
    if(stop == 2)
    {
      break;
    }  
    if(zoneRunning == -1)
    {
      if(progRunning == 0)
      {
        baseAddr -= 50;
        for(int i = 0; i < 18; i++)
        {
          baseAddr += 50;
          byte mode = EEPROM.read(baseAddr);
          if(mode == 1)
          {
            for(int j = 0; j < 7; j++)
            {
              byte day = EEPROM.read(baseAddr + j + 1);
              if(day == 1)
              {
                if(t.dayOfTheWeek() == j)
                {
                  int tempBaseAddr = baseAddr + 10;
                  for(int k = 0; k < 8; k++)
                  {
                    byte locHour = EEPROM.read(tempBaseAddr);
                    byte locMin = EEPROM.read(tempBaseAddr + 1);
                    if(locHour == (byte)t.hour() && locMin == (byte)t.minute())
                    { 
                      zoneRunning = k + 1;
                      progRunning = i + 1;
                      digitalWrite(zonesMap[zoneRunning - 1], HIGH);
                    }
                    else
                    {
                      if(locHour != 0xFF && locMin != 0xFF)
                      {
                        break;
                      }
                    }
                    tempBaseAddr += 5;  
                  }                      
                }
              }
            }
          }
        }
      }
    }
    else
    {
      baseAddr = 110 + ((progRunning - 1) * 50) + (5 * (zoneRunning - 1));
      byte locHour = EEPROM.read(baseAddr);
      byte locMin = EEPROM.read(baseAddr + 1);
      int rt = readIntFromEEPROM(baseAddr + 2);
      DateTime curr(t.year(), t.month(), t.day(), locHour, locMin, 0);
      DateTime future (curr + TimeSpan(0, 0, rt, 0));
      if(t.hour() == future.hour() && t.minute() == future.minute())
      {
        digitalWrite(zonesMap[zoneRunning - 1], LOW);
        zoneRunning = -1;
      }
    }
    if(zoneRunning == -1)
    {
      if(progRunning != 0)
      {
        baseAddr = 110 + ((progRunning - 1) * 50) + ((zoneRunning - 1) * 5);
        for(int k = zoneRunning - 1; k < 8; k++)
        {
          byte locHour = EEPROM.read(baseAddr);
          byte locMin = EEPROM.read(baseAddr + 1);
          if(locHour == (byte)t.hour() && locMin == (byte)t.minute())
          { 
            zoneRunning = k + 1;
            digitalWrite(zonesMap[zoneRunning - 1], HIGH);
            break;
          }
          baseAddr += 5;
        }
        if(zoneRunning == -1)
        {
          progRunning = 0;
        }
      }
    }
    stop++;
  }
}

bool checkConflict(int testProg)
{
  int baseAddr = 110;
  byte currMode, testMode;
  int locStart, testStart, locEnd, testEnd;
  for(int k = 0; k < 8; k++)
  {
    testStart = EEPROM.read(baseAddr + (50 * (testProg - 1)));
    if(testStart != 0xFF)
    {
      testStart *= EEPROM.read((baseAddr + 1) + (50 * (testProg - 1)));
      break;
    }
    baseAddr += 5;
  }
  baseAddr = 100;
  testEnd = EEPROM.read((baseAddr + 8) + (50 * (testProg - 1))) * EEPROM.read((baseAddr + 9) + (50 * (testProg - 1)));
  testMode = EEPROM.read(baseAddr + (50 * (testProg - 1)));
  if(testMode == 1)
  {
    for(int i = 0; i < 18; i++)
    {
      currMode = EEPROM.read(baseAddr);
      if(currMode == 1 && i != (testProg - 1))
      {
        for(int j = 0; j < 7; j++)
        {
          byte locDay = EEPROM.read(101 + (50 * i) + j);
          byte testDay = EEPROM.read(101 + (50 * (testProg - 1)) + j);
          if(locDay == 1 && testDay == 1)
          {
            int tempBaseAddr = baseAddr + 10;
            for(int k = 0; k < 8; k++)
            {
              locStart = EEPROM.read(tempBaseAddr);
              if(locStart != 0xFF)
              {
                locStart *= EEPROM.read(tempBaseAddr + 1);
                break;
              }
              tempBaseAddr += 5;
            }
            locEnd = EEPROM.read(baseAddr + 8) * EEPROM.read(baseAddr + 9);
            if(((testEnd > locStart) && (testEnd <= locEnd)) || ((testStart >= locStart) && (testStart < locEnd)) || ((testStart <= locStart) && (testEnd >= locEnd)))
            {
              printToLcdFF(F("WARNING: Time"), F("Conf. w/ Prog "));
              lcd.setCursor(14, 1);
              lcd.print(i + 1);
              delay(4000);
              return true;
            }
          }
        }
      }
      baseAddr += 50;
    }
  }
  return false;
}