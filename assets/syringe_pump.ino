// Arduino Syringe Pump
// Controls a NEMA 17 stepper motor to dispense fluid at a
// specified volumetric flow rate. Includes an RGB status LED,
// latching start/pause button, limit/stop input, and I2C LCD.

// Imported libraries
#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Stepper motor: DRIVER mode, STEP pin = 2, DIR pin = 3
AccelStepper stepper(AccelStepper::DRIVER, 2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Syringe / flow-rate parameters
// 10 mL = 14.9 mm diameter
// 20 mL = 19 mm diameter
double flowRate = 10.6;
double diameter = 19;
double totalVol = 20;

// Conversion from mm^3 to mL
double conversionFactor = 0.001;

// Pi constant
const float MY_PI = 3.14159265;

// Button, LED, and latching-button pins
const int buttonPin = 4;
const int redLED = 11;
const int greenLED = 10;
const int blueLED = 9;
const int latchingButton = 5;

// Other variables
boolean motorRunning = false;
long lastStepCount = 0;
double lastRemainingTime = 0;

void setup()
{
  // Maximum motor speed: 1000 steps/second
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(0);
  stepper.setCurrentPosition(0);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(latchingButton, INPUT_PULLUP);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  lcd.init();                 // Initialize LCD
  lcd.backlight();            // Turn on LCD backlight

  lcd.setCursor(0, 0);
  lcd.print("RATE:");
  lcd.print(flowRate);

  lcd.setCursor(0, 1);
  lcd.print("I2C LCD Ready");
}

// Calculates the cross-sectional area of the syringe
double calcArea(double diameter)
{
  double r = diameter / 2.0;
  return MY_PI * r * r;
}

// Calculates the required motor step rate for a given flow rate
double numStepsPerSec(double diameter, double flowRate)
{
  double leadScrew = 2.0;
  double motorStepRate = 3200;

  double area = calcArea(diameter);

  // Convert mL/min to mL/sec
  double flowMlSec = flowRate / 60.0;

  // Volume moved during one lead-screw revolution
  double volumePerRev = leadScrew * area;

  // Volume moved per motor step
  double volumePerStep = volumePerRev / motorStepRate;

  // Convert mm^3 to mL
  double mlPerStep = volumePerStep * conversionFactor;

  // Calculate required steps/sec
  double stepsPerSec = flowMlSec / mlPerStep;

  return stepsPerSec;
}

// Computes remaining time based on the amount of fluid remaining
// and the number of steps already taken. Used for the LCD.
double computeRemainingTime(double diameter, double flowRate,
                            double totalVolume_ml, long stepsMoved)
{
  double leadScrew = 2.0;
  double motorStepRate = 3200;

  // Area of the syringe
  double area = calcArea(diameter);

  // Volume moved per step (mm^3/step)
  double volumePerRev = leadScrew * area;
  double volumePerStep = volumePerRev / motorStepRate;

  // Convert mm^3 to mL
  double mlPerStep = volumePerStep * conversionFactor;

  // Total volume delivered so far
  double delivered_ml = stepsMoved * mlPerStep;

  // Remaining volume
  double remaining_ml = totalVolume_ml - delivered_ml;

  if (remaining_ml < 0)
    remaining_ml = 0;

  // Calculate remaining time
  if (flowRate <= 0)
    return -1;  // Avoid division by zero

  double remainingTime_sec = (remaining_ml / flowRate) * 60.0;

  return remainingTime_sec;
}

void loop()
{
  // Determine button states
  boolean buttonPressed = (digitalRead(buttonPin) == LOW);
  boolean latchingButtonPressed = (digitalRead(latchingButton) == HIGH);

  // If the latching button is not pressed and the normal button is pressed,
  // stop the motor and show yellow
  if (buttonPressed && !latchingButtonPressed)
  {
    stepper.setSpeed(0);

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, LOW);
  }

  // If the latching button is pressed, stop the motor and show red
  else if (latchingButtonPressed)
  {
    stepper.setSpeed(0);

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);
  }

  else
  {
    // Negative step rate rotates the motor in the required direction
    stepper.setSpeed(-numStepsPerSec(diameter, flowRate));

    // Show green while running
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, LOW);

    long currentSteps = abs(stepper.currentPosition());

    // Calculate remaining time
    lastRemainingTime = computeRemainingTime(
      diameter,
      flowRate,
      totalVol,
      currentSteps
    );
  }

  // Display remaining time on LCD
  int minutes = lastRemainingTime / 60;
  int seconds = (int)lastRemainingTime % 60;

  lcd.setCursor(0, 1);
  lcd.print("Left: ");
  lcd.print(minutes);
  lcd.print(" min ");

  if (seconds < 10)
    lcd.print("0");  // Add leading zero for 0-9 seconds

  lcd.print(seconds);
  lcd.print(" s ");

  // Run the stepper at the commanded constant speed
  stepper.runSpeed();
}
