#ifndef __BRACCIO__
#define __BRACCIO__

#include <Arduino.h>
#include <Servo.h>

#define LOW_LIMIT_TIMEOUT       2000
#define HIGH_LIMIT_TIMEOUT      6000
#define SOFT_START_CONTROL_PIN  12
#define SOFT_START_DISABLED     -999
#define SOFT_START_DEFAULT      0

enum SPEED {T_LENT = 30, LENT = 25, MOYEN = 20, RAPIDE = 15, T_RAPIDE = 10, NONE = 0};
enum NAME  {BASE, SHOULDER, ELBOW, WRIST_ROT, WRIST_VER, GRIPPER};
    
class _Braccio 
{
    private:
        Servo base;
        Servo shoulder;
        Servo elbow;
        Servo wrist_rot;
        Servo wrist_ver;
        Servo gripper;

        //stores current positions of each motor
        byte _Positions[6];

        //stores pins to attach servos
        byte _Pins[6];
        
        /*
        * This function, used only with the Braccio Shield V4 and greater,
        * turn ON the Braccio softly and save Braccio from brokes.
        * The SOFT_START_CONTROL_PIN is used as a software PWM
        * @param soft_start_level: the minimum value is -70, , default value is 0 (SOFT_START_DEFAULT)
        */
        void _softStart(int soft_start_level);
        
        /*
        * Software implementation of the PWM for the SOFT_START_CONTROL_PIN,HIGH
        * @param high_time: the time in the logic level high
        * @param low_time: the time in the logic level low
        */
        void _softwarePWM(int high_time, int low_time);

    
  	public:
        _Braccio();
    		
        /**
         * Initialises the braccio and makes him stands
         */
        void begin(byte &vBase, byte &vShoulder, byte &vElbow, byte &vWrist_rot, byte &vWrist_ver, byte &vGripper, int soft_start_level = SOFT_START_DEFAULT); 

        /**
         * This function allow the user to control all the servo motors in the Braccio
         * @param Vbase New position of the base
         * @param Vshoulder New position of the shoulder
         * @param Velbow New position of the elbow
         * @param Vwrist_rot New position of the wrist_rot
         * @param Vwrist_ver New position of the wrist_ver
         * @param Vgripper New position of the gripper
         * @param speed Speed of the movement
         */
        void moveAll(byte vBase, byte vShoulder, byte vElbow, byte vWrist_rot, byte vWrist_ver, byte vGripper, const SPEED speed);

        /* Tests the arm around all directions */
        void rangeTest(byte &posBase, byte &posShoulder, byte &posElbow, byte &posWristRot, byte &posWristVer, byte &posGripper);

        /* This function makes the arm stand */
        void stand(byte &posBase, byte &posShoulder, byte &posElbow, byte &posWristRot, byte &posWristVer, byte &posGripper);

        /* Resets positions of every motors */
        void resetPos(byte &posBase, byte &posShoulder, byte &posElbow, byte &posWristRot, byte &posWristVer, byte &posGripper);

        /**
         * Turns the motor specified
         * @param motor Name of the motor
         * @param position New position of this motor
         * @param speed Speed of the movement
         */
        void moveMotor(const NAME name, byte position, const SPEED speed);

        /**
         * Fully opens gripper
         * @param speed Speed of the movement
         */
        void openGripper(byte &posGripper, const SPEED speed);

        /**
         * Fully closes gripper
         * @param speed Speed of the movement
         */
        void closeGripper(byte &posGripper, const SPEED speed);
};

extern _Braccio Braccio;

#endif
