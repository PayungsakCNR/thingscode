#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#define wifi_ssid "CNRIoT"
#define wifi_password "iot.coe.psu.ac.th"

#define mqtt_server "202.29.151.23"
#define mqtt_user "pao-temp-hum"
#define mqtt_password "iot.coe.psu.ac.th"
#define mq135_topic  "sensor/nodemcu/mq135"
#define light_topic  "sensor/nodemcu/light"
#define ledPin  D0
#define light_pin D5

WiFiClient espClient;
PubSubClient client(espClient);

int sensorValue;
int lightVal;
String payload;
String payload_light;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(light_pin, INPUT);
  Serial.begin(115200);      // sets the serial port to 9600
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
    
      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;  
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      clientName += "-";
      clientName += String(micros() & 0xff, 16);
      Serial.print("Connecting to ");
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);

    if (client.connect((char*)clientName.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    
  }
}

void loop(){
      led_blink();
      if (!client.connected()) {
        reconnect();
      }
      client.loop();

      // Wait a few seconds between measurements.
      delay(2000);
      
  sensorValue = analogRead(0);       // read analog input pin 0
  delay(500);
  lightVal = digitalRead(light_pin);
   
  //payload="{\"AirQuality\":"+sensorValue"}";
  String AirQualityStr=String(sensorValue);
  String LightStr = String(lightVal);
  
  payload="{\"AirQuality\":"+AirQualityStr+"}";
  payload_light="{\"Light\":"+LightStr+"}";
  
  client.publish(mq135_topic, String(payload).c_str(), true);
  delay(100);
  client.publish(light_topic, String(payload_light).c_str(), true);
  Serial.println(sensorValue, DEC);  // prints the value read
  Serial.println(lightVal, DEC);  // prints the value read
  delay(5000);                        // wait 5000ms for next reading

}

void led_blink(){
      digitalWrite(ledPin,LOW);//Turn on the LED
      delay(2000);
      digitalWrite(ledPin,HIGH);//Turn on the LED
}
