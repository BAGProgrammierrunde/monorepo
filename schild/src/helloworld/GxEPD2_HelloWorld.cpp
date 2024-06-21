#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

/*
VCC 	3V3 	Power input (3.3V)
GND 	GND 	Ground
DIN 	P14 	SPI MOSI pin, data input
SCLK 	P13 	SPI CLK pin, clock signal input
CS 	P15 	Chip selection, low active
DC 	P27 	Data/command, low for commands, high for data
RST 	P26 	Reset, low active
BUSY 	P25 	Busy status output pin (means busy) 

// mapping of Waveshare ESP32 Driver Board
// BUSY -> 25, RST -> 26, DC -> 27, CS-> 15, CLK -> 13, DIN -> 14


GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=/ 15, /*DC=/ 27, /*RST=/ 26, /*BUSY=/ 25));
SPIClass hspi(HSPI);
  hspi.begin(13, 12, 14, 15); // remap hspi for EPD (swap pins)

int8_t sck, int8_t miso, int8_t mosi, int8_t ss
sck = 13 CLK
miso = 12
mosi = 14 DIN
ss = 15 CS
*/

/*

  hspi.begin(12, 13, 11, 10);
FSPICS0 = 10   (CS)
FSPID   = 11
FSPICLK = 12   (CLK)
FSPIQ   = 13   (DIN)
FSPIWP  = 14

Anschlüsse:
lila    BUSY= 4
weiß    RST = 6
grün    DC  = 5
orange  CS  = 10  SS  - FSPICS0
gelb    CLK = 12  CLK - FSPICLK
blau    DIN = 11  FSPID
braun   GND = GND
grau    VCC = 3.3V

SPI Naming:
SCLK : SCK, CLK.
MOSI : SIMO, SDO, DO, DOUT, SO, MTSR, PICO, FSPID.
MISO : SOMI, SDI, DI, DIN,  SI, MRST, POCI, FSPIQ.
SS : nCS, CS, CSB, CSN, nSS, STE, SYNC. 
*/
//#define USE_HSPI_FOR_EPD
//SPIClass fspi(FSPI);

//#define PIN_WHITE_LED 7
//#define PIN_BUTTON_READ 8


//GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/ SS, /*DC=*/ 0, /*RST=*/ 6, /*BUSY=*/ 4)); // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)

GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/ 22, /*DC=*/ 0, /*RST=*/ 21, /*BUSY=*/ 19)); // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)

#include <Arduino.h>
#include <SPI.h>
const char HelloWorld[] = "Hello World!";

void setup()
{
  Serial.begin(115000);
  while (!Serial);
  Serial.println("GxEPD2 2.9-inch  e-ink display test start1");

  display.init(115200, true, 2, true); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  Serial.println("Display initialized");

  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  Serial.println("Starting display update");
  do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.print(random(10000000));
  } while (display.nextPage());
  Serial.println("Display update done");


  //pinMode(PIN_WHITE_LED, OUTPUT);
  //pinMode(PIN_BUTTON_READ, INPUT);
}

void loop() {
  //Serial.println("GxEjiäjkoä");
  //esp_deep_sleep(500000000);
  //delay(5000);

  /*if (digitalRead(PIN_BUTTON_READ)) {
    digitalWrite(PIN_WHITE_LED, HIGH);
  } else {
    digitalWrite(PIN_WHITE_LED, LOW);
  }*/
  //delay(20000);
  //Serial.println("GxEPD2 2.9-inch e-ink display test");
};


/*
#include <Arduino.h>
#include <esp_wifi.h>

void setup() {
  Serial.begin(115200);  //etwas strange. bei 10mhz muss serial geschwindigkeit ein vielfaches der "normalen" baud sein. wird dennoch mit 115200 baud ausgelesen!
  setCpuFrequencyMhz(80);
  //Param power unit is 0.25dBm, range is [8, 84] corresponding to 2dBm - 20dBm.
  esp_wifi_set_max_tx_power(40); //40->10dbm!
  // put your setup code here, to run once:
  }
  */
/*
uint32_t Freq = 0;

void loop() {  Freq = getCpuFrequencyMhz();
  Freq = getCpuFrequencyMhz();
  Serial.print("CPU Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
  Freq = getXtalFrequencyMhz();
  Serial.print("XTAL Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
  Freq = getApbFrequency();
  Serial.print("APB Freq = ");
  Serial.print(Freq);
  Serial.println(" Hz");
  esp_deep_sleep(5000000);
}
*/