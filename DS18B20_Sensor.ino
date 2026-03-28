#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin();
  Serial.println("DS18B20 example");
}

void loop() {
  env.update();
  Serial.print("DS18B20 Temp: "); Serial.println(env.ds18b20Temp());
  Serial.print("Context: "); Serial.println(env.getContext());
  delay(1000);
}
