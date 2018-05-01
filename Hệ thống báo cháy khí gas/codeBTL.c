#include<LiquidCrystal.h>
//System state
const char onMessage[]="Alarm on";
const char offMessage[]="Alarm off";
const char activeMessage[][16]={"Fire Alarm","Please evacuate"};
const int OFF = 0, ON = 1, ACTIVE = 2;
int state = OFF;
//Pins
const int tempSensor = A2;
const int gasSensor = A3;
const int asSensor = A0;

const int led = 5;
const int piezo = 3;
const int power = 2;

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

int temperature;
int gas;
int as;

void setup()
{
	pinMode(led,OUTPUT);
	pinMode(piezo,OUTPUT);
	lcd.begin(16,2);
	setOff();
}
void loop()
{	
	int on = digitalRead(power);
	if(on && (state == OFF)){
		setOn();
	}else if(!on && (state != OFF)){
		setOff();
	}
	
	if(state == ON){
		readSensors();
		if(temperature > 45 || gas > 30 || as > 60){
			setActive();
		}
	}else if(state == ACTIVE){
		int ledState = digitalRead(led);
		digitalWrite(led,!ledState);
		if(ledState == HIGH){
			delay(50);
		}else{
			delay(20);
		}
	}
}

void setOff(){
	state = OFF;
	digitalWrite(led,LOW);
	noTone(piezo);
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.print(offMessage);
}

void setOn(){
	state = ON;
	digitalWrite(led,HIGH);
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.print(onMessage);
}

void setActive(){
	state = ACTIVE;
	tone(piezo,750);
	lcd.clear();
	lcd.print(activeMessage[0]);
	lcd.setCursor(0,1);
	lcd.print(activeMessage[1]);
}

void readSensors(){
	int t = analogRead(tempSensor);
	temperature = map(t,20,350,-40,125);
	int g = analogRead(gasSensor);
	gas = map(g,157,600,0,100);
	int a = analogRead(asSensor);
	as = map(a,0,1023, 36, 84);
}

