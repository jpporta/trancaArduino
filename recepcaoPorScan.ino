void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
        uint8_t c = OemToAscii(mod, key);

        if (c)
                OnKeyPressed(c);
}

/* what to do when symbol arrives */
void KbdRptParser::OnKeyPressed(uint8_t key)
{
        static uint32_t next_time = 0; //watchdog
        static uint8_t current_cursor = 0; //tracks current cursor position

        if ( millis() > next_time ) {
                lcd.clear();
                current_cursor = 0;
                delay( 5 ); //LCD-specific
                lcd.setCursor( 0, 0 );
        }//if( millis() > next_time ...

        next_time = millis() + 200; //reset watchdog

        if ( current_cursor++ == ( DISPLAY_WIDTH + 1 )) { //switch to second line if cursor outside the screen
                lcd.setCursor( 0, 1 );
        }
//-------Recepcao da string, 1 char por passagem---------
        char ch = (char) key;
        //if(ch != ??????){
        //}
        //Serial.println("colocar o que sair na proxima linha na linha 68, se nao tiver nada, apague linhas: 68, e as de Serial");
        //Serial.println(ch);
        string[index] = ch;
        index++;
        check = true;
//---------------------------------
};



/*Usado para reiniciar o precesso sem reutilizar o setup, que nao eh sudavel*/
void restart(){
        Serial.println("Proximo");
        lcd.clear();
        lcd.noAutoscroll();
        lcd.print("Bem Vindo");
}


void resetaString() {
        for(int i = 0; i<30; i++)
                string[i] = '-';
        index = 0;
}

void compara() {
        index++;//index para por o \0
        string[index] = '\0'; //<-- Olha o bixo aqui
        /*Serial.println(string); //DEBUG ONLY*/
        if (comparaStringRecebida())
                abrirPorta();

        else
                acessoInvalido();

        delay(1500);//tempo para ler a menssagem de boas vindas, ou negado
        restart();
}
void acessoInvalido(){ // Mensagem de Acesso Negado, confirmando a nao liberacao
        lcd.print("ACESSO NEGADO");
        aberto = false;
}


/*Funcao para analizar a string recebida pelo leitor e
   comparar com as senhas de todos os funcionarios, indentifica qual
   funcionario que eh, o recepciona com uma menssagem
   e retorna bool, true para liberado, e false para negado*/
bool comparaStringRecebida(){
        char menssagem[20];
        bool liberado = false;
        srtcat(menssagem,"Bem Vindo ");
        for(int i = 0; i < NFUNC; i++) {//para saber quem entrou
                if(strcmp(string, senhas[i]) == 0) {
                        srtcat(menssagem,funcionarios[i]);
                        lcd.print(mensagem);
                        abrirPorta();
                        liberado = true;
                        break;
                }
        }
        return liberado;
}
