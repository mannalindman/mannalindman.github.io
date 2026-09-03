---
title: "Syringe Pump"
excerpt: "DIY syringe pump capable of 1 uL / min flow rate up to 10 mL / min"
header:
  image: /assets/img/syringe_pump_3.jpg
  teaser: /assets/img/syringe_pump_3.jpg
gallery:
  - image_path: assets/img/syringe_pump_1.jpg
  - image_path: assets/img/syringe_pump_2.jpg
  - image_path: assets/img/syringe_pump_3.jpg
   
---

# Features

* Stepper-motor-driven linear actuation using a lead screw
* Compatible with 10 mL and 20 mL syringes
* Arduino Uno control system
* A4988 stepper motor driver
* Adjustable volumetric flow rate through software
* Microstepping for smoother and lower-vibration operation
* Latching start/pause button
* Limit switch to detect when the syringe reaches the empty position
* RGB LED status indicator
 * Green — Pump running
 * Yellow — Pump paused
 * Red — Syringe empty
* Custom 3D-printed mechanical components
* Enclosed electronics designed to reduce exposure to accidental liquid spills
* Compact aluminum-extrusion-based mechanical structure

# Off-the-Shelf Parts

* NEMA 17 Stepper Motor
* 250 mm Lead Screw — 2 mm Lead	
* 250 mm Lead Screw — 8 mm
* 1/4" × 8 mm Flexible Coupling
* 200 mm × 8 mm Linear Rod
* LM8UU Linear Bearing
* 2040 Aluminum Extrusion — 1 ft
* Arduino Uno
* A4988 Stepper Motor Driver
* MEAN WELL RQ-65D Power Supply
* Small Breadboard
* Panel-Mount Latching Push Button
* Limit Switch
* RGB Common-Cathode LED
* Wires, Resistors & Fasteners
* Syringe

# CAD Model
<iframe src="https://myhub.autodesk360.com/ue2df0af5/shares/public/SH35dfcQT936092f0e437224cd558fdcdc2f?mode=embed" width="800" height="600" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"  frameborder="0"></iframe>

# Arduino Code
a. Arduino code 
b. //imported libraries  
c. #include <AccelStepper.h> 
d. #include <LiquidCrystal_I2C.h> 
e. #include <Wire.h> 
f.  
g. AccelStepper stepper(AccelStepper::DRIVER, 2, 3); 
h. LiquidCrystal_I2C lcd(0x27, 16, 2);   
i.  
j. //10 mL = 14.9 
k. //20 mL = 19  
l. double flowRate = 10.6; 
m. double diameter = 19; 
n. double totalVol = 20; 
o.  
p. // converting the mL to mm^3 to mL 
q. double conversionFactor = 0.001; 
r. //pi constant 
s. const float MY_PI = 3.14159265; 
t.  
u. // all the locations of the button, LED & latching button  
v. const int buttonPin = 4; 
w. const int redLED = 11; 
x. const int greenLED = 10; 
y. const int blueLED = 9; 
z. const int latchingButton = 5; 
aa.  
bb. //other variables for the methods  
cc. boolean motorRunning = false; 
dd. long lastStepCount = 0; 
ee. double lastRemainingTime = 0; 
ff.  
 
