#include <ESP8266WiFi.h>
#include <stdlib.h>

const int led = 2;

char *ssid = "iPhone";
char *senha = "bartira02";

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
              
              String s = "temp";
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
        client.println("<html><body>");
        client.println("<style>");
        client.println("body {");
        client.println("    background-color: dodgerblue;");
        client.println("}");
        client.println("</style>");
        client.println("<h1>Formulário de Exemplo</h1>");
        client.println("<form method='GET' action='http://172.20.10.8/'>");
        client.println("<label for='nome'>Temp:</label>");
        client.println("<input type='number' id='nome' name='temp' required><br>");
        client.println("<label for='nome'>pH:</label>");
        client.println("<input type='number' id='nome' name='pH' required><br>");
        client.println("<label for='nome'>OXP:</label>");
        client.println("<input type='number' id='nome' name='OXP' required><br>");
        client.println("<p>" + (String) TEMPERATURA  + "</p>");
        client.println("<iframe src='https://thingspeak.com/channels/1466041/charts/3?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&title=Temperatura&type=line'></iframe>");
        client.println("<input type='submit' value='Enviar'>");
        client.println("</form>");
        client.println("</body></html>");
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
