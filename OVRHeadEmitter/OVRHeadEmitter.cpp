// OVRHeadEmitter.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <string>
#include "OVR.h"
#include "PrecisionTest.h"
#include "OrientationReporter.h"

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

int _tmain(int argc, _TCHAR* argv[])
{
	System::Init(Log::ConfigureDefaultLog(LogMask_All));

	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	
	if (!pHMD) {
		cout << "Could not discover HMD." << endl;
		cin.ignore();
		exit(0);
	}


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

	PrecisionTest test(2);

	OrientationReporter reporter;

	string input;
	cout << "Enter the address (<addess>:<port>) of the gimbal endpoint: ";
	cin >> input;

	if (!reporter.Connect(input.c_str())) {
		pSensor.Clear();
		pHMD.Clear();
		cout << "could not create curl object." << endl;
		cin.ignore();
		exit(0);
	}

	while (!done) {
		Quatf orientation = sFusion.GetOrientation();

		float yaw = 0.0f;
		float pitch = 0.0f;
		float roll = 0.0f;

		orientation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

		if (test.Test(pitch, yaw, roll)) {

			cout << "Pitch: " << RadToDegree(pitch) << " Yaw: " << RadToDegree(yaw) << 
				" Roll: " << RadToDegree(roll) << endl;

			reporter.Report(RadToDegree(pitch), RadToDegree(yaw));
		}

		Sleep(SampleMilliseconds);
	}

	cout << "Resources released, press any key to exit." << endl;
	cin.ignore();
	pSensor.Clear();
	pHMD.Clear();
	return 0;
}

