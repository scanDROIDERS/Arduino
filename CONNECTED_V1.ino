#include <ESP8266WiFi.h>
#include <stdlib.h>

const int led = 2;

char *ssid = "tomh"; //iPhone //tomh //CLARO_2G6469F3
char *senha = "44556677"; //bartira02 //44556677 // C96469F3

WiFiServer server(80); // Define o servidor na porta 80

std::vector<String> splitString(const String& input, char delimiter) {
  std::vector<String> result;
  String substring;
  int lastIndex = 0;
  int delimiterIndex;

  while ((delimiterIndex = input.indexOf(delimiter, lastIndex)) != -1) {
    substring = input.substring(lastIndex, delimiterIndex);
    result.push_back(substring);
    lastIndex = delimiterIndex + 1;
  }

  substring = input.substring(lastIndex);
  result.push_back(substring);

  return result;
}


void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(led, OUTPUT);

  // Conecta-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexão WiFi estabelecida!");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Novo cliente conectado!");
    int contentLength;
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        //VARIAVEIS
        float TEMPERATURA = 0.0f;

        
        Serial.println("GET DOS GET: " + request);
        int paramStart = request.indexOf('?');
        if (paramStart != -1) {
          String params = request.substring(paramStart + 1);
          Serial.println("Parâmetros: " + params);
    
          // Separar os parâmetros com base no caractere "&"
          std::vector<String> paramList = splitString(params, '&');
    
          // Iterar sobre os parâmetros e separar o nome e o valor
          for (const String& param : paramList) {
            std::vector<String> keyValue = splitString(param, '=');
            if (keyValue.size() == 2) {
              String paramName = keyValue[0];
              String paramValue = keyValue[1];
              
              String s = "temp_max";
              if (paramName == s) {
                TEMPERATURA = atof(paramValue.c_str());
                Serial.println("Temp: " + (String) TEMPERATURA);
                compare(TEMPERATURA);
              }
              Serial.println("Nome: " + paramName);
              Serial.println("Valor: " + paramValue);
            }
          }
        }

        // Envia uma resposta ao cliente
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("");
        client.println("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>Projeto Piscicultura</title></head><style>body{font-family:Arial,sans-serif;background-color:#f4f4f4;padding:20px;}h1{text-align:center;margin-bottom:30px;}form{max-width:500px;margin:0 auto;background-color:#ffffff;border-radius:5px;padding:30px;box-shadow:0 2px 5px rgba(0,0,0,0.1);}label{display:block;margin-bottom:10px;font-weight:bold;}input[type='text']{width:100%;padding:10px;border-radius:5px;border:1px solid #ccc;box-sizing:border-box;margin-bottom:10px;}input[type='submit']{background-color:#14b8a6;color:#ffffff;border:none;padding:10px 20px;border-radius:5px;cursor:pointer;font-size:16px;}input[type='submit']:hover{background-color:#45a049;}.category{margin-bottom:20px;}@media only screen and (max-width:600px){form{padding:20px;}}</style><body><h1>Projeto Piscicultura</h1><form method='GET' action='"+ WiFi.localIP().toString() +"'><div class='category'><label for='ph'>pH:</label><input type='text' id='ph_min' name='ph_min' placeholder='Mínimo' step='0.01' required><input type='text' id='ph_max' name='ph_max' placeholder='Máximo' step='0.01' required></div><label for='salinidade'>Salinidade:</label><input type='text' id='salinidade_min' name='salinidade_min' placeholder='Mínimo' step='0.01' required><input type='text' id='salinidade_max' name='salinidade_max' placeholder='Máximo' step='0.01' required></div><div class='category'><label for='o2'>O2:</label><input type='text' id='o2_min' name='o2_min' placeholder='Mínimo' step='0.01' required><input type='text' id='o2_max' name='o2_max' placeholder='Máximo' step='0.01' required></div><div class='category'><label for='cond'>Condutividade (%):</label><input type='text' id='cond_min' name='cond_min' placeholder='Mínimo' step='0.01' required><input type='text' id='cond_max' name='cond_max' placeholder='Máximo' step='0.01' required></div><div class='category'><label for='temp'>Temperatura:</label><input type='text' id='temp_min' name='temp_min' placeholder='Mínimo' step='0.01' required><input type='text' id='temp_max' name='temp_max' placeholder='Máximo' step='0.01' required></div><div class='category'><label for='oxi'>Oxigênio:</label><input type='text' id='oxi_min' name='oxi_min' placeholder='Mínimo' step='0.01' required><input type='text' id='oxi_max' name='oxi_max' placeholder='Máximo' step='0.01' required></div><input type='submit' value='Enviar'></form></body></html>");
        break;
      }
    }

    delay(1);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void compare(float value) {
  if (value > 0) {
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
}
