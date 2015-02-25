 int ledPin = 13; // Set the pin to digital I/O 13

 void setup() 
 {
   pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
   Serial.begin(19200); // Start serial communication at 19200 bps
 }

 void loop() 
 {
   if (Serial.available()) 
   { // If data is available to read,
     char val = Serial.read(); // read it and store it in 
     Serial.write(val);
     digitalWrite( ledPin, HIGH);
   }

   //delay(10); // Wait 10 milliseconds for next reading
}

