# ESP01 Control System with MQTT and Android App Integration
📝 Descrição do Projeto
Este projeto utiliza o microcontrolador ESP01 para controle de saídas digitais via protocolo MQTT integrado a um aplicativo Android. A configuração é simples, com suporte a um portal Wi-Fi interativo usando WiFiManager.

Funcionalidades:
✅ Automação de saídas digitais usando shift registers controlados pelo ESP01.

✅ Monitoramento em tempo real do estado das saídas e força do sinal Wi-Fi (RSSI).

✅ Configuração Wi-Fi amigável via portal interativo (WiFiManager).

✅ Integração com App Android para controle remoto e leitura de feedback.

🚀 Recursos
🔌 ESP01 + MQTT
Comunicação segura usando protocolo MQTT em SSL.

Subscrição em tópicos para receber comandos de controle remoto.

Publicação do estado das saídas sob demanda ou em alterações de estado.

Monitoramento da qualidade do sinal Wi-Fi (RSSI) em tempo real.

💡 Shift Register
Ampliação de pinos digitais através de shift registers.

🌐 WiFiManager
Portal interativo para configurar redes Wi-Fi sem necessidade de reprogramar o ESP01.

📱 App Android
Controle remoto das saídas digitais.

Monitoramento do estado das saídas em tempo real.

🛠️ Requisitos
ESP8266 (ESP01).

Broker MQTT (ex.: HiveMQ Cloud).

Bibliotecas:

WiFiManager (tzapu)

PubSubClient

ArduinoJson

App Android (Android 5.0 ou superior).

⚙️ Configuração
1️⃣ Clone este repositório
git clone https://github.com/Talis-dev/esp01_riveMQ_8chRelay.git

2️⃣ Configure as credenciais MQTT
Abra o arquivo principal e configure:

Servidor MQTT (mqttServer)

Porta (mqttPort)

Usuário (mqttUser)

Senha (mqttPassword)

3️⃣ Portal Wi-Fi
Se o ESP01 não encontrar redes Wi-Fi salvas, ele criará uma rede chamada: ESP_ConfigPortal. Conecte-se a ela e configure sua rede Wi-Fi acessando o navegador no endereço 192.168.4.1.

4️⃣ Carregue o Firmware
Use o Arduino IDE para compilar e carregar o código no ESP01.

🔗 Tópicos MQTT
saida/update: Atualiza os estados das saídas digitais.

Payload: JSON contendo bits das saídas. Exemplo: {"0": 1, "1": 0, "2": 1}

saida/state: Publica automaticamente o estado das saídas quando solicitado ou alterado.

Payload: JSON contendo o estado das saídas e a força do sinal Wi-Fi (RSSI). Exemplo: {"0": 1, "1": 0, "2": 1, "RSSI": -60}

saida/request: Solicita o estado atual das saídas. O ESP01 responde publicando no tópico saida/state.

📱 App Android
Funcionalidades:
Controle remoto das saídas: Envio de comandos via MQTT.

Leitura do feedback: Estado das saídas atualizado em tempo real.

Configuração:
Configure o app com os mesmos dados do broker MQTT usados no ESP01.

Servidor MQTT

Porta

Usuário e senha

O app permite um controle intuitivo para automação completa!

🛠️ Estrutura do Código
setup(): Configuração inicial (Wi-Fi, MQTT, e GPIOs).

loop(): Monitoramento da conexão MQTT e processamento de comandos.

mqttCallback(): Processa mensagens recebidas nos tópicos MQTT.

publishState(): Publica o estado atual das saídas no tópico MQTT.

updateShiftRegister(): Atualiza o shift register para refletir os estados desejados.

🛡️ Licença
Este projeto está sob a licença MIT.
