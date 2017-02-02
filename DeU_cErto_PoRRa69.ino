#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

#include <hid.h>
#include <hiduniversal.h>

#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <hidboot.h>


#define DISPLAY_WIDTH 16
char string[30];//string que recebe o que foi escaneado
int index = 0;//controla a string acima

bool aberto = false;//para saber quando a porta esta destrancada

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

USB          Usb;
USBHub       Hub(&Usb);
HIDUniversal Hid(&Usb);

HIDBoot<HID_PROTOCOL_KEYBOARD>    Keyboard(&Usb);

class KbdRptParser : public KeyboardReportParser
{

    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    virtual void OnKeyDown  (uint8_t mod, uint8_t key);
    virtual void OnKeyPressed(uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

/* what to do when symbol arrives */
void KbdRptParser::OnKeyPressed(uint8_t key)
{
  static uint32_t next_time = 0;      //watchdog
  static uint8_t current_cursor = 0;  //tracks current cursor position

  if ( millis() > next_time ) {
    lcd.clear();
    current_cursor = 0;
    delay( 5 );  //LCD-specific
    lcd.setCursor( 0, 0 );
  }//if( millis() > next_time ...

  next_time = millis() + 200;  //reset watchdog

  if ( current_cursor++ == ( DISPLAY_WIDTH + 1 )) { //switch to second line if cursor outside the screen
    lcd.setCursor( 0, 1 );
  }
//-------Recepcao da string, 1 char por passagem---------
  strinf[index] = (char) key;
  index++;
//---------------------------------
};

KbdRptParser Prs;

void setup()
{
  Serial.begin( 9600 );
  Serial.println("Start");
  pinMode(porta, OUTPUT);
  pinMode(9, OUTPUT);

  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }

  delay( 200 );

  Hid.SetReportParser(0, (HIDReportParser*)&Prs);
  // set up the LCD's number of columns and rows:
  lcd.begin(DISPLAY_WIDTH, 2);
  lcd.clear();
  lcd.noAutoscroll();
  lcd.print("Ovo de Pascoa S2");
  delay( 200 );
}

void loop(){
  Usb.Task();//A cada interacao do loop, e recebido um carcter
  if (index == 8)//se o tam for igual da senha compara
    compara();
  if (aberto) {
    abrirPorta();
  }
}

void compara() {
  index++;//index para por o \0
  strinf[index] = '\0'; //<-- Olha o bixo aqui
  Serial.println(strinf);
  if (strcmp(strinf, "16101416") == 0) {
    lcd.print("Alohomora");
    aberto = true;
  }
  else {
    lcd.print("Colaportus");
    aberto = false;
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
  }
  index = 0;//zera index para preparar para a proxima recepcao
  delay(1000);
  setup();
}

