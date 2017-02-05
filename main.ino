#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

#include <hid.h>
#include <hiduniversal.h>

#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <hidboot.h>


#define DISPLAY_WIDTH 16
#define MAX 8
#define NFUNC 10
char string[30];//string que recebe o que foi escaneado
int index = 0;//controla a string acima

bool aberto = false;//para saber quando a porta esta destrancada
bool check = false; //resolver bug de strings menores
const char *senhas[NFUNC][MAX];
senhas[0] = "JRFis7rH";
senhas[1] = "WccN8x8H";
senhas[2] = "5LOWMWqp";
senhas[3] = "RDSZ3qqL";
senhas[4] = "UA78vkL1";
senhas[5] = "DyX5GoOF";
senhas[6] = "I4heQdNU";
senhas[7] = "J6IfQhv9";
senhas[8] = "y18Bhn0x";
senhas[9] = "XNnzJQyy";

const char *funcionarios[NFUNC][MAX];
funcionarios[0] = "Augusto";
funcionarios[1] = "Bruno";
funcionarios[2] = "Jansey";
funcionarios[3] = "Dib";
funcionarios[4] = "Paulo";
funcionarios[5] = "Porta";
funcionarios[6] = "Rafael";
funcionarios[7] = "Kitaka";
funcionarios[8] = "Armando";
funcionarios[9] = "Pedro";
// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);

HIDBoot<HID_PROTOCOL_KEYBOARD>    Keyboard(&Usb);

class KbdRptParser : public KeyboardReportParser
{

void PrintKey(uint8_t mod, uint8_t key);

protected:
virtual void OnKeyDown  (uint8_t mod, uint8_t key);
virtual void OnKeyPressed(uint8_t key);
};
//*===================================================
//*Declaracao de Funcoes
//*
//*AcoesPorta:
void abrirPorta();
void fecharPorta();
//*recepcaoPorScan:
void restart();
void resetaString();
void compara();
void acessoInvalido();
bool comparaStringRecebida();
//*===================================================

KbdRptParser Prs;

void setup()
{
        resetaString();
        Serial.begin( 9600 );
        Serial.println("Start");
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
        lcd.print("Bem Vindo");
        delay( 200 );
}

void loop(){
        Usb.Task();//A cada interacao do loop, e recebido um carcter
        if(check) {
                check = false;
                for(int i = 0; i<MAX; i++) {
                        if(string[i] == '-') {
                                check = true;
                                break;
                        }
                }
                if(!check) {
                        compara();
                }
                else{
                        acessoInvalido();
                }
                resetaString();
                check = false;
        }
}
