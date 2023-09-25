# poc-bem-estar
Poc para demonstrar o uso de sensores e comunicação sem fio

## Descrição 
  O objetivo da poc-bem-estar é  criar uma plataforma web que sugere aos praticantes de atividades físicas em espaços outdoors recomendações de acordo com as condições climáticas medidas por uma rede de sensores instaladas no local Outdoor.
  
  Assim, para a implementação desta poc, foi utilizado o BE900, o rádio utilizado para a comunicação da rede sem fio e antenas omnidirecionais acopladas aos rádios para cobertura do sinal, também foi utilizado o transdutor DHT22 para a coleta de dados de Temperatura (ºC) e Umidade (0% - 100%) no local.Por se tratar de um ambiente externo, usamos baterias para alimentação do sensor, ao passo que o Sensor-base estava acoplado a um computador.

  Para o estabelecimento da comunicação entre a rede sem fio, foi utilizado o protocolo MOT, desenvolvido pelo laboratório WissTek dentro da UNICAMP. Este protocolo tem como objetivo permitir a construção de redes com multiplos saltos, que é a troca de informações entre os sensores. Para isso, são estabelecidos a criação de pacotes que armazenam a informação, ao todo são 52 bytes de informação separados em 5 camadas:
- Física (Phy): Nesta camada são coletadas as informações relativas ao canal e comunicação rádio. 
- Controle de acesso ao meio (MAC): Nesta camada é feito o controle de informações, é possível por exemplo controlar a economia de energia de um sensor, caso necessário.
- Rede (Net): Essa camada é responsável pela criação de campos que conectam os outros nó sensores fora do alcance do nó base e garante a manutenção do roteamento.
- Transporte (Transp): Esta camada é responsável pela troca de informações entre os dispositivos fim a fim.
- Aplicação (Data): De modo geral, a camada data nesta apliação é dedicada a coleta dos dados obtidos pelo sensor, assim, nela são armazenados os dados de temperatura, umidade e RSSI.
## Componentes da Solução 
### Componentes de Hardware
Nó Sensor
- Rádio BE900
  - Microcontrolador ATMEGA 328
  - CC1101
- Transdutor digital DHT22
- Antena omnidirecional
- Placa de alimentação e comunicação
- Baterias para alimentação do nó sensor, 2x (9800mAh, 4.2V)


Nó Base
- Rádio BE900
  - Microcontrolador ATMEGA 328
  - CC1101
- Conversor USB-SERIAL
- Cabo USB
- Alimentação feita pelo cabo USB

Notebook para display
- windows ou linux
  NOTA: O funcionamento da poc-bem-estar no sistema operacional Linux requer uma alteração em /codigo_notebook_display na linha 31, para isso, comente a linha 30 e ative a linha 31. 
### Componentes de Software
Código do nó sensor, ver código em /codigo_no_sensor

Código do nó base, ver código em /codigo_no_base

Código do notebook display, ver código em /codigo_notebook_display


## Autores 
Arthur Alves dos Santos Neto

Caio Moreira de Souza

Heitor Escobar Souza

## Contato
Arthur Alves dos Santos Neto (aas.neto7@gmail.com)

Heitor Escobar Souza

Caio Moreira de Souza
