#include "Braccio.h"

byte posBase;
byte posShoulder;
byte posElbow;
byte posWristRot;
byte posWristVer;
byte posGripper;

_Braccio Braccio;

//Initialize Braccio object
_Braccio::_Braccio() {}


void _Braccio::begin(int soft_start_level) 
{
    if(soft_start_level != SOFT_START_DISABLED)
    {
        pinMode(SOFT_START_CONTROL_PIN,OUTPUT);
        digitalWrite(SOFT_START_CONTROL_PIN,LOW);
    }
    
    //inits pins
    _Pins[BASE]       = 11;
    _Pins[SHOULDER]   = 10;
    _Pins[ELBOW]      =  9;
    _Pins[WRIST_ROT]  =  6;
    _Pins[WRIST_VER]  =  5;
    _Pins[GRIPPER]    =  3;
    
	posBase 	= 90;
	posShoulder = 95;
	posElbow 	= 95; 
	posWristRot = 90;
	posWristVer = 90;
	posGripper 	= 90;

  	// initialization pin Servo motors
  	base.attach(_Pins[BASE]);
  	shoulder.attach(_Pins[SHOULDER]);
  	elbow.attach(_Pins[ELBOW]);
  	wrist_rot.attach(_Pins[WRIST_ROT]);
  	wrist_ver.attach(_Pins[WRIST_VER]);
  	gripper.attach(_Pins[GRIPPER]);
  
  	//inits standing position
  	stand();
   
    if(soft_start_level != SOFT_START_DISABLED)
        _softStart(soft_start_level);
}

void _Braccio::_softwarePWM(int high_time, int low_time)
{
	digitalWrite(SOFT_START_CONTROL_PIN,HIGH);
	delayMicroseconds(high_time);
	digitalWrite(SOFT_START_CONTROL_PIN,LOW);
	delayMicroseconds(low_time); 
}


void _Braccio::_softStart(int soft_start_level)
{      
	long int tmp=millis();
	while(millis()-tmp < LOW_LIMIT_TIMEOUT)
		_softwarePWM(80+soft_start_level, 450 - soft_start_level);   //the sum should be 530usec	

	while(millis()-tmp < HIGH_LIMIT_TIMEOUT)
		_softwarePWM(75 + soft_start_level, 430 - soft_start_level); //the sum should be 505usec

	digitalWrite(SOFT_START_CONTROL_PIN,HIGH);
}

void _Braccio::moveAll(byte vBase, byte vShoulder, byte vElbow, byte vWrist_rot, byte vWrist_ver, byte vGripper, const SPEED speed)
{
	// Check values, to avoid dangerous positions for the Braccio
	if (vBase > 180) 		vBase 		= 180;
	if (vShoulder < 20) 	vShoulder 	= 20;
	if (vShoulder > 160) 	vShoulder 	= 160;
	if (vElbow > 180) 		vElbow 		= 180;
	if (vWrist_rot > 180) 	vWrist_rot 	= 180;
	if (vWrist_ver > 180) 	vWrist_ver 	= 180;
    if (vGripper < 25) 		vGripper 	= 25;
	if (vGripper > 90) 		vGripper 	= 90;

	bool exit = false;

	//Until the all motors are in the desired position
	//For each servo motor if next degree is not the same of the previuos than do the movement		
	while (!exit) 
	{		
		//turns base	
		if (vBase != posBase) 
		{			
			base.write(posBase);

			if (vBase > posBase) 
			{
				posBase++;
			}
			
			if (vBase < posBase) 
			{
				posBase--;
			}
		}

		//turns shoulder
		if (vShoulder != posShoulder)  
		{
			shoulder.write(posShoulder);

			if (vShoulder > posShoulder) 
			{
				posShoulder++;
			}
			
			if (vShoulder < posShoulder) 
			{	
				posShoulder--;
			}
		}

		//turns elbow
		if (vElbow != posElbow)  
		{
			elbow.write(posElbow);

			if (vElbow > posElbow)
			{
				posElbow++;
			}
			
			if (vElbow < posElbow) 
			{
				posElbow--;
			}
		}

		//turns wrist rot
		if (vWrist_rot != posWristRot) 
		{
			wrist_rot.write(posWristRot);

			if (vWrist_rot > posWristRot) 
			{
				posWristRot++;				
			}
			
			if (vWrist_rot < posWristRot) 
			{
				posWristRot--;
			}
		}

		//turns wrist ver
		if (vWrist_ver != posWristVer)
		{
			wrist_ver.write(posWristVer);

			if (vWrist_ver > posWristVer) 
			{
				posWristVer++;
			}

			if (vWrist_ver < posWristVer) 
			{
				posWristVer--;
			}
		}

		//turns gripper
		if (vGripper != posGripper)
		{
			gripper.write(posGripper);

			if (vGripper > posGripper)
				posGripper++;
			
			if (vGripper < posGripper) 
				posGripper--;
		}
		
		//delay between each movement
		delay(speed);
		
		//It checks if all the servo motors are in the desired position 
		if ((vBase == posBase) && (vShoulder == posShoulder)
			  && (vElbow == posElbow) && (vWrist_rot == posWristRot)
			    && (vWrist_ver == posWristVer) && (vGripper == posGripper))
		{
			exit = true;
		} 
	}
}

