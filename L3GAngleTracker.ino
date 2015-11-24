/* Include A-Star libraries */
#include <AStar32U4.h>

/* Include sensor libraries */
#include <Wire.h>
#include <L3G.h>

/* Include utils libraries */
#include <math.h>


/* Data structures */
struct AxisValues {
  long x;
  long int y;
  long z;
};

struct LongAxisValues {
  long x;
  long int y;
  long z;
};

/* Timers */
unsigned long initialTime;

/* Gyro */
L3G gyro;
// Sensitivity in units of mdps/LSB
double gyroSensitivity = 8.75;
// Neutral values in units of LSB (raw reading)
AxisValues gyroNeutral;
// Angle in units of md/s
LongAxisValues gyroAngle;

long calculateAngleOffset(int value, int neutralValue, unsigned long dt) {
  long offset = (long)(gyroSensitivity*(value - neutralValue)*dt/1000);
  return offset;
}

void calibrate() {
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
  gyroNeutral.x = (int)gyro.g.x;
  gyroNeutral.y = (int)gyro.g.y;
  gyroNeutral.z = (int)gyro.g.z;
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
  initialTime = millis();
}

void loop() {
  gyro.read();

  unsigned long currentTime = millis();
  unsigned long dt = currentTime - initialTime;

  gyroAngle.x += calculateAngleOffset((int)gyro.g.x, gyroNeutral.x, dt);
  gyroAngle.y += calculateAngleOffset((int)gyro.g.y, gyroNeutral.y, dt);
  gyroAngle.z += calculateAngleOffset((int)gyro.g.z, gyroNeutral.z, dt);
  

  Serial.print("X:");
  Serial.print(gyroAngle.x/1000);
  Serial.print(", Y:");
  Serial.print(gyroAngle.y/1000);
  Serial.print(", Z: ");
  Serial.println(gyroAngle.z/1000);

  initialTime = currentTime;
}

