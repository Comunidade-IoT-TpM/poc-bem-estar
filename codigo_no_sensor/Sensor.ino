// Radiuino_Sensor : firmware para os nos Sensores da rede

// Mais informacoes em www.radiuino.cc
// Copyright (c) 2015
// Author: Pedro Henrique Gomes, Omar C. Branquinho, Tiago T. Ganselli e Debora M Ferreira, Guilherme Lopes da Silva, Raphael Montali de Assumpçao.
// Versao 2.3: 18/05/2017

// Este arquivo e parte da plataforma Radiuino
// Este programa e um software livre; voce pode redistribui-lo e/ou modifica-lo dentro dos termos da Licenca Publica Geral Menor GNU
// como publicada pela Fundacao do Software Livre (FSF); na versao 2 da Licenca, ou (na sua opniao) qualquer futura versao.
// Este programa e distribuido na esperanca que possa ser  util, mas SEM NENHUMA GARANTIA; sem uma garantia implicita
// de ADEQUACAO a qualquer MERCADO ou APLICACAO EM PARTICULAR. Veja a Licenca Publica Geral Menor GNU para maiores detalhes.
// Voce deve ter recebido uma copia da Licenca Publica Geral Menor GNU junto com este programa, se nao, escreva para a Fundacao
// do Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. This library
// is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. You should have received a copy
// of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St,
// Fifth Floor, Boston, MA  02110-1301  USA

#include <RADIUINO.h>
#include <EEPROM.h>
#include <SPI.h>
#include <DHT.h>



#include "Headers.h"

#define FIRMWARE_VERSION   1.0     /* Versao 1.0 Radiuino */

byte int_rx = 0;                   /* Inicializacao da interrupcao de recepcao de pacotes - O hardware gera uma interrup��o no come�o que nao deve ser tratada */
byte int_buff = 0;                 /* Inicializacao da interrupcao de buffer overflow - O hardware gera uma interrupcao no come�o que nao deve ser tratada */


#define DHTPIN IO3_PIN //ligado na porta D7 da DK105

// Utilize a linha de acordo com o modelo do sensor
//#define DHTTYPE DHT11   // Sensor DHT11
#define DHTTYPE DHT22   // Sensor DHT 22  (AM2302)
//#define DHTTYPE DHT21   // Sensor DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);


float temp, temperatura, temperatura_min, temperatura_max, umid, umidade, umidade_min, umidade_max; //Variaveis para armazenar temperatura e umidade

int maximoT = -200;
int minimoT = 1000;
int maximoU = -200;
int minimoU = 1000;


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 2000;           // interval at which to blink (milliseconds)

/**
   Configura o Arduino. Executado uma unica vez no inicio do firmware.
*/
void setup()
{
  pinMode(IO1_PIN, OUTPUT); //eh necessario para as DKs 106 e 107
  digitalWrite(IO1_PIN, LOW); // DK106
  //digitalWrite(IO1_PIN, HIGH); // DK107


  pinMode(IO3_PIN, INPUT); //configurando o DH11 como entrada
  dht.begin();

  /* Inicializacao da camada F�sica */
  Phy.initialize();

  /* Inicializacao da camada de Controle de Acesso ao Meio */
  Mac.initialize();

  /* Inicializacao da camada de Rede */
  Net.initialize();

  /* Inicializacao da camada de Transporte */
  Transp.initialize();

  /* Inicializacao da camada de Aplicacao */
  App.initialize();

  /* Anexa as funcoes de interrupcao de recepcao de pacotes e de estouro de buffer de recepcao */
  attachInterrupt(0, IntReceiveData, RISING);
  attachInterrupt(1, IntBufferOverflow, RISING);
  pinMode(GDO0, INPUT);

  /* Inicializa o Timer1 e configura o periodo para 1 segundo */
  Timer1.initialize(1000000);

  /* Anexa a funcao de interrupcao de estouro do Timer1 */
  Timer1.attachInterrupt(IntTimer1);

  /* Escreve mensagem de inicializacao */
  //Serial.print("Radiuino! Sensor");

  /*Configura os pinos PD4 e PD6 como saída*/
  pinMode(IO0_PIN, OUTPUT);
  pinMode(IO2_PIN, OUTPUT);

  /*Aciona os LEDs que geralmente são ligados aos pinos PD4 e PD6*/
  digitalWrite(IO0_PIN, HIGH);
  digitalWrite(IO2_PIN, HIGH);


}

