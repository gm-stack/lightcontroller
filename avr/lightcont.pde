int redPin = 9;
int greenPin = 10;
int bluePin = 11;

int i = 0;
void setup() {
  Serial.begin(115200);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
}

void loop() {
  int a = Serial.read();
  if (a != -1) {
    int redByte = a;
    delay(1);
    int greenByte = Serial.read();
    delay(1);
    int blueByte = Serial.read();
    analogWrite(redPin,redByte);
    analogWrite(greenPin,greenByte);
    analogWrite(bluePin,blueByte);
  }
}
