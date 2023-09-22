// APP : classe da camada de Aplica��o

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

#include "Headers.h"

/**
 * Construtor da camada de Aplica��o.
 */
APP::APP()
{
  dataInitAddress = 2; // Os dados devem começar a ser gravados a partir de pkt->Data[dataInitAddress]
}

/**
 * Inicializa a camada de Aplica��o.
 */
void APP::initialize(void) 
{  
  // Faz com que todos os pinos de IO sejam saida
  pinMode (IO0_PIN, OUTPUT); 
  pinMode (IO1_PIN, OUTPUT); 
  pinMode (IO2_PIN, OUTPUT); 
  pinMode (IO3_PIN, OUTPUT); 
  pinMode (IO4_PIN, OUTPUT); 
  pinMode (IO5_PIN, OUTPUT);
}

/**
 * Envia o pacote para a camada inferior
 */
inline void APP::send(packet * pkt) 
{    
  return;  
}

/**
 * Recebe o pacote da camada inferior
 */
inline void APP::receive(packet * pkt) 
{

  /* Aqui deve ser colocado o codigo da camada de aplicacao */

  // Os dados devem começar a ser gravados a partir de pkt->Data[dataInitAddress] que é setado automaticamente no recebimento do pacote de acordo com o tipo de pacote de rede.
  //------------------------------------Temperatura------------------------------------------------------------

  if (temperatura < 0)
  {
    pkt->Data[dataInitAddress] = 1; // Pode ser utilizado para indicar o tipo de sensor
    temperatura = abs(temperatura);
  }
  else
  {
     pkt->Data[dataInitAddress] = 0; // Pode ser utilizado para indicar o tipo de sensor
  }

  //colocando a temperatura no pacote
  pkt->Data[(dataInitAddress+1)] = (byte) ((int)(temperatura * 10) / 256); // Valor inteiro no byte
  pkt->Data[(dataInitAddress+2)] = (byte) ((int)(temperatura * 10) % 256); // Resto da divis�o noo byte

  //------------------------------------Temperatura minima-----------------------------------------------------------

  if (temperatura_min < 0)
  {
    pkt->Data[(dataInitAddress+3)] = 1; // Pode ser utilizado para indicar o tipo de sensor no byte
    temperatura_min = abs(temperatura_min);
  }
  else
  {
     pkt->Data[(dataInitAddress+3)] = 0; // Pode ser utilizado para indicar o tipo de sensor no byte
  }

  //colocando a temperatura mínima no pacote
  pkt->Data[(dataInitAddress+4)] = (byte) ((int)(temperatura_min * 10) / 256); // Valor inteiro no byte
  pkt->Data[(dataInitAddress+5)] = (byte) ((int)(temperatura_min * 10) % 256); // Resto da divis�o noo byte

  //------------------------------------Temperatura maxima-----------------------------------------------------------

  //colocando a temperatura máxima no pacote
  pkt->Data[(dataInitAddress+6)] = (byte) ((int)(temperatura_max * 10) / 256); // Valor inteiro no byte
  pkt->Data[(dataInitAddress+7)] = (byte) ((int)(temperatura_max * 10) % 256); // Resto da divis�o noo byte

//------------------------------------Umidade-----------------------------------------------------------
  //colocando a umidade no pacote
  pkt->Data[(dataInitAddress+8)] = (byte) ((int)(umidade * 10) / 256); // Valor inteiro no byte
  pkt->Data[(dataInitAddress+9)] = (byte) ((int)(umidade * 10) % 256); // Resto da divis�o noo byte

//------------------------------------Umidade minima-----------------------------------------------------------
  //colocando a umidade mínima no pacote
  pkt->Data[(dataInitAddress+10)] = (byte) ((int)(umidade_min * 10) / 256); // Valor inteiro no byte 34
  pkt->Data[(dataInitAddress+11)] = (byte) ((int)(umidade_min * 10) % 256); // Resto da divis�o noo byte 35

//------------------------------------Umidade maxima-----------------------------------------------------------
  //colocando a umidade máxima no pacote
  pkt->Data[(dataInitAddress+12)] = (byte) ((int)(umidade_max * 10) / 256); // Valor inteiro no byte 36
  pkt->Data[(dataInitAddress+13)] = (byte) ((int)(umidade_max * 10) % 256); // Resto da divis�o noo byte 37

  
  // Envia o pacote para a camada de transporte
  Transp.send(pkt);
  
  return;  
}
 
/* Instanciação do objeto de acesso � classe da camada de Aplicação */
APP App = APP();