gg. void setup() 
hh. {   
ii.   //the maxiumum speed the motor is supposed to go  
jj.    //steps per second 1000/sec 
kk.    stepper.setMaxSpeed(1000); 
ll.    stepper.setSpeed(0); 
mm.    stepper.setCurrentPosition(0); 
nn.  
oo.    pinMode(buttonPin, INPUT_PULLUP);  
pp.    pinMode(latchingButton,INPUT_PULLUP); 
9 
 
   
 
qq.  
rr.    pinMode(redLED, OUTPUT); 
ss.    pinMode(greenLED, OUTPUT); 
tt.    pinMode(blueLED, OUTPUT); 
uu.  
vv.   lcd.init();        // initialize the LCD 
ww.   lcd.backlight();   // turn on the backlight 
xx.  
yy.   lcd.setCursor(0, 0); 
zz.   lcd.print("RATE:"); 
aaa.  
bbb.   lcd.print(flowRate); 
ccc.  
ddd.   lcd.setCursor(0, 1); 
eee.   lcd.print("I2C LCD Ready"); 
fff.  
ggg. } 
hhh.  
iii. //area method used to calculate area of syringe  
jjj. double calcArea(double diameter){ 
kkk.    double r = diameter/2.0; 
lll.    return MY_PI*r*r; 
mmm. } 
nnn.  
ooo. //calculating the number of steps per second  
ppp. double numStepsPerSec(double diameter,double flowRate){ 
qqq. double leadScrew = 2.0; 
rrr. double motorStepRate = 3200; 
sss. double area = calcArea(diameter); 
ttt. //converts mL per min to mL per second  
uuu. double flowMlSec = flowRate/60.0; 
vvv. double volumePerRev = (leadScrew)*area; // mm^3/rev 
www. double volumePerStep = (volumePerRev/motorStepRate);//mm^3/step 
xxx. double mlPerStep = volumePerStep*(conversionFactor);//mm^3--> 
mL/step 
yyy. double stepsPerSec = (flowMlSec/mlPerStep);//step/sec 
zzz. return stepsPerSec; 
aaaa. } 
bbbb.  
cccc. // Computes remaining time based on how much volume is left 
dddd. //USED FOR LCD SCREEN  
eeee. double computeRemainingTime(double diameter, double flowRate, double 
totalVolume_ml, long stepsMoved) { 
ffff.   double leadScrew = 2.0; 
gggg.   double motorStepRate = 3200; 
10 
 
   
 
hhhh.  
iiii.   // area of the syringe  
jjjj.   double area = calcArea(diameter); 
kkkk.  
llll.   // volume moved per step (mm^3/step) --- 
mmmm.   double volumePerRev = leadScrew * area;                  // mm^3 
per full lead screw revolution 
nnnn.   double volumePerStep = volumePerRev / motorStepRate;     // mm^3 
per step 
oooo.  
pppp.   //convert mm^3 → mL 
qqqq.   double mlPerStep = volumePerStep * conversionFactor; 
rrrr.  
ssss.   //total delivered so far 
tttt.   double delivered_ml = stepsMoved * mlPerStep; 
uuuu.  
vvvv.   //remaining volume 
wwww.   double remaining_ml = totalVolume_ml - delivered_ml; 
xxxx.   if (remaining_ml < 0) remaining_ml = 0; 
yyyy.  
zzzz.   //compute remaining time  
aaaaa.   if (flowRate <= 0) return -1;  // avoid divide by zero 
bbbbb.   double remainingTime_sec = (remaining_ml / flowRate) * 60.0; 
ccccc.  
ddddd.   return remainingTime_sec; 
eeeee. } 
fffff.  
 
ggggg. void loop() 
hhhhh. {   
iiiii.   //boolean variables for the button states  
jjjjj.    boolean buttonPressed = (digitalRead(buttonPin)==LOW); 
kkkkk.    boolean latchingButtonPressed = 
(digitalRead(latchingButton)==HIGH); 
lllll.  
mmmmm.    //if the latching button is not pressed and the normal button is 
pressed make it yellow 
nnnnn.    if(buttonPressed && !latchingButtonPressed){ 
ooooo.     //stops the motor and makes it yellow  
ppppp.     stepper.setSpeed(0); 
qqqqq.     digitalWrite(redLED, HIGH); 
rrrrr.     digitalWrite(greenLED, HIGH); 
sssss.     digitalWrite(blueLED, LOW); 
ttttt.    } 
uuuuu.  
11 
 
   
 
vvvvv.    //if the latching button is pressed then make it red  
wwwww.     else if (latchingButtonPressed) { 
xxxxx.     //stops the motor  
yyyyy.     stepper.setSpeed(0);              
zzzzz.     digitalWrite(redLED, HIGH); 
aaaaaa.     digitalWrite(greenLED, LOW); 
bbbbbb.     digitalWrite(blueLED, LOW); 
cccccc.   } 
dddddd.  
eeeeee.    else{ 
ffffff.     //negate the steps per second to make it rotate the other way  
gggggg.     stepper.setSpeed(-numStepsPerSec(diameter,flowRate)); 
hhhhhh.  
iiiiii.     //turns it to green 
jjjjjj.     digitalWrite(redLED, LOW); 
kkkkkk.     digitalWrite(greenLED, HIGH); 
llllll.     digitalWrite(blueLED, LOW); 
mmmmmm.     long currentSteps = abs(stepper.currentPosition()); 
nnnnnn.  
oooooo.     // diameter, flow rate, total volume in the syringe 
pppppp.      lastRemainingTime = 
computeRemainingTime(diameter,flowRate,totalVol,currentSteps); 
qqqqqq.    } 
rrrrrr.  
ssssss. // Display remaining time on the LCD screen  
tttttt. int minutes = lastRemainingTime / 60;       // whole minutes 
uuuuuu. int seconds = (int)lastRemainingTime % 60;  // leftover seconds 
vvvvvv.  
wwwwww. //LCD screen printing  
xxxxxx. lcd.setCursor(0, 1); 
yyyyyy. lcd.print("Left: "); 
zzzzzz. lcd.print(minutes); 
aaaaaaa. lcd.print(" min "); 
bbbbbbb. if (seconds < 10) lcd.print("0");  // add a leading zero for 0–9 
ccccccc. lcd.print(seconds); 
ddddddd. lcd.print(" s  "); 
eeeeeee. stepper.runSpeed(); 
fffffff. } 

{% include gallery caption="Syringe Pump Gallery" %}
