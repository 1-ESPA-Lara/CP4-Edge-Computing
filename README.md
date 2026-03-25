# Smart Lamp – Controle e Monitoramento com FIWARE

## Descrição do projeto
Projeto acadêmico desenvolvido na FIAP com foco em Internet das Coisas (IoT). O sistema consiste em uma Smart Lamp controlada remotamente por meio de uma máquina digital utilizando FIWARE e protocolo MQTT.

Além do controle de ligar e desligar a lâmpada, o projeto também realiza a leitura da luminosidade ambiente por meio de um sensor analógico conectado ao ESP32, enviando essas informações para a plataforma.

---

## Visão geral do projeto

### Simulação no Wokwi
Link: https://wokwi.com/projects/459286243218051073

### Foto do simulador
<img width="313" height="310" alt="image" src="https://github.com/user-attachments/assets/de47c76f-7425-4837-b072-e7ce8fbbde7d" />


### Arquitetura do projeto
![Arquitetura do Projeto](./img/arquitetura-smart-lamp.png)

---

## Funcionalidades
- Controle remoto da lâmpada via MQTT
- Leitura da luminosidade em tempo real
- Envio do estado da lâmpada para o broker
- Integração com FIWARE
- Monitoramento do ambiente por sensor analógico

---

## Conceitos envolvidos
- Internet das Coisas (IoT)
- ESP32
- MQTT
- FIWARE
- Sensoriamento analógico
- Comunicação entre dispositivo físico e plataforma digital

---

## Como reproduzir o projeto
1. Monte o circuito conforme o diagrama do simulador ou imagem do repositório.
2. Abra o código na Arduino IDE.
3. Instale as bibliotecas necessárias:
   - `WiFi.h`
   - `PubSubClient.h`
4. Faça o upload do código para o ESP32.
5. Conecte o dispositivo à rede Wi-Fi configurada no código.
6. Inicie a comunicação com o broker MQTT.
7. Envie comandos para ligar ou desligar a lâmpada e acompanhe os valores de luminosidade publicados.

---

## Componentes utilizados
- ESP32
- LED ou lâmpada simulada
- Sensor de luminosidade analógico
- Protoboard
- Jumpers
- Broker MQTT
- Plataforma FIWARE

---

## Tópicos MQTT utilizados
- **Subscribe:** `/TEF/lamp001/cmd`
- **Publish estado:** `/TEF/lamp001/attrs`
- **Publish luminosidade:** `/TEF/lamp001/attrs/l`

---

## Equipe de Desenvolvimento
- **Lara Mofid Essa Alssabak** — RM567947  
- **Maria Luisa Boucinhos Franco** — RM567355  
- **Roberta Moreira dos Santos** — RM567825
