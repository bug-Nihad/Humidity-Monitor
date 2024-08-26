
#define BLYNK_TEMPLATE_ID "TMPL6GoXCUkfm"
#define BLYNK_TEMPLATE_NAME "Mist MakerCopy"
#define BLYNK_AUTH_TOKEN "PLRxu7cjuHk_uITR4c9Wkvgc-A2FiJDV"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;

#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

const char *ssid = "Cartoon"; 
const char *pass = "12345678";   
boolean temp = true;

#define DHTPIN 4      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT type (DHT11 or DHT22)
DHT dht(DHTPIN, DHTTYPE);

const int mist_switch = 16;
int sw_value = 0;

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V2);
}
void mist_on()
{
  digitalWrite(mist_switch, LOW);
  delay(500);
  digitalWrite(mist_switch, HIGH);
  Serial.println("Mist is On.");
  temp = false;
  // Blynk.setProperty(V3, "color", "#23C48E");
  // Blynk.virtualWrite(V3, 255);
}

void mist_off()
{
  digitalWrite(mist_switch, LOW);
  delay(500);
  digitalWrite(mist_switch, HIGH);
  delay(1000);
  digitalWrite(mist_switch, LOW);
  delay(500);
  digitalWrite(mist_switch, HIGH);
  temp = true;
  Serial.println("Mist is off.");
  // Blynk.setProperty(V3, "color", "#D3435C");
  // Blynk.virtualWrite(V3, 255);
}

BLYNK_WRITE(V2)
{
  sw_value = param.asInt();
  Serial.print("Button value is: ");
  Serial.println(sw_value);
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
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Blynk Connected: ");
  Serial.println(Blynk.connected());


  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Conditions for Humidifier
  if (humidity <= sw_value && temp == true)
  {
    mist_on();
  }
  else if (humidity > sw_value && temp == false)
  {
    mist_off();
  }

  // Updating the values
  Blynk.virtualWrite(V0, humidity);
  Blynk.virtualWrite(V1, temperature);
  if (temp == false) // Mist is on
  {
    Blynk.setProperty(V3, "color", "#23C48E"); // Green
    Blynk.virtualWrite(V3, 200);
  }
  else if (temp == true)  //Mist is off
  {
    Blynk.setProperty(V3, "color", "#D3435C");  //Red
    Blynk.virtualWrite(V3, 200);
  }

  delay(500);


}
