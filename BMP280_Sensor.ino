#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin();
  Serial.println("BMP280 example");
}

void loop() {
  env.update();
  Serial.print("BMP Temp: "); Serial.println(env.bmpTemperature());
  Serial.print("BMP Press: "); Serial.println(env.bmpPressure());
  Serial.print("Context: "); Serial.println(env.getContext());
  delay(1000);
}
