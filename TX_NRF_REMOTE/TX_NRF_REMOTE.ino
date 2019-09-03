
//VCC >>> 5V
//GND >>> GND
//CE >>>> 7
//CSN >>> 8
//SCK >>> 13
//MOSI >> 11
//MISO >> 12

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7, 8);                   // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

const unsigned long interval = 200; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload
  char text, text2;
  unsigned long counter, counter2;
};

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Transmitting.......");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {


  network.update();                          // Check the network regularly


  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    payload_t payload = { 'g', 'f', analogRead(A0), analogRead(A1)};
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header, &payload, sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}