/**
   Laco de execucao do Arduino. Executado continuamente.
*/
void loop()
{
  /* Verifica se deve entrar em modo Sleep */
  verifySleepEntering();

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) { //Leitura
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    umid = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temp) || isnan(umid)) {
      //g_pkt.Data[0] = 1; //problema de medida
      //digitalWrite(IO3_PIN, LOW);
      return;
    }
    else {

      temperatura = temp; //atualiza a variavel temperatura
      umidade = umid; // atualiza a variavel umidade
      
      if (temperatura > maximoT) {
        maximoT = temperatura;
      }

      if (temperatura < minimoT) {
        minimoT = temperatura;
      }

      if (umidade > maximoU) {
        maximoU = umidade;
      }

      if (temperatura < minimoU) {
        minimoU = umidade;
      }

      temperatura_min = minimoT;  // calcula o valor mínimo de temperatura
      temperatura_max = maximoT; // calcula o valor máximo de temperatura

      umidade_min = minimoU; // calcula o valor mínimo de Umidade
      umidade_max = maximoT; // calcula o valor mínimo de Umidade
    }
    

    

    
  }

}

/**
   Trata o pacote recebido pela rede sem fio.
*/
void IntReceiveData()
{

  /* Se for a primeira vez que a interrupcao � executada */
  if (int_rx == 0)
  {
    int_rx = 1;
    return;
  }

  /* Reenvia toda informacao recebida da rede para o PC */
  if ( digitalRead(GDO0) == HIGH )
  {
    /* Recebe os dados do RF */
    Phy.receive(&g_pkt);
  }

  return;
}

/**
   Trata o estouro do buffer de recepcao.
*/
void IntBufferOverflow()
{
  /* Se for a primeira vez que a interrupcao � executada */
  if (int_buff == 0) {
    int_buff = 1;
    return;
  }

  /* Esvazia o buffer de recepcao e vai para o estado de RX */
  cc1101.Strobe(CC1101_SFRX);
  cc1101.Strobe(CC1101_SRX);

  return;
}

/**
   Trata o estouro do per�odo do Timer1.
*/
void IntTimer1()
{
  /* Aqui podem ser colocadas tarefas peri�dicas. O per�odo do Timer1 � de 1 segundo por padr�o.
     Mas pode ser mudado no setup() */

  return;
}

/**
   Realiza uma operacao Atomica para verificar se deve entrar no modo Sleep ou se deve sair do modo Sleep
*/
void verifySleepEntering ( void ) {
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    if (Mac.time_to_sleep > 0) {

      /* Aguarda o fim da tranmissao do ACK SLEEP */
      delayMicroseconds(500);
      while (Phy.txFifoFree() != CC1101_FIFO_SIZE);

      /* Entra em modo Sleep */
      Phy.lowPowerOn();

    }
    else if (Mac.time_to_sleep == 0) {

      /* Habilita o conversor AD */
      _SFR_BYTE(ADCSRA) |= _BV(ADEN);

      /* Saindo do modo de Sleep */
      cc1101.Strobe(CC1101_SIDLE);
      delay(1);
      cc1101.Strobe(CC1101_SFTX);
      cc1101.Strobe(CC1101_SFRX);
      cc1101.Strobe(CC1101_SRX);

      Mac.time_to_sleep = -1;
      Mac.last_wakeup = millis();
    }

    if ((Mac.flag_sleep2) && (Mac.time_to_sleep < 0)) {
      if ( (millis() - Mac.last_wakeup) >= Mac.t2) {
        Mac.time_to_sleep = Mac.t3;
      }
    }
  }
}

/**
   Interrupcao de WatchDog. Executada sempre que o contador de WatchDog estoura
*/
ISR(WDT_vect) {

  /* Verifica se precisamos decrementar o contador de Sleep */
  if (Mac.time_to_sleep > 0) {
    Mac.time_to_sleep--;
    return;
  }

}
