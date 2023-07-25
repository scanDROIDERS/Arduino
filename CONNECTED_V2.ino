#include <ESP8266WiFi.h>
#include <stdlib.h>
#include <bits/stdc++.h>

const int led = 2;

char *ssid = "CLARO_2G6469F3"; //iPhone //tomh //CLARO_2G6469F3 //Beatles-Sala
char *senha = "C96469F3"; //bartira02 //44556677 // C96469F3 //Verdao1914

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
        float temp_min = 15;
        float temp_max = 30;

        float ph_min = 5;
        float ph_max = 8;

        float orp_min = 200;
        float orp_max = 600;

        float do_min = 5;
        float do_max = 20;

        float sal_min = 0;
        float sal_max = 42;

        float cond_min = 15000;
        float cond_max = 25000;

        
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

              //IF's
              if (paramName == "temp_min") {
                  temp_min = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) temp_min);
                  compare(temp_min);
              } else if (paramName == "temp_max") {
                  temp_max = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) temp_max);
                  compare(temp_max);
              } else if (paramName == "ph_min") {
                  ph_min = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) ph_min);
                  compare(ph_min);
              } else if (paramName == "ph_max") {
                  ph_max = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) ph_max);
                  compare(ph_max);
              } else if (paramName == "o2_min") {
                  orp_min = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) orp_min);
                  compare(orp_min);
              } else if (paramName == "o2_max") {
                  orp_max = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) orp_max);
                  compare(orp_max);
              } else if (paramName == "ox_max") {
                  do_max = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) do_max);
                  compare(do_max);
              } else if (paramName == "ox_min") {
                  do_min = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) do_min);
                  compare(do_min);
              } else if (paramName == "sal_min") {
                  sal_min = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) sal_min);
                  compare(sal_min);
              } else if (paramName == "sal_max") {
                  sal_max = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) sal_max);
                  compare(sal_max);
              } else if (paramName == "cond_max") {
                  cond_max = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) cond_max);
                  compare(cond_max);
              }  else if (paramName == "cond_min") {
                  cond_min = atof(paramValue.c_str());
                  Serial.println("Temp: " + (String) cond_min);
                  compare(cond_min);
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
        //client.println("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>Projeto Piscicultura</title></head><style>body{font-family:Arial,sans-serif;background-color:#f4f4f4;padding:20px;}h1{text-align:center;margin-bottom:30px;}form{max-width:500px;margin:0 auto;background-color:#ffffff;border-radius:5px;padding:30px;box-shadow:0 2px 5px rgba(0,0,0,0.1);}label{display:block;margin-bottom:10px;font-weight:bold;}input[type='text']{width:100%;padding:10px;border-radius:5px;border:1px solid #ccc;box-sizing:border-box;margin-bottom:10px;}input[type='submit']{background-color:#14b8a6;color:#ffffff;border:none;padding:10px 20px;border-radius:5px;cursor:pointer;font-size:16px;}input[type='submit']:hover{background-color:#45a049;}.category{margin-bottom:20px;}@media only screen and (max-width:600px){form{padding:20px;}}</style><body><h1>Projeto Piscicultura</h1><form method='GET' action='"+ WiFi.localIP().toString() +"'><div class='category'><label for='ph'>pH:</label><input type='text' id='ph_min' name='ph_min' placeholder='Mínimo' step='0.01' required><input type='text' id='ph_max' name='ph_max' placeholder='Máximo' step='0.01' required></div><label for='salinidade'>Salinidade:</label");
        //client.println("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>body{font-family:Arial,sans-serif;background-color:#ffffff;margin:0;padding:0;color:#333333}.navbar{background-color:#00979C;padding:10px;display:flex;align-items:center}.navbar-brand{color:#ffffff;font-size:24px;font-weight:bold;text-decoration:none;margin-left:20px;flex-grow:1}.navbar-nav{display:flex;list-style:none;margin:0;padding:0;justify-content:flex-start;flex-grow:1.7}.nav-item{margin-left:10px}.nav-item a{color:#ffffff;text-decoration:none;padding:8px 12px;border-radius:4px;transition:background-color 0.3s ease}.nav-item a:hover{background-color:#007d81}.container{max-width:500px;margin:0 auto;padding:20px;background-color:#f5f5f5;box-shadow:0 2px 5px rgba(0,0,0,0.1)}h1{text-align:center;margin-bottom:30px;color:#00979C}label{display:block;margin-bottom:10px;font-weight:bold}select,input[type='number']{width:100%;padding:10px;margin-bottom:20px;border:1px solid #ccc;border-radius:4px;box-sizing:border-box;font-size:16px}input[type='submit']{width:100%;padding:10px;background-color:#00979C;color:#ffffff;border:none;border-radius:4px;cursor:pointer;font-size:16px}input[type='submit']:hover{background-color:#007d81}.chart-area{display:flex;flex-wrap:wrap;gap:20px;justify-content:center;overflow:hidden}.chart-item{width:400px;height:300px;position:relative}.chart-item iframe{position:absolute;top:0;left:0;width:106.5%;height:100%;aspect-ratio:16/9;border:none}</style></head><body><div class='navbar'><a href='#' class='navbar-brand'>Projeto Piscicultura</a><ul class='navbar-nav'><li class='nav-item'><a href='#graficos'>Gráficos</a></li></ul></div><div class='container'><form methid='GET' action='http://177.34.16.99:4000'><label for='modo'>Modo:</label><select id='modo' name='modo'><option value='manual'>Modo Manual</option><option value='automatico'>Modo Automático</option></select><label for='ph_min'>pH mínimo:</label><input type='number' id='ph_min' name='ph_min' placeholder='Informe o pH mínimo'><label for='ph_max'>pH máximo:</label><input type='number' id='ph_max' name='ph_max' placeholder='Informe o pH máximo'><label for='sal_min'>Salinidade mínima:</label><input type='number' id='sal_min' name='sal_min' placeholder='Informe a salinidade mínima'><label for='sal_max'>Salinidade máxima:</label><input type='number' id='sal_max' name='sal_max' placeholder='Informe a salinidade máxima'><label for='o2_min'>O2 mínimo:</label><input type='number' id='o2_min' name='o2_min' placeholder='Informe o nível de oxigênio mínimo'><label for='o2_max'>O2 máximo:</label><input type='number' id='o2_max' name='o2_max' placeholder='Informe o nível de oxigênio máximo'><label for='cond_min'>Condutividade mínima (%):</label><input type='number' id='cond_min' name='cond_min' placeholder='Informe a condutividade mínima em %'><label for='cond_max'>Condutividade máxima (%):</label><input type='number' id='cond_max' name='cond_max' placeholder='Informe a condutividade máxima em %'><label for='temp_min'>Temperatura mínima:</label><input type='number' id='temp_min' name='temp_min' placeholder='Informe a temperatura mínima'><label for='temp_max'>Temperatura máxima:</label><input type='number' id='temp_max' name='temp_max' placeholder='Informe a temperatura máxima'><label for='ox_min'>Oxigênio mínimo:</label><input type='number' id='ox_min' name='ox_min' placeholder='Informe o nível de oxigênio dissolvido mínimo'><label for='ox_max'>Oxigênio máximo:</label><input type='number' id='ox_max' name='ox_max' placeholder='Informe o nível de oxigênio dissolvido máximo'><input type='submit' value='Enviar'></form></div><h2 id='graficos'>Gráficos do ThingSpeak</h2><div class='chart-area'><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/1?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Temperatura&type=line&xaxis=Tempo&yaxis=Graus' scrolling='yes'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/2?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=pH&type=line&xaxis=Tempo&yaxis=pH' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/3?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Oxigenio+Dissolvido&type=line&xaxis=Tempo&yaxis=Mg%2FL' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/4?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=ORP&type=line&xaxis=Tempo&yaxis=mV' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/5?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Condutivdade&type=line&xaxis=Tempo&yaxis=uS%2FL' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/6?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Salinidade&type=line&xaxis=Tempo&yaxis=ppt&yaxismax=42&yaxismin=0' scrolling='no'></iframe></div></div></body></html>");
        //client.println("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>*{scroll-behavior:smooth;}body{font-family:Arial,sans-serif;background-color:#ffffff;margin:0;padding:0;color:#333333;}.navbar{background-color:#00979C;padding:10px;display:flex;align-items:center;}.navbar-brand{color:#ffffff;font-size:24px;font-weight:bold;text-decoration:none;margin-left:20px;flex-grow:1;}.navbar-nav{display:flex;list-style:none;margin:0;padding:0;justify-content:flex-start;flex-grow:1.7;}.nav-item{margin-left:10px;}.nav-item a{color:#ffffff;text-decoration:none;padding:8px 12px;border-radius:4px;transition:background-color 0.3s ease;}.nav-item a:hover{background-color:#007d81;}.container{max-width:500px;margin:0 auto;padding:20px;background-color:#f5f5f5;box-shadow:0 2px 5px rgba(0,0,0,0.1);}select,.input-number{width:100%;padding:10px;margin-bottom:20px;border:1px solid #ccc;border-radius:4px;box-sizing:border-box;font-size:16px;}.input-submit{width:100%;padding:10px;background-color:#00979C;color:#ffffff;border:none;border-radius:4px;cursor:pointer;font-size:16px;}.input-submit:hover{background-color:#007d81;}.chart-area{display:flex;flex-wrap:wrap;gap:20px;justify-content:center;overflow:hidden;}.chart-item{width:400px;height:300px;position:relative;}.chart-item iframe{position:absolute;top:0;left:0;width:106.5%;height:100%;aspect-ratio:16/9;border:none;}</style></head><body><div class='navbar'><p class='navbar-brand'>Projeto Piscicultura</p></div><div class='container'><form method='GET' action='http://191.191.248.217:8080'><label for='modo'>Modo:</label><select id='modo' name='modo'><option value='automatico'>Modo Automático</option><option value='manual'>Modo Manual</option></select><label for='ph_min'>pH mínimo:</label><input type='text' id='ph_min' name='ph_min' class='input-number' placeholder='Informe o pH mínimo'><label for='ph_max'>pH máximo:</label><input type='text' id='ph_max' name='ph_max' class='input-number' placeholder='Informe o pH máximo'><label for='sal_min'>Salinidade mínima:</label><input type='text' id='sal_min' name='sal_min' class='input-number' placeholder='Informe a salinidade mínima'><label for='sal_max'>Salinidade máxima:</label><input type='text' id='sal_max' name='sal_max' class='input-number' placeholder='Informe a salinidade máxima'><label for='o2_min'>O2 mínimo:</label><input type='text' id='o2_min' name='o2_min' class='input-number' placeholder='Informe o nível de oxigênio mínimo'><label for='o2_max'>O2 máximo:</label><input type='text' id='o2_max' name='o2_max' class='input-number' placeholder='Informe o nível de oxigênio máximo'><label for='cond_min'>Condutividade mínima (%):</label><input type='text' id='cond_min' name='cond_min' class='input-number' placeholder='Informe a condutividade mínima em %'><label for='cond_max'>Condutividade máxima (%):</label><input type='text' id='cond_max' name='cond_max' class='input-number' placeholder='Informe a condutividade máxima em %'><label for='temp_min'>Temperatura mínima:</label><input type='text' id='temp_min' name='temp_min' class='input-number' placeholder='Informe a temperatura mínima'><label for='temp_max'>Temperatura máxima:</label><input type='text' id='temp_max' name='temp_max' class='input-number' placeholder='Informe a temperatura máxima'><label for='ox_min'>Oxigênio mínimo:</label><input type='text' id='ox_min' name='ox_min' class='input-number' placeholder='Informe o nível de oxigênio dissolvido mínimo'><label for='ox_max'>Oxigênio máximo:</label><input type='text' id='ox_max' name='ox_max' class='input-number' placeholder='Informe o nível de oxigênio dissolvido máximo'><input type='submit' value='Enviar' class='input-submit'></form></div><h2 id='graficos'>Gráficos do ThingSpeak</h2><div class='chart-area'><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/1?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Temperatura&type=line&xaxis=Tempo&yaxis=Graus' scrolling='yes'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/2?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=pH&type=line&xaxis=Tempo&yaxis=pH' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/3?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Oxigenio+Dissolvido&type=line&xaxis=Tempo&yaxis=Mg%2FL' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/4?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=ORP&type=line&xaxis=Tempo&yaxis=mV' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/5?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Condutivdade&type=line&xaxis=Tempo&yaxis=uS%2FL' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/6?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Salinidade&type=line&xaxis=Tempo&yaxis=ppt&yaxismax=42&yaxismin=0' scrolling='no'></iframe></div></div></body><script>const modoSelect=document.getElementById('modo');const inputs=Array.from(document.querySelectorAll('.input-number'));modoSelect.value='automatico';if(modoSelect.value==='automatico'){inputs.forEach(input=>{input.value='15';input.removeAttribute('required');});}modoSelect.addEventListener('change',function(){const modo=modoSelect.value;if(modo==='automatico'){inputs.forEach(input=>{input.value='15';input.removeAttribute('required');});}else if(modo==='manual'){inputs.forEach(input=>{input.value='';input.setAttribute('required',true);});}});</script></html>");
        client.println("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>*{scroll-behavior:smooth;}body{font-family:Arial,sans-serif;background-color:#ffffff;margin:0;padding:0;color:#333333;}.navbar{background-color:#00979C;padding:10px;display:flex;align-items:center;}.navbar-brand{color:#ffffff;font-size:24px;font-weight:bold;text-decoration:none;margin-left:20px;flex-grow:1;}.navbar-nav{display:flex;list-style:none;margin:0;padding:0;justify-content:flex-start;flex-grow:1.7;}.nav-item{margin-left:10px;}.nav-item a{color:#ffffff;text-decoration:none;padding:8px 12px;border-radius:4px;transition:background-color 0.3s ease;}.nav-item a:hover{background-color:#007d81;}.container{max-width:500px;margin:0 auto;padding:20px;background-color:#f5f5f5;box-shadow:0 2px 5px rgba(0,0,0,0.1);}select,.input-number{width:100%;padding:10px;margin-bottom:20px;border:1px solid #ccc;border-radius:4px;box-sizing:border-box;font-size:16px;}.input-submit{width:100%;padding:10px;background-color:#00979C;color:#ffffff;border:none;border-radius:4px;cursor:pointer;font-size:16px;}.input-submit:hover{background-color:#007d81;}.chart-area{display:flex;flex-wrap:wrap;gap:20px;justify-content:center;overflow:hidden;}.chart-item{width:400px;height:300px;position:relative;}.chart-item iframe{position:absolute;top:0;left:0;width:106.5%;height:100%;aspect-ratio:16/9;border:none;}</style></head><body><div class='navbar'><p class='navbar-brand'>Projeto Piscicultura</p></div><div class='container'><form method='GET' action='http://177.34.16.99:4000'><label for='modo'>Modo:</label><select id='modo' name='modo'><option value='automatico'>Modo Automático</option><option value='manual'>Modo Manual</option></select><label for='ph_min'>pH mínimo:</label><input type='text' id='ph_min' name='ph_min' class='input-number' placeholder='Informe o pH mínimo'><label for='ph_max'>pH máximo:</label><input type='text' id='ph_max' name='ph_max' class='input-number' placeholder='Informe o pH máximo'><label for='sal_min'>Salinidade mínima:</label><input type='text' id='sal_min' name='sal_min' class='input-number' placeholder='Informe a salinidade mínima'><label for='sal_max'>Salinidade máxima:</label><input type='text' id='sal_max' name='sal_max' class='input-number' placeholder='Informe a salinidade máxima'><label for='o2_min'>O2 mínimo:</label><input type='text' id='o2_min' name='o2_min' class='input-number' placeholder='Informe o nível de oxigênio mínimo'><label for='o2_max'>O2 máximo:</label><input type='text' id='o2_max' name='o2_max' class='input-number' placeholder='Informe o nível de oxigênio máximo'><label for='cond_min'>Condutividade mínima (%):</label><input type='text' id='cond_min' name='cond_min' class='input-number' placeholder='Informe a condutividade mínima em %'><label for='cond_max'>Condutividade máxima (%):</label><input type='text' id='cond_max' name='cond_max' class='input-number' placeholder='Informe a condutividade máxima em %'><label for='temp_min'>Temperatura mínima:</label><input type='text' id='temp_min' name='temp_min' class='input-number' placeholder='Informe a temperatura mínima'><label for='temp_max'>Temperatura máxima:</label><input type='text' id='temp_max' name='temp_max' class='input-number' placeholder='Informe a temperatura máxima'><label for='ox_min'>Oxigênio mínimo:</label><input type='text' id='ox_min' name='ox_min' class='input-number' placeholder='Informe o nível de oxigênio dissolvido mínimo'><label for='ox_max'>Oxigênio máximo:</label><input type='text' id='ox_max' name='ox_max' class='input-number' placeholder='Informe o nível de oxigênio dissolvido máximo'><input type='submit' value='Enviar' class='input-submit'></form></div><h2 id='graficos'>Gráficos do ThingSpeak</h2><div class='chart-area'><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/1?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Temperatura&type=line&xaxis=Tempo&yaxis=Graus' scrolling='yes'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/2?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=pH&type=line&xaxis=Tempo&yaxis=pH' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/3?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Oxigenio+Dissolvido&type=line&xaxis=Tempo&yaxis=Mg%2FL' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/4?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=ORP&type=line&xaxis=Tempo&yaxis=mV' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/5?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Condutivdade&type=line&xaxis=Tempo&yaxis=uS%2FL' scrolling='no'></iframe></div><div class='chart-item'><iframe src='https://thingspeak.com/channels/1466041/charts/6?bgcolor=%23ffffff&color=%23d62020&days=30&dynamic=true&results=1000&title=Salinidade&type=line&xaxis=Tempo&yaxis=ppt&yaxismax=42&yaxismin=0' scrolling='no'></iframe></div></div></body><script>const modoSelect=document.getElementById('modo');const inputs=Array.from(document.querySelectorAll('.input-number'));modoSelect.value='automatico';if(modoSelect.value==='automatico'){inputs.forEach(input=>{input.value='15';input.removeAttribute('required');});}modoSelect.addEventListener('change',function(){const modo=modoSelect.value;if(modo==='automatico'){inputs.forEach(input=>{input.value='15';input.removeAttribute('required');});}else if(modo==='manual'){inputs.forEach(input=>{input.value='';input.setAttribute('required',true);});}});</script></html>");
        break;
      }
    }

    delay(1);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void compare(float value) {
  delay(500);
  if (value > 0) {
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
}
