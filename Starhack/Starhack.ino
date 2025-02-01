#include <dht_nonblocking.h>

#define DTHPIN 2
#define DTHTYPE DHT11

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  DHT_nonblocking dht(2, "DHT11");


  float temp = 0.0;
  float hum = 0.0;
  
  if (dht.measure(&temp, &hum)) {
  Serial.print("Temp : ");
  Serial.print(temp);
  Serial.print("\t");
  
  Serial.print("Hum : ");
  Serial.print(hum);
  Serial.print("\t");
  }

  
}
