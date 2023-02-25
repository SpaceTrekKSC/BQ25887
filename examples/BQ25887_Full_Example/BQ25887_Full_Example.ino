/*BQ25887_Full_Example.ino
 * Author: Andrew Gafford
 * email:  agafford@spacetrek.com
 * date:   May 2020
 * 
 * The BQ25887 library provides full access to all of the register fields
 * of the BQ25887 I2C Controlled 2-Cell, 2-A Boost-Mode Battery Charger.
 * 
 * Improperly charging a lithium battery can cause an explosion and fire.
 * Make sure you know what you are doing if you change any of the settings
 * from the default values.  Use this program at your own risk.  I am NOT
 * responsible for any damage or bodily harm that is cased by using this
 * library.
 * 
 * Each register has a read method and each field has a get method. Before
 * using the get methods the read method for the corresponding register 
 * must be called to refresh the field data. The read methods return a
 * 1, 2, or 4 byte unsigned integer, except for the ADC read methods that 
 * return signed integers.  The get methods return a floating point, boolean, 
 * unsigned integer, or signed integer depending on the type of data in the
 * field.
 */


#include <Arduino.h>
#include <Wire.h>
#include <BQ25887.h>

#define BQ_CD_PIN 66
#define ERROR_LED A5

BQ25887 charger = BQ25887();


//global variables for first five regsiters
float cellVoltageLimit = 0.0;

float chargeCurrentLimit = 0.0;
bool hizMode = true;
bool iLimPin  = false;

float inputVoltageLimit = 0.0;
bool vInDpmRst = false;
bool batDischg = true;
bool pfmOoa = true;

float inputCurrentLimit = 0.0;
bool forceIco = true;
bool forceIndet = true;
bool enIco = false;

float prechargeCurrentLimit = 0.0;
float terminationCurrentLimit = 0.0;



