
#define BLYNK_TEMPLATE_ID "TMPL6GoXCUkfm"
#define BLYNK_TEMPLATE_NAME "Mist MakerCopy"
#define BLYNK_AUTH_TOKEN "PLRxu7cjuHk_uITR4c9Wkvgc-A2FiJDV"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;

#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

const char *ssid = "Akifa-wifi";       // Akifa-wifi
const char *pass = "akifa123"; // akifa123
boolean temp = true;

#define DHTPIN 4      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT type (DHT11 or DHT22)
DHT dht(DHTPIN, DHTTYPE);

const int mist_switch = 16;

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V2);
}

void setup()
{
  Serial.begin(115200);
  dht.begin();

  // Setting Up the pins
  pinMode(DHTPIN, INPUT);
  pinMode(mist_switch, OUTPUT);
  digitalWrite(mist_switch, HIGH);
  

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Blynk.begin(auth, ssid, pass);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{

  Blynk.run();
  Serial.println(Blynk.connected());
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println(humidity);
  Serial.println(temperature);


// Conditions for Humidifier
  if (humidity<= 78 && temp == true){
    digitalWrite(mist_switch, LOW);
    delay(500);
    digitalWrite(mist_switch, HIGH);
    Serial.println("On");
    temp = false;
  }
  else if(humidity>78 && temp == false){
    digitalWrite(mist_switch, LOW);
    delay(500);
    digitalWrite(mist_switch, HIGH);
    delay(1000);
    digitalWrite(mist_switch, LOW);
    delay(500);
    digitalWrite(mist_switch, HIGH);
    temp = true;
    Serial.println("off");
  }

// Updating the values
  Blynk.virtualWrite(V0, humidity);
  Blynk.virtualWrite(V1, temperature);
}
