#pragma once
#include "DEVICE.h"
#include <Lovense.h>
#include <LovenseDef.h>
#include <LovenseToyInfo.h>
#include <map>
#include <functional>

class Lovense_Device :public Device {
	LVSToyType Device_Type;
	

};

CLovenseToyManager* toyManager;
class Toy {
	enum LightStatus {
		L_on,
		L_off,
		L_flashing
	};
	LVSToyType device_type;
	std::string ID, Name;
	int vibration_level = 0;
	int vibration_level_2 = 0;
	int rotation_level = 0;
	bool rotation_dir_clockwise = false;
	int battery_level = 100;
	bool connected = false;
	bool is_tracking = false;
	LightStatus light_status = L_off;
	CLovenseToyManager* toyManager;
	std::map<std::string, Toy*> TrackingTargets;

public:

	LVSToyType GetType() {
		return device_type;
	}

	std::string GetID() {
		return ID;
	}

	void AddTrackingTarget(Toy* t) {
		if (TrackingTargets.find(t->GetID()) == TrackingTargets.end()) {
			TrackingTargets.insert(std::pair<std::string, Toy*>(t->GetID(), t));
		}
	}

	void SetToyManager(CLovenseToyManager* mgr) {
		toyManager = mgr;
	}
	void SetConnected(bool b) {
		connected = b;
	}
	bool GetConnected() {
		return connected;
	}
	void SetID(std::string id) {
		ID = id;
	};
	void SetDeviceName(std::string name) {
		Name = name;
	};
	void SetBatteryLevel(int i) {
		battery_level = i;
	}
	void SetDeviceType(LVSToyType type) {
		device_type = type;
	}

	void RequestDeviceType() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_GET_DEVICE_TYPE, 0);
	}

	void RequestFlashLight() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_FLASH, 0);
	}

	void RequestLightOn() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_LIGHT_ON, 0);
	}
	void RequestLightOff() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_LIGHT_OFF, 0);
	}

	void RequestRotateDevice(int clockwise, int level) {
		switch (clockwise)
		{
		case 1:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_ROTATE_CLOCKWISE, level);
			break;
		case 2:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_ROTATE_ANTI_CLOCKWISE, level);
			break;
		default:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_ROTATE, level);
		}
	}

	void RequestStartTracking() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_START_MOVE, 0);
	}

	void RequestStopTracking() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_STOP_MOVE, 0);
	}

	void RequestChangeRotationDir(int level) {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_ROTATE_CHANGE, level);
	}

	void RequestVibrate(int level, int Vibe = 0) {
		switch (Vibe)
		{
		case 1:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_VIBRATE1, level);
			break;
		case 2:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_VIBRATE2, level);
			break;
		default:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_VIBRATE, level);
			break;
		}
	}

	void RequestShutOff() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_POWER_OFF, 0);
	}

	void RequestConnect() {
		toyManager->ConnectToToy(ID.c_str());
	}
	void RequestDisConnect() {
		toyManager->DisConnectedToy(ID.c_str());
	}

	void RequestSucktion(int mode, int seconds) {
		switch (mode)
		{
		case 0:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_AIR_IN, seconds);
			break;
		case 1:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_AIR_OUT, seconds);
			break;
		default:
			toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_AIR_AUTO, seconds);
			break;
		}
	}

	void RequestBattery() {
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_GET_BATTERY, 0);
	}

	std::function<void(Toy* t, CLovenseToy::CmdType cmd, std::string result, CLovenseToy::Error errorCode)> EventProcessor;

	void ProcessEvent(CLovenseToy::CmdType cmd, std::string result, CLovenseToy::Error errorCode) {
		EventProcessor(this, cmd, result, errorCode);
	}
};