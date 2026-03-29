//Autor: Fábio Henrique Cabrini
//Resumo: Esse programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT possibilitando o Helix saber
//se o led está ligado ou desligado.
//Revisões:
//Rev1: 26-08-2023 Código portado para o ESP32 e para realizar a leitura de luminosidade e publicar o valor em um tópico aprorpiado do broker 
//Autor Rev1: Lucas Demetrius Augusto 
//Rev2: 28-08-2023 Ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev2: Fábio Henrique Cabrini
//Rev3: 1-11-2023 Refinamento do código e ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev3: Fábio Henrique Cabrini

/*
Autor dos comentários adicionais: Lara Mofid Essa Alsaabak, Maria Luisa Boucinhas Franco, Roberta Moreira dos Santos
Descrição: Comentários explicativos adicionados para melhor entendimento do funcionamento do código,
sem alterar a lógica original.
*/

#include <WiFi.h>
#include <PubSubClient.h>

/*
Autor: Maria Luisa Boucinhas Franco
Descrição: Definições padrão de configuração de rede, MQTT e hardware.
Esses valores podem ser alterados conforme necessidade do usuário.
*/

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "20.163.23.245"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_001"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard

/*
Autor: Lara Mofid Essa Alsaabak
Descrição: Prefixo utilizado para montar os comandos recebidos via MQTT.
*/
const char* topicPrefix = "lamp001";

/*
Autor: Roberta Moreira dos Santos
Descrição: Variáveis que recebem os valores padrão e permitem modificações em tempo de execução.
*/
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

/*
Autor: Roberta Moreira dos Santos
Descrição: Inicialização dos clientes WiFi e MQTT.
*/
WiFiClient espClient;
PubSubClient MQTT(espClient);

/*
Autor: Roberta Moreira dos Santos
Descrição: Variável que armazena o estado atual do LED ('0' desligado, '1' ligado).
*/
char EstadoSaida = '0';

/*
Autor: Maria Luisa Boucinhas Franco
Descrição: Inicializa a comunicação serial para debug.
*/
void initSerial() {
    Serial.begin(115200);
}

/*
Autor: Maria Luisa Boucinhas Franco
Descrição: Inicializa a conexão Wi-Fi chamando a função de reconexão.
*/
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

/*
Autor: Maria Luisa Boucinhas Franco
Descrição: Configura o servidor MQTT e define a função callback.
*/
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

/*
Autor: Lara Mofid Essa Alsaabak
Descrição: Função principal de configuração executada uma vez no boot.
*/
void setup() {
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}

/*
Autor: Lara Mofid Essa Alsaabak
Descrição: Loop principal executado continuamente.
Gerencia conexões, envia estado e leitura de luminosidade.
*/
void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    handleLuminosity();
    MQTT.loop();
}

/*
Autor: Roberta Moreira dos Santos
Descrição: Responsável por conectar ou reconectar ao Wi-Fi.
*/
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}

/*
Autor: Lara Mofid Essa Alsaabak
Descrição: Callback chamado quando uma mensagem MQTT é recebida.
Responsável por interpretar comandos e controlar o LED.
*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;

    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }

    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}

/*
Autor: Maria Luisa Boucinhas Franco
Descrição: Verifica e mantém conexões Wi-Fi e MQTT ativas.
*/
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();

    reconectWiFi();
}

/*
Autor: Lara Mofid Essa Alsaabak
Descrição: Publica o estado atual do LED no broker MQTT.
*/
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }

    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}

/*
Autor: Lara Mofid Essa Alsaabak
Descrição: Inicializa o pino do LED e faz um efeito visual de inicialização.
*/
void InitOutput() {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);

    boolean toggle = false;

    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

/*
Autor: Maria Luisa Boucinhas Franco
Descrição: Responsável por reconectar ao broker MQTT caso a conexão seja perdida.
*/
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);

        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

/*
Autor: Roberta Moreira dos Santos
Descrição: Realiza a leitura de um sensor analógico (potenciômetro ou LDR),
converte o valor para porcentagem e publica no MQTT.
*/
void handleLuminosity() {
    const int potPin = 34;

    int sensorValue = analogRead(potPin);

    int luminosity = map(sensorValue, 0, 4095, 0, 100);

    String mensagem = String(luminosity);

    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());

    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}