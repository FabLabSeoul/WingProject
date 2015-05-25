 char val; // Data received from the serial port
 int ledPin = 13; // Set the pin to digital I/O 13

 void setup() 
 {
   pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
//   Serial.begin(9600); // Start serial communication at 9600 bps
   Serial.begin(115200); // Start serial communication at 9600 bps
 }

 void loop() 
 {
   if (Serial.available()) 
   { // If data is available to read,
     val = Serial.read(); // read it and store it in val
     Serial.print(val);
   }

   delay(10); // Wait 10 milliseconds for next reading
}

