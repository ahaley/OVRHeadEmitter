// OVRHeadEmitter.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include "OVR.h"
using namespace std;
using namespace OVR;

Ptr<DeviceManager> pManager;
Ptr<HMDDevice> pHMD;
Ptr<SensorDevice> pSensor;

DWORD SampleMilliseconds = 5;

bool done = false;

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	if (fdwCtrlType == CTRL_C_EVENT) {
		cout << "Received CTRL-C..." << endl;
		pSensor.Clear();
		pHMD.Clear();
		done = true;
		return true;
	}
	return false;
}

class PrecisionTest
{
public:
	PrecisionTest(int precision)
	{
		_precision = pow(10.0f, precision);
	}

	BOOL Test(float x, float y, float z)
	{
		int newX = x * _precision;
		int newY = y * _precision;
		int newZ = z * _precision;

		bool result = newX == _x && newY == _y && newZ == _z;
		
		if (result) {
			_x = newX;
			_y = newY;
			_z = newZ;
		}
		return result;
	}

private:
	int _x;
	int _y;
	int _z;
	int _precision;
};


int _tmain(int argc, _TCHAR* argv[])
{
	System::Init(Log::ConfigureDefaultLog(LogMask_All));

	
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	
	pSensor = *pHMD->GetSensor();
	
	if (!pHMD && !pSensor) {
		cout << "Oculus Rift not detected." << endl;
		cin.ignore();
		exit(0);	
	}
	cout << "Oculus rift detected." << endl;

	SensorFusion sFusion;
	sFusion.AttachToSensor(pSensor);

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );

	PrecisionTest test(3);

	while (!done) {

		Quatf orientation = sFusion.GetOrientation();

		float yaw = 0.0f;
		float pitch = 0.0f;
		float roll = 0.0f;

		orientation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

		if (test.Test(pitch, yaw, roll)) {

			cout << "Pitch: " << pitch << " Yaw: " << RadToDegree(yaw) << 
				", Roll: " << RadToDegree(roll) << endl;
		}

		Sleep(SampleMilliseconds);
	}

	cout << "Resources released, press any key to exit." << endl;
	cin.ignore();
	return 0;
}

