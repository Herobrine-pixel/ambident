#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin(); // BH1750 uses Wire on A4/A5
  Serial.println("BH1750 example");
}

void loop() {
  env.update();
  Serial.print("Context: "); Serial.println(env.getContext());
  Serial.print("Lux: "); Serial.println(env.lux());
  delay(1000);
}
