#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin();
  Serial.println("MQ2 example");
}

void loop() {
  env.update();
  Serial.print("Gas: "); Serial.println(env.gasRaw());
  Serial.print("Context: "); Serial.println(env.getContext());
  delay(1000);
}
