#include "Braccio.h"

_Braccio Braccio;

//Initialize Braccio object
_Braccio::_Braccio() {}


void _Braccio::begin(byte &vBase, byte &vShoulder, byte &vElbow, byte &vWrist_rot, byte &vWrist_ver, byte &vGripper, int soft_start_level) 
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
    
  	// initialization pin Servo motors
  	base.attach(_Pins[BASE]);
  	shoulder.attach(_Pins[SHOULDER]);
  	elbow.attach(_Pins[ELBOW]);
  	wrist_rot.attach(_Pins[WRIST_ROT]);
  	wrist_ver.attach(_Pins[WRIST_VER]);
  	gripper.attach(_Pins[GRIPPER]);
  
  	//inits standing position
  	resetPos(vBase, vShoulder, vElbow, vWrist_rot, vWrist_ver, vGripper);
  
  	//For each step motor this set up the initial degree
  	base.write(_Positions[BASE]);
  	shoulder.write(_Positions[SHOULDER]);
  	elbow.write(_Positions[ELBOW]);
  	wrist_ver.write(_Positions[WRIST_ROT]);
  	wrist_rot.write(_Positions[WRIST_VER]);
  	gripper.write(_Positions[GRIPPER]);
   
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
		if (vBase != _Positions[BASE]) 
		{			
			base.write(_Positions[BASE]);

			if (vBase > _Positions[BASE]) 
			{
				_Positions[BASE]++;
			}
			
			if (vBase < _Positions[BASE]) 
			{
				_Positions[BASE]--;
			}
		}

		//turns shoulder
		if (vShoulder != _Positions[SHOULDER])  
		{
			shoulder.write(_Positions[SHOULDER]);

			if (vShoulder > _Positions[SHOULDER]) 
			{
				_Positions[SHOULDER]++;
			}
			
			if (vShoulder < _Positions[SHOULDER]) 
			{	
				_Positions[SHOULDER]--;
			}
		}

		//turns elbow
		if (vElbow != _Positions[ELBOW])  
		{
			elbow.write(_Positions[ELBOW]);

			if (vElbow > _Positions[ELBOW])
			{
				_Positions[ELBOW]++;
			}
			
			if (vElbow < _Positions[ELBOW]) 
			{
				_Positions[ELBOW]--;
			}
		}

		//turns wrist rot
		if (vWrist_rot != _Positions[WRIST_ROT]) 
		{
			wrist_rot.write(_Positions[WRIST_ROT]);

			if (vWrist_rot > _Positions[WRIST_ROT]) 
			{
				_Positions[WRIST_ROT]++;				
			}
			
			if (vWrist_rot < _Positions[WRIST_ROT]) 
			{
				_Positions[WRIST_ROT]--;
			}
		}

		//turns wrist ver
		if (vWrist_ver != _Positions[WRIST_VER])
		{
			wrist_ver.write(_Positions[WRIST_VER]);

			if (vWrist_ver > _Positions[WRIST_VER]) 
			{
				_Positions[WRIST_VER]++;
			}

			if (vWrist_ver < _Positions[WRIST_VER]) 
			{
				_Positions[WRIST_VER]--;
			}
		}

		//turns gripper
		if (vGripper != _Positions[GRIPPER])
		{
			gripper.write(_Positions[GRIPPER]);

			if (vGripper > _Positions[GRIPPER])
				_Positions[GRIPPER]++;
			
			if (vGripper < _Positions[GRIPPER]) 
				_Positions[GRIPPER]--;
		}
		
		//delay between each movement
		delay(speed);
		
		//It checks if all the servo motors are in the desired position 
		if ((vBase == _Positions[BASE]) && (vShoulder == _Positions[SHOULDER])
			  && (vElbow == _Positions[ELBOW]) && (vWrist_rot == _Positions[WRIST_ROT])
			    && (vWrist_ver == _Positions[WRIST_VER]) && (vGripper == _Positions[GRIPPER]))
		{
			exit = true;
		} 
	}
}

void _Braccio::resetPos(byte &posBase, byte &posShoulder, byte &posElbow, byte &posWristRot, byte &posWristVer, byte &posGripper)
{
	posBase       			= 90;
	posShoulder     		= 95;
	posElbow      			= 95; 
	posWristRot 			= 90;
	posWristVer 			= 90;
	posGripper      		= 90;
	_Positions[BASE] 		= 90;
  	_Positions[SHOULDER] 	= 95;
  	_Positions[ELBOW] 		= 95;
  	_Positions[WRIST_ROT] 	= 90;
  	_Positions[WRIST_VER] 	= 90;
  	_Positions[GRIPPER] 	= 90;
}

void _Braccio::stand(byte &posBase, byte &posShoulder, byte &posElbow, byte &posWristRot, byte &posWristVer, byte &posGripper)
{
	resetPos(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], RAPIDE);
}

void _Braccio::rangeTest(byte &posBase, byte &posShoulder, byte &posElbow, byte &posWristRot, byte &posWristVer, byte &posGripper)
{
	//turns base
	stand(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	for (byte i = 0; i < 180; i++)
	{
		moveAll(i, _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], RAPIDE);
	}

	//turns shoulder
	stand(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	for (byte i = 20; i < 160; i++)
	{
		moveAll(_Positions[BASE], i, _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], RAPIDE);
	}

	//turns elbow
	stand(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	for (byte i = 0; i < 180; i++)
	{
		moveAll(_Positions[BASE], _Positions[SHOULDER], i, _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], RAPIDE);
	}

	//turns wrist rot
	stand(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	for (byte i = 0; i < 180; i++)
	{
		moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], i, _Positions[WRIST_VER], _Positions[GRIPPER], RAPIDE);
	}

	//turns wrist ver
	stand(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	for (byte i = 0; i < 180; i++)
	{
		moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], i, _Positions[GRIPPER], RAPIDE);
	}

	//turns gripper
	stand(posBase, posShoulder, posElbow, posWristRot, posWristVer, posGripper);
	for (byte i = 25; i < 90; i++)
	{
		moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], i, RAPIDE);
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
			moveAll(position, _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], speed);
			break;
		}

		case SHOULDER:
		{
			position = (position > 160) ? 160 : position;
			position = (position < 20) ? 20 : position;
			moveAll(_Positions[BASE], position, _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], speed);
			break;
		}

		case ELBOW:
		{
			moveAll(_Positions[BASE], _Positions[SHOULDER], position, _Positions[WRIST_ROT], _Positions[WRIST_VER], _Positions[GRIPPER], speed);
			break;
		}

		case WRIST_ROT:
		{
			moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], position, _Positions[WRIST_VER], _Positions[GRIPPER], speed);
			break;
		}

		case WRIST_VER:
		{
			moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], position, _Positions[GRIPPER], speed);
			break;
		}

		case GRIPPER:
		{
			position = (position > 90) ? 90 : position;
			position = (position < 25) ? 25 : position;
			moveAll(_Positions[BASE], _Positions[SHOULDER], _Positions[ELBOW], _Positions[WRIST_ROT], _Positions[WRIST_VER], position, speed);
			break;
		}
	}
}

void _Braccio::openGripper(byte &posGripper, const SPEED speed)
{
  	_Positions[GRIPPER] = 90;
    posGripper			= 90;
  	moveMotor(GRIPPER, _Positions[GRIPPER], speed);
}

void _Braccio::closeGripper(byte &posGripper, const SPEED speed)
{
  	_Positions[GRIPPER] = 25;
	posGripper	 		= 25;
  	moveMotor(GRIPPER, _Positions[GRIPPER], speed);
}