void setup() {  
  pinMode(BQ_CD_PIN, OUTPUT);
  digitalWrite(BQ_CD_PIN, LOW);
  
  Serial.begin(115200);
  delay(3000);

  if(charger.begin()){
    Serial.println(F("BQ25887 is working..."));
  }
  else{
    Serial.println(F("BQ25887 is NOT working.  Check your wires..."));
    for(int i = 0; i > 20; i++){
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }

  Serial.println(F("Starting..."));
  Serial.println();

//  charger.registerReset();
//  delay(100);
  
  getFirstFiveRegs();
  printFirstFiveRegs();

  Serial.println(F("Setting Fields..."));
  setFields();

//  charger.setADC_EN(true);                                    //enable ADC control
//  charger.setADC_ONE_SHOT(false);                             //enable ADC one-shot mode

//  charger.setEN_CHG(true);

  Serial.println(F("Polling all registers..."));
  charger.pollAllRegs();
  printAllFields();  

  Serial.println(F("Setup finsihed... Entering Loop..."));  

  

}

void loop() {
//  readStatFields();
//  printStatFields();
//  
//  readADCRegs();
//  printADCRegs();
//  Serial.println(); Serial.println();
//  
//  blinkFaults();

  
  printAllFields(); 


  delay(10000);
  charger.wdReset();
}

void getFirstFiveRegs(){
  //each register has a read method to get the values from the register
  //the read method must be called before using a get method  
  //the get methods return the values from fields within the registers

  Serial.println(F("Reading Cell Voltage Limit Register..."));
  charger.readCellVoltageLimitReg();                                        //the cell voltage limit is the max voltage allowed on each cell.  It has a range of 3.4V to 4.6V
  cellVoltageLimit = charger.getCellVoltageLimit();                         //getCellVoltageLimit() returns the voltage limit as a floating point number

  Serial.println(F("Reading Charge Current Limit Register..."));
  charger.readChargeCurrentLimitReg();
  chargeCurrentLimit = charger.getChargeCurrentLimit();
  hizMode = charger.getHIZMode();
  iLimPin = charger.getILIMPinFunction();

  Serial.println(F("Reading Input Voltage Limit Register..."));
  charger.readInputVoltageLimitReg();
  inputVoltageLimit = charger.getInputVoltageLimit();
  vInDpmRst = charger.getEN_VINDPM_RST();
  batDischg = charger.getEN_BAT_DISCHG();
  pfmOoa = charger.getPFM_OOA_DIS();

  Serial.println(F("Reading Input Current Limit Register..."));
  charger.readInputCurrentLimitReg();
  inputCurrentLimit = charger.getInputCurrentLimit();
  forceIco = charger.getFORCE_ICO();
  forceIndet = charger.getFORCE_INDET();
  enIco = charger.getEN_ICO();

  Serial.println(F("Reading Precharge & Termination Charge Current Limit Register..."));
  charger.readPreTermCurrentLimitReg();
  prechargeCurrentLimit = charger.getPrechargeCurrentLimit();
  terminationCurrentLimit = charger.getTerminationCurrentLimit();

  Serial.println(F("Done reading first five registers..."));
  Serial.println();
}

void printFirstFiveRegs(){
  Serial.print(F("Cell Voltage Limit: ")); Serial.println(cellVoltageLimit);
  Serial.print(F("Charge Current Limit: ")); Serial.println(chargeCurrentLimit);
  Serial.print(F("hizMode: ")); Serial.println(hizMode);
  Serial.print(F("iLimPin: ")); Serial.println(iLimPin);
  Serial.print(F("Input Voltage Limit: ")); Serial.println(inputVoltageLimit);
  Serial.print(F("vInDpmRst: ")); Serial.println(vInDpmRst);
  Serial.print(F("batDischg: ")); Serial.println(batDischg);
  Serial.print(F("pfmOoa: ")); Serial.println(pfmOoa);
  Serial.print(F("Input Current Limit: ")); Serial.println(inputCurrentLimit);
  Serial.print(F("forceIco: ")); Serial.println(forceIco);
  Serial.print(F("forceIndet: ")); Serial.println(forceIndet);
  Serial.print(F("enIco: ")); Serial.println(enIco);
  Serial.print(F("Precharge Current Limit: ")); Serial.println(prechargeCurrentLimit);
  Serial.print(F("Termination Current Limit: ")); Serial.println(terminationCurrentLimit);
  Serial.println();
}

void setFields(){
//  charger.setCellVoltageLimit(4.2);                           //cell voltage limit in Volt as a floating point, Range 3.4 - 4.6, default is 4.2V
//  charger.setChargeCurrentLimit(1.5);                         //charger current limit in Amp as a floating point, range 0.1 - 2.2, default is 1.5A
//  charger.setHIZMode(false);                                  //high impedance mode as boolean true or false, default is false
//  charger.setILIMPinFunction(true);                           //enable current limit pin as boolean true or false, default is true
//  charger.setInputVoltageLimit(4.3);                          //input voltage limit as floating point, Range 3.9 - 5.5, default is 4.3V
//  charger.setEN_VINDPM_RST(true);                             //enable VINDPM reset when adapter is plugged in as boolean true or false, default is true
//  charger.setEN_BAT_DISCHG(false);                            //enable battery discharge load as boolean true or false, default is false
//  charger.setPFM_OOA_DIS(false);                              //disable PFM Out-of-Audio mode as boolean true or false, default is false
//  charger.setInputCurrentLimit(3.0);                          //input current limit in Amps as floating point, range 0.5 - 3.3, default is 3.0A
//  charger.setFORCE_ICO(false);                                //force start input current optimizer as boolean true or false, default is false
//  charger.setFORCE_INDET(false);                              //force PSEL input detection as boolean true or false, default is false
//  charger.setEN_ICO(true);                                    //enable input current optimization algorithm control as boolean true or false, default is true
//  charger.setPrechargeCurrentLimit(0.15);                     //precharge current limit in Amp as floating point, range 0.05 - 0.8, default is 0.15;
//  charger.setTerminationCurrentLimit(0.15);                   //termination current limit in Amp as floating point, range 0.05 - 0.8, default 0.15A
//  charger.setEN_TERM(true);                                   //enable termination control as boolean true or false, default is true
//  charger.setSTAT_DIS(false);                                 //disable status pin as boolean true or false, default is false
//  charger.setWATCHDOG(1);                                     //watchdog timer setting as uint8_t, 0 is disable WD timer, 1 is 40s, 2 is 80s, 3 is 160s, incorrect values go to default value of 1
//  charger.setEN_TIMER(true);                                  //enable charging safety timer as boolean true or false, default is true
//  charger.setCHG_TIMER(2);                                    //fast charge timer setting as uint8_t, 0 is 5hrs, 1 is 8hrs, 2 is 12hrs, 3 is 20 hrs, incorrect values go to default value of 2
//  charger.setTMR2X_EN(true);                                  //enable slow safety timer by 2X during DPM or TREG as boolean true or false, default is true
//  charger.setAUTO_INDET_EN(true);                             //enable auto PSEL input detection as boolean true or false, default is true
//  charger.setT_REG_THRESH(3);                                 //thermal regulation threshold as uint8_t, 0 is 60C, 1 is 80C, 2 is 100C, 3 is 120C, incorrect values go to default value of 3
//  charger.setEN_CHG(true);                                    //enable charger as boolean true or false, default is true
//  charger.setCELLLOWV_THRESH(1);                              //cell low voltage threshold as uint8_t, 0 is 2.8V, 1 is 3.0V, incorrect values go to default value of 1
//  charger.setVCELL_RECHG_THRESH_OFF(1);                       //recharge threshold offset as uint8_t, 0 is 0.05V, 1 is 0.1V, 2 is 0.15V, 3 is 0.2V, incorrect values go to default value of 1
//  charger.setPFM_DIS(false);                                  //disable PFM Mode as boolean true or false, default is false
//  charger.setTOPOFF_TIMER(0);                                 //top-off timer setting as uint8_t, 0 is disable, 1 is 15min, 2 is 30min, 3 is 45min, incorrect values go to default value of 0
//  charger.setJEITA_VSET(1);                                   //JEITA high temp voltage setting as uint8_t, 0 is suspend charge, 1 is 8.0V, 2 is 8.3V, 3 is VREG unchanged, incorrect values go to default value of 1
//  charger.setJEITA_ISETH(1);                                  //JEITA high temp current setting as uint8_t, 0 is 40% ICHG, 1 is 100% ICHG, incorrect values go to default value of 1
//  charger.setJEITA_ISETC(1);                                  //JEITA low temp current setting as uint8_t, 0 is suspend charge, 1 is 20% ICHG, 2 is 40% ICHG, 3 is 100% ICH, incorrect values go to default value of 1
//  charger.setADC_DONE_MASK(false);                            //mask ADC conversion done from producing an INT pulse
//  charger.setIINDPM_MASK(false);                              //mask IINDPM regulation from producing an INT pulse
//  charger.setVINDPM_MASK(false);                              //mask VINDPM regulation from producing an INT pulse
//  charger.setT_REG_MASK(false);                               //mask IC temperature regulation from producing an INT pulse
//  charger.setWD_MASK(false);                                  //mask watchdog timer from producing an INT pulse
//  charger.setCHRG_MASK(false);                                //mask charge status from producing an INT pulse
//  charger.setPG_MASK(false);                                  //mask power good from producing an INT pulse
//  charger.setVBUS_MASK(false);                                //mask VBUS status from producing an INT pulse
//  charger.setTS_MASK(false);                                  //mask TS status from producing an INT pulse
//  charger.setICO_MASK(false);                                 //mask input current optimizer from producing an INT pulse
//  charger.setVBUS_OVP_MASK(false);                            //mask input over-voltage fault from producing an INT pulse
//  charger.setTSHUT_MASK(false);                               //mask thermal shutdown fault from producing an INT pulse
//  charger.setTMR_MASK(false);                                 //mask charge safety timer fault from producing an INT pulse
//  charger.setSNS_SHORT_MASK(false);                           //mask SNS short fault from producing an INT pulse
//  charger.setADC_EN(true);                                    //enable ADC control
//  charger.setADC_ONE_SHOT(false);                             //enable ADC one-shot mode
//  charger.setADC_SAMPLE_SPEED(0);                             //ADC sample speed as uint8_t, 0 is slowest with 15 bit resolution, 1 has 14 bit resolution, 2 has 13 bit resolution, 3 is the fastest with 12 bit resolution, incorrect values default to 0
//  charger.setIBUS_ADC_DIS(false);                             //disable ADC IBUS
//  charger.setICHG_ADC_DIS(false);                             //disable ADC ICHG
//  charger.setVBUS_ADC_DIS(false);                             //disable ADC VBUS
//  charger.setVBAT_ADC_DIS(false);                             //disable ADC VBAT
//  charger.setTS_ADC_DIS(false);                               //disbale ADC TS
//  charger.setVCELL_ADC_DIS(false);                            //disable ADC VCELL
//  charger.setTDIE_ADC_DIS(false);                             //disable ADC TDIE
//  charger.setVDIFF_END_OFFSET(1);                             //cell ballancing exit threshold as uint8_t, 0 throu 7, 0 is 0.03V, 7 is 0.1V, step is 0.01V, incorrect values go to the default value of 1
//  charger.setTCB_QUAL_INTERVAL(0);                            //interval between taking meassurments for cell balancing as uint8_t, 0 is 2min, 1 is 4 min, incorrect values go to default value of 0
//  charger.setTCB_ACTIVE(2);                                   //time interval to stop charging and discharging for cell voltage meassurments as uint8_t, 0 is 4s, 1 is 32s, 2 is 2min, 3 is 4min, incorrect values go to default value of 2
//  charger.setTSETTLE(2);                                      //delay between charge disable and voltage meassurment as uint8_t, 0 is 10ms, 1 is 100ms, 2 is 1s, 3 is 2s, incorrect values go to default value of 2
//  charger.setVQUAL_TH(15);                                    //threshold from cell balancing pre-qual to cell balancing qual. 0 is 40mV, 14 is 180mV, 10mV steps. 15 is disabled.  If incorrect value sets to default disabled
//  charger.setVDIFF_START(4);                                  //threshold from cell balancing qual to cell balancing active. 0 is 40mV, 15 is 190mV, 10mV steps.  If incorrect value goes to default of 4
//  charger.setCB_CHG_DIS(true);                                //disable charge for accurate cell balancing measurment
//  charger.setCB_AUTO_EN(true);                                //enable automatic cell balancing
//  charger.setQCBL_EN(false);                                  //turn on QCBH to discharge top cell
//  charger.setQCBH_EN(false);                                  //turn on QCBL to discharge bottom cell
//  charger.setCB_MASK(false);                                  //mask cell balancing INT pulse
//  charger.setHS_CV_MASK(false);                               //mask high side cell balancing FET in CV mode INT pulse
//  charger.setLS_CV_MASK(false);                               //mask low side cell balancing FET in CV mode INT pulse
//  charger.setHS_OV_MASK(false);                               //mask high cell in over voltage INT pulse
//  charger.setLS_OV_MASK(false);                               //mask low cell in over voltage INT pulse
//  charger.setCB_OC_MASK(false);                               //mask cell balance over-current protection active INT pulse

//  charger.wdReset();                                          //reset watchdog timer
//  charger.registerReset();                                    //resets all the registers
}

void printAllFields(){
  charger.pollAllRegs();

  //Cell Voltage Limit
  Serial.print(F("Cell Voltage Limit: ")); Serial.println(charger.getCellVoltageLimit());

  //Charge Current Limit
  Serial.print(F("Charge Current Limit: ")); Serial.println(charger.getChargeCurrentLimit());
  Serial.print(F("hizMode: ")); Serial.println(charger.getHIZMode());
  Serial.print(F("iLimPin: ")); Serial.println(charger.getILIMPinFunction());

  //Input Voltage Limit
  Serial.print(F("Input Voltage Limit: ")); Serial.println(charger.getInputVoltageLimit());
  Serial.print(F("vInDpmRst: ")); Serial.println(charger.getEN_VINDPM_RST());
  Serial.print(F("batDischg: ")); Serial.println(charger.getEN_BAT_DISCHG());
  Serial.print(F("pfmOoa: ")); Serial.println(charger.getPFM_OOA_DIS());

  //Input Current Limit
  Serial.print(F("Input Current Limit: ")); Serial.println(charger.getInputCurrentLimit());
  Serial.print(F("forceIco: ")); Serial.println(charger.getFORCE_ICO());
  Serial.print(F("forceIndet: ")); Serial.println(charger.getFORCE_INDET());
  Serial.print(F("enIco: ")); Serial.println(charger.getEN_ICO());

  //Pre/Term Current Limit
  Serial.print(F("Precharge Current Limit: ")); Serial.println(charger.getPrechargeCurrentLimit());
  Serial.print(F("Termination Current Limit: ")); Serial.println(charger.getTerminationCurrentLimit());

  //charge control settings
  Serial.print(F("Enable Termination: ")); Serial.println(charger.getEN_TERM());
  Serial.print(F("Status Disable: ")); Serial.println(charger.getSTAT_DIS());
  Serial.print(F("Watchdog: ")); Serial.println(charger.getWATCHDOG());
  Serial.print(F("Enable Timer: ")); Serial.println(charger.getEN_TIMER());
  Serial.print(F("Charge Timer: ")); Serial.println(charger.getCHG_TIMER());
  Serial.print(F("Timer 2X Enable: ")); Serial.println(charger.getTMR2X_EN());
  Serial.print(F("Auto Input Detect Enable: ")); Serial.println(charger.getAUTO_INDET_EN());
  Serial.print(F("Thermal Regulation Threshold: ")); Serial.println(charger.getT_REG_THRESH());
  Serial.print(F("Enable Charging: ")); Serial.println(charger.getEN_CHG());
  Serial.print(F("Cell Low Volt Threshold: ")); Serial.println(charger.getCELLLOWV_THRESH());
  Serial.print(F("Volt Cell Recharge Threshold Offset: ")); Serial.println(charger.getVCELL_RECHG_THRESH_OFF());
  Serial.print(F("PFM Disable: ")); Serial.println(charger.getPFM_DIS());
  Serial.print(F("Watchdog Reset: ")); Serial.println(charger.getWD_RST());
  Serial.print(F("Top-Off Timer: ")); Serial.println(charger.getTOPOFF_TIMER());
  Serial.print(F("JEITA Volt Set: ")); Serial.println(charger.getJEITA_VSET());
  Serial.print(F("JEITA Current Set HOT: ")); Serial.println(charger.getJEITA_ISETH());
  Serial.print(F("JEITA Current Set COLD: ")); Serial.println(charger.getJEITA_ISETC());

  //ICO Current Limit
  Serial.print(F("ICO Current Limit: ")); Serial.println(charger.getICOCurrentLimit());

  //Charge Status  
  Serial.print(F("Current In DPM Status: ")); Serial.println(charger.getIINDPM_STAT());
  Serial.print(F("Voltage In DPM Status: ")); Serial.println(charger.getVINDPM_STAT());
  Serial.print(F("Thermal Regulation Status: ")); Serial.println(charger.getTREG_STAT());
  Serial.print(F("Watchdog Status: ")); Serial.println(charger.getWD_STAT());
  Serial.print(F("Charge Status: ")); Serial.println(charger.getCHRG_STAT());
  Serial.print(F("Power Good Status: ")); Serial.println(charger.getPG_STAT());
  Serial.print(F("VBUS Status: ")); Serial.println(charger.getVBUS_STAT());
  Serial.print(F("ICO Status: ")); Serial.println(charger.getICO_STAT());

  //NTC Status
  Serial.print(F("NTC Status: ")); Serial.println(charger.getNTCStatus());

  //Fault Status
  Serial.print(F("VBUS OVP Fault Status: ")); Serial.println(charger.getVBUS_OVP_STAT());
  Serial.print(F("TSHUT Fault Status: ")); Serial.println(charger.getTSHUT_STAT());
  Serial.print(F("Timer Status: ")); Serial.println(charger.getTMR_STAT());

  //Charge Flags
  Serial.print(F("Current In DPM Flag: ")); Serial.println(charger.getIINDPM_FLAG());
  Serial.print(F("Voltage In DPM Flag: ")); Serial.println(charger.getVINDPM_FLAG());
  Serial.print(F("Thermal Regulation Flag: ")); Serial.println(charger.getTREG_FLAG());
  Serial.print(F("Watchdog Flag: ")); Serial.println(charger.getWD_FLAG());
  Serial.print(F("Charge Flag: ")); Serial.println(charger.getCHRG_FLAG());
  Serial.print(F("Power Good Flag: ")); Serial.println(charger.getPG_FLAG());
  Serial.print(F("VBUS Flag: ")); Serial.println(charger.getVBUS_FLAG());
  Serial.print(F("TS Flag: ")); Serial.println(charger.getTS_FLAG());
  Serial.print(F("ICO Flag: ")); Serial.println(charger.getICO_FLAG());

  //Fault Flags
  Serial.print(F("VBUS OVP Fault Flag: ")); Serial.println(charger.getVBUS_OVP_FLAG());
  Serial.print(F("TSHUT Fault Flag: ")); Serial.println(charger.getTSHUT_FLAG());
  Serial.print(F("Timer Fault Flag: ")); Serial.println(charger.getTMR_FLAG());

  //Charger INT Masks
  Serial.print(F("ADC Done Mask: ")); Serial.println(charger.getADC_DONE_MASK());
  Serial.print(F("IINDPM Mask: ")); Serial.println(charger.getIINDPM_MASK());
  Serial.print(F("VINDOM Mask: ")); Serial.println(charger.getVINDPM_MASK());
  Serial.print(F("T_REG Mask: ")); Serial.println(charger.getT_REG_MASK());
  Serial.print(F("Watchdog Mask: ")); Serial.println(charger.getWD_MASK());
  Serial.print(F("Charge Mask: ")); Serial.println(charger.getCHRG_MASK());
  Serial.print(F("Power Good Mask: ")); Serial.println(charger.getPG_MASK());
  Serial.print(F("VBUS Mask: ")); Serial.println(charger.getVBUS_MASK());
  Serial.print(F("TS Mask: ")); Serial.println(charger.getTS_MASK());
  Serial.print(F("ICO Mask: ")); Serial.println(charger.getICO_MASK());

  //Fault INT Masks
  Serial.print(F("VBUS OVP Mask: ")); Serial.println(charger.getVBUS_OVP_MASK());
  Serial.print(F("TSHUT Mask: ")); Serial.println(charger.getTSHUT_MASK());
  Serial.print(F("Timer Mask: ")); Serial.println(charger.getTMR_MASK());
  Serial.print(F("SNS Short Mask: ")); Serial.println(charger.getSNS_SHORT_MASK());

  //ADC Control Settings
  Serial.print(F("ADC Enable: ")); Serial.println(charger.getADC_EN());
  Serial.print(F("ADC One Shot: ")); Serial.println(charger.getADC_ONE_SHOT());
  Serial.print(F("ADC Sample Speed: ")); Serial.println(charger.getADC_SAMPLE_SPEED());

  //ADC Function Disable Settings
  Serial.print(F(": ")); Serial.println(charger.getIBUS_ADC_DIS());
  Serial.print(F(": ")); Serial.println(charger.getICHG_ADC_DIS());
  Serial.print(F(": ")); Serial.println(charger.getVBUS_ADC_DIS());
  Serial.print(F(": ")); Serial.println(charger.getVBAT_ADC_DIS());
  Serial.print(F(": ")); Serial.println(charger.getTS_ADC_DIS());
  Serial.print(F(": ")); Serial.println(charger.getVCELL_ADC_DIS());
  Serial.print(F(": ")); Serial.println(charger.getTDIE_ADC_DIS());

  //ADC
  Serial.print(F("ADC IBUS: ")); Serial.println(charger.getADC_IBUS());
  Serial.print(F("ADC ICHG: ")); Serial.println(charger.getADC_ICHG());
  Serial.print(F("ADC VBUS: ")); Serial.println(charger.getADC_VBUS());
  Serial.print(F("ADC VBAT: ")); Serial.println(charger.getADC_VBAT());
  Serial.print(F("ADC Volt Top Cell: ")); Serial.println(charger.getADC_VCELLTOP());
  Serial.print(F("ADC TS: ")); Serial.println(charger.getADC_TS());
  Serial.print(F("ADC TDIE: ")); Serial.println(charger.getADC_TDIE());
  Serial.print(F("ADC Volt Bottom Cell: ")); Serial.println(charger.getADC_VCELLBOT());

  //Part INFO
  Serial.print(F("Part Number: ")); Serial.println(charger.getPartNumber());
  Serial.print(F("Device Revision: ")); Serial.println(charger.getDevRev());

  //Cell Balance Control Settings
  Serial.print(F("VDIFF END Offset: ")); Serial.println(charger.getVDIFF_END_OFFSET());
  Serial.print(F("TCB Qual Interval: ")); Serial.println(charger.getTCB_QUAL_INTERVAL());
  Serial.print(F("TCB Active: ")); Serial.println(charger.getTCB_ACTIVE());
  Serial.print(F("T Settle: ")); Serial.println(charger.getTSETTLE());
  Serial.print(F(":V Qual Threshold ")); Serial.println(charger.getVQUAL_TH());
  Serial.print(F("VDIFF Start: ")); Serial.println(charger.getVDIFF_START());

  //Cell Balance Status
  Serial.print(F(": ")); Serial.println(charger.getCB_CHG_DIS());
  Serial.print(F(": ")); Serial.println(charger.getCB_AUTO_EN());
  Serial.print(F(": ")); Serial.println(charger.getCB_STAT());
  Serial.print(F(": ")); Serial.println(charger.getHS_CV_STAT());
  Serial.print(F(": ")); Serial.println(charger.getLS_CV_STAT());
  Serial.print(F(": ")); Serial.println(charger.getHS_OV_STAT());
  Serial.print(F(": ")); Serial.println(charger.getLS_OV_STAT());
  Serial.print(F(": ")); Serial.println(charger.getCB_OC_STAT());

  //Cell Balance Flags
  Serial.print(F(": ")); Serial.println(charger.getQCBH_EN());
  Serial.print(F(": ")); Serial.println(charger.getQCBL_EN());
  Serial.print(F(": ")); Serial.println(charger.getCB_FLAG());
  Serial.print(F(": ")); Serial.println(charger.getHS_CV_FLAG());
  Serial.print(F(": ")); Serial.println(charger.getLS_CV_FLAG());
  Serial.print(F(": ")); Serial.println(charger.getHS_OV_FLAG());
  Serial.print(F(": ")); Serial.println(charger.getLS_OV_FLAG());
  Serial.print(F(": ")); Serial.println(charger.getCB_OC_FLAG());

  //Cell Balance INT Mask
  Serial.print(F(": ")); Serial.println(charger.getCB_MASK());
  Serial.print(F(": ")); Serial.println(charger.getHS_CV_MASK());
  Serial.print(F(": ")); Serial.println(charger.getLS_CV_MASK());
  Serial.print(F(": ")); Serial.println(charger.getHS_OV_MASK());
  Serial.print(F(": ")); Serial.println(charger.getLS_OV_MASK());
  Serial.print(F(": ")); Serial.println(charger.getCB_OC_MASK());


  
  
  
}

void readADCRegs(){
  charger.readADCIbusReg();
  charger.readADCIchgReg();
  charger.readADCVbusReg();
  charger.readADCVbatReg();
  charger.readADCVCellTopReg();
  charger.readADCTsReg();
  charger.readADCTDieReg();
  charger.readADCVCellBotReg();
}

void printADCRegs(){
  Serial.println();
  Serial.print(F("ADC IBUS: ")); Serial.println(charger.getADC_IBUS());
  Serial.print(F("ADC ICHG: ")); Serial.println(charger.getADC_ICHG());
  Serial.print(F("ADC VBUS: ")); Serial.println(charger.getADC_VBUS());
  Serial.print(F("ADC VBAT: ")); Serial.println(charger.getADC_VBAT());
  Serial.print(F("ADC CELL TOP: ")); Serial.println(charger.getADC_VCELLTOP());
  Serial.print(F("ADC CELL BOT: ")); Serial.println(charger.getADC_VCELLBOT());
}

void readStatFields(){  
  charger.readChargeStatusReg();
  charger.readNtcStatusReg();
  charger.readFaultStatusReg();
  charger.readCellBalStatReg();
}

void printStatFields(){
  Serial.println();
  Serial.println(F("CHARGE STATS:"));
  Serial.print(F("IINDPM STAT: ")); Serial.println(charger.getIINDPM_STAT());
  Serial.print(F("VINDPM STAT: ")); Serial.println(charger.getVINDPM_STAT());
  Serial.print(F("TREG STAT: ")); Serial.println(charger.getTREG_STAT());
  Serial.print(F("WD STAT: ")); Serial.println(charger.getWD_STAT());
  Serial.print(F("CHRG STAT: ")); Serial.println(charger.getCHRG_STAT());
  Serial.print(F("PG STAT: ")); Serial.println(charger.getPG_STAT());
  Serial.print(F("VBUS STAT: ")); Serial.println(charger.getVBUS_STAT());
  Serial.print(F("ICO STAT: ")); Serial.println(charger.getICO_STAT());

  Serial.println();
  Serial.println(F("NTC STATS:"));
  Serial.print(F("NTC STAT: ")); Serial.println(charger.getNTCStatus());
  
  Serial.println();
  Serial.println(F("FAULT STATS:"));
  Serial.print(F("VBUS OV Fault: ")); Serial.println(charger.getVBUS_OVP_STAT());
  Serial.print(F("TSHUT Fault: ")); Serial.println(charger.getTSHUT_STAT());
  Serial.print(F("Timer Fault: ")); Serial.println(charger.getTMR_STAT());

  Serial.println();
  Serial.println(F("CELL BALANCE STATS:"));
  Serial.print(F("CELL BAL STAT: ")); Serial.println(charger.getCB_STAT());
  Serial.print(F("HIGH SIDE CV: ")); Serial.println(charger.getHS_CV_STAT());
  Serial.print(F("LOW SIDE CV: ")); Serial.println(charger.getLS_CV_STAT());
  Serial.print(F("HIGH SIDE OV: ")); Serial.println(charger.getHS_OV_STAT());
  Serial.print(F("LOW SIDE OV: ")); Serial.println(charger.getLS_OV_STAT());
  Serial.print(F("CELL BAL OC: ")); Serial.println(charger.getCB_OC_STAT());
}

void blinkFaults(){
  charger.readFaultStatusReg();

  if(charger.getVBUS_OVP_STAT()){
    blinkNumber(1);
  }
  
  if(charger.getTSHUT_STAT()){
    blinkNumber(2);
  }

  if(charger.getTMR_STAT()){
    blinkNumber(3);
  }

  blinkFast();
}

void blinkFast(){
  for(int i = 0; i < 10; i++){
    digitalWrite(ERROR_LED, LOW);
    delay(100);
    digitalWrite(ERROR_LED, HIGH);
    delay(100);
  }

  digitalWrite(ERROR_LED, LOW);
}

void blinkNumber(int numberBlinks){
  for(int j = 0; j < 2; j++){
    for(int i = 0; i < numberBlinks; i++){
      digitalWrite(ERROR_LED, LOW);
      delay(1000);
      digitalWrite(ERROR_LED, HIGH);
      delay(1000);
    }
    digitalWrite(ERROR_LED, LOW);
    delay(3000);
  }
}
