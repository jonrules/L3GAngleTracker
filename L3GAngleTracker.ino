/* Include A-Star libraries */
#include <AStar32U4.h>

/* Include sensor libraries */
#include <Wire.h>
#include <L3G.h>

/* Include utils libraries */
#include <math.h>


/* Data structures */
struct AxisValues {
  int x;
  int y;
  int z;
};

/* Timers */
unsigned long time;
int dt = 100;

/* Gyro */
L3G gyro;
int gyroSensitivity = 1;
float gyroScale = 0.3;
int gyroAccelerometerCutoff = 2000;
AxisValues gyroDirection = { 1, 1, -1 };
AxisValues gyroNeutral;
AxisValues gyroValues;

int calculateRotationOffset(int value, int neutralValue) {
  int offset = (int)(gyroScale*(value - neutralValue));
  Serial.println(offset);
  return offset;
}

int calibrate() {
  ledRed(1);
  ledYellow(1);
  delay(1000);
  ledRed(0);
  ledYellow(1);
  delay(1000);
  ledRed(1);
  ledYellow(1);
  delay(1000);

  /* Calibrate gyro */
  gyro.read();
  gyroNeutral.x = gyro.g.x;
  gyroNeutral.y = gyro.g.y;
  gyroNeutral.z = gyro.g.z;
  Serial.print("Gyro Neutral: (");
  Serial.print(gyroNeutral.x);
  Serial.print(", ");
  Serial.print(gyroNeutral.y);
  Serial.print(", ");
  Serial.print(gyroNeutral.z);
  Serial.println(")");

  ledGreen(1);
  delay(1000);
  ledRed(0);
  ledYellow(0);
  ledGreen(0);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!gyro.init(L3G::deviceType::device_D20H, L3G::sa0State::sa0_high))
  {
    delay(2000);
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }
  gyro.enableDefault();

  calibrate();

  // Initial values
  time = millis();
  gyro.read();
  gyroValues.x = (int)gyro.g.x;
  gyroValues.y = (int)gyro.g.y;
  gyroValues.z = (int)gyro.g.z;
}

void loop() {
  gyro.read();

  Serial.print("Gyro.g.x:");
  Serial.print((int)gyro.g.x);
  Serial.print(", Gyro.g.y:");
  Serial.print((int)gyro.g.y);
  Serial.print(", Gyro.g.z:");
  Serial.print((int)gyro.g.z);


}

