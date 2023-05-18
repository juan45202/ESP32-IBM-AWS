// Librerias para usar el ESP32
# include <WiFi.h>
# include <WiFiClient.h>
# include <PubSubClient.h>

// Configuracion red WiFi
const char* ssid = "Galaxy A324AA1";
const char* password = "padre123";

# define ORG "12yv6g"
# define DEVICE_TYPE "ESP32"
# define DEVICE_ID "E8560000F0C8"
# define TOKEN "KPp7rSTWqu-_fNjf+K"

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic1[] = "iot-2/evt/estado1/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

int trigPin = 5;
int echoPin = 18;
int duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  Serial.println();
  Serial.println("Connecting to: ");
  Serial.println(ssid);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (!client.connected()) {
    Serial.println("Reconnecting client to: ");
    Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    Serial.print("Bluemix connected");
  }  
  
}

long lastMsg = 0;

void loop() {
  // put your main code here, to run repeatedly:

  client.loop();

  long now = millis();

  if (now - lastMsg > 3000) {
    
    lastMsg = now;
    
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    Serial.println(distance);              

    String payload = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
    payload += ",\"Distancia\":";
    payload += distance;
    payload += "}}";

    Serial.print("Sending payload: ");
    Serial.println(payload);    

    if (client.publish(pubtopic1, (char*) payload.c_str())) {
      Serial.println("Pubish OK");              
    } else {
      Serial.println("Pubish Failed");              
    }
    
  }

  

}
