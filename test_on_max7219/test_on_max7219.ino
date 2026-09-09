#define DIN_PIN   11
#define LOAD_PIN  10
#define CLK_PIN   13

int hours   = 12;
int minutes = 0;
int seconds = 0;

unsigned long lastTick = 0;
bool colonOn = true;

void sendData(byte address, byte data) {
  digitalWrite(LOAD_PIN, LOW);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, address);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  digitalWrite(LOAD_PIN, HIGH);
}

void showTime() {
  byte h10 = hours / 10;
  byte h1  = hours % 10;
  byte m10 = minutes / 10;
  byte m1  = minutes % 10;

  byte d2 = h1;
  byte d3 = h10;
  if (colonOn) {
    d2 |= 0x80;
    d3 |= 0x80;
  }

  sendData(0x01, m1);   // DIG0
  sendData(0x02, m10);  // DIG1
  sendData(0x03, d2);   // DIG2
  sendData(0x04, d3);   // DIG3
}

void setup() {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(LOAD_PIN, OUTPUT);

  digitalWrite(CLK_PIN, LOW);
  digitalWrite(LOAD_PIN, HIGH);
  delay(100);

  sendData(0x0F, 0x00);   // off
  sendData(0x0C, 0x01);   // Normal 
  sendData(0x09, 0x0F);   
  sendData(0x0B, 0x03);   
  sendData(0x0A, 0x08);   

  showTime();
  lastTick = millis();
}

void loop() {
  if (millis() - lastTick >= 1000) {
    lastTick += 1000;

    if (++seconds >= 60) {
      seconds = 0;
      if (++minutes >= 60) {
        minutes = 0;
        if (++hours >= 24) hours = 0;
      }
    }

    colonOn = !colonOn;   // blink
    showTime();
  }
}