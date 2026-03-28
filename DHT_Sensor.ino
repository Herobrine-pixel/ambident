#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin(); // default DHT pin = 2
  Serial.println("DHT example");
}

void loop() {
  env.update();
  Serial.print("Context: "); Serial.println(env.getContext());
  Serial.print("Temp: "); Serial.println(env.temperature());
  Serial.print("Hum: "); Serial.println(env.humidity());
  delay(1000);
}
