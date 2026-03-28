#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin(); // gas A0, mic A2, dht pin 2, ds18b20 pin 3
  Serial.println("All sensors demo");
}

void loop() {
  env.update();
  Serial.print("Context: "); Serial.println(env.getContext());
  Serial.print("Gas: "); Serial.println(env.gasRaw());
  Serial.print("Mic: "); Serial.println(env.micRaw());
  Serial.print("Lux: "); Serial.println(env.lux());
  Serial.print("Temp: "); Serial.println(env.temperature());
  Serial.print("Hum: "); Serial.println(env.humidity());
  Serial.print("BMP Temp: "); Serial.println(env.bmpTemperature());
  Serial.print("BMP Press: "); Serial.println(env.bmpPressure());
  Serial.print("DS18B20: "); Serial.println(env.ds18b20Temp());
  delay(1500);
}