void _Braccio::resetPos()
{
	posBase       			= 90;
	posShoulder     		= 95;
	posElbow      			= 95; 
	posWristRot 			= 90;
	posWristVer 			= 90;
	posGripper      		= 90;
}

void _Braccio::stand()
{
	//moveAll(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper, RAPIDE);
  moveAll(90, 95, 95, 90, 90, 90, RAPIDE);
  //resetPos();
}

void _Braccio::rangeTest()
{
	//turns base
	stand();
	for (byte i = 0; i < 180; i++)
	{
		moveAll(i, posShoulder, posElbow, posWristRot, posWristVer, posGripper, RAPIDE);
	}

	//turns shoulder
	stand();
	for (byte i = 20; i < 160; i++)
	{
		moveAll(posBase, i, posElbow, posWristRot, posWristVer, posGripper, RAPIDE);
	}

	//turns elbow
	stand();
	for (byte i = 0; i < 180; i++)
	{
		moveAll(posBase, posShoulder, i, posWristRot, posWristVer, posGripper, RAPIDE);
	}

	//turns wrist rot
	stand();
	for (byte i = 0; i < 180; i++)
	{
		moveAll(posBase, posShoulder, posElbow, i, posWristVer, posGripper, RAPIDE);
	}

	//turns wrist ver
	stand();
	for (byte i = 0; i < 180; i++)
	{
		moveAll(posBase, posShoulder, posElbow, posWristRot, i, posGripper, RAPIDE);
	}

	//turns gripper
	stand();
	for (byte i = 25; i < 90; i++)
	{
		moveAll(posBase, posShoulder, posElbow, posWristRot, posWristVer, i, RAPIDE);
	} 
}


/**
 * Turns the motor specified
 * @param motor Name of the motor
 * @param position New position of this motor
 * @param speed Speed of the movement
 */
void _Braccio::moveMotor(const NAME name, byte position, const SPEED speed)
{
	position = (position > 180) ? 180 : position;

	//turns motor according to the name
	switch (name)
	{
		case BASE:
		{
			moveAll(position, posShoulder, posElbow, posWristRot, posWristVer, posGripper, speed);
			break;
		}

		case SHOULDER:
		{
			position = (position > 160) ? 160 : position;
			position = (position < 20) ? 20 : position;
			moveAll(posBase, position, posElbow, posWristRot, posWristVer, posGripper, speed);
			break;
		}

		case ELBOW:
		{
			moveAll(posBase, posShoulder, position, posWristRot, posWristVer, posGripper, speed);
			break;
		}

		case WRIST_ROT:
		{
			moveAll(posBase, posShoulder, posElbow, position, posWristVer, posGripper, speed);
			break;
		}

		case WRIST_VER:
		{
			moveAll(posBase, posShoulder, posElbow, posWristRot, position, posGripper, speed);
			break;
		}

		case GRIPPER:
		{
			position = (position > 90) ? 90 : position;
			position = (position < 25) ? 25 : position;
			moveAll(posBase, posShoulder, posElbow, posWristRot, posWristVer, position, speed);
			break;
		}
	}
}

void _Braccio::openGripper(const SPEED speed)
{
    posGripper			= 90;
  	moveMotor(GRIPPER, posGripper, speed);
}

void _Braccio::closeGripper(const SPEED speed)
{
	posGripper	 		= 25;
  	moveMotor(GRIPPER, posGripper, speed);
}
