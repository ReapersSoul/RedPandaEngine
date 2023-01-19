#pragma once
#include "DEVICE.h"
#include <Lovense.h>
#include <LovenseDef.h>
#include <LovenseToyInfo.h>
#include <map>
#include <functional>
#include "Scripting_Language.h"

class Lovense_Device :public Device {
	LVSToyType Device_Type;
	

};

CLovenseToyManager* toyManager;
class Toy : public Scripting_Language::Tableizable {
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
	//setters
	void SetVibrationLevel(int level) {
		vibration_level = level;
	}
	void SetVibrationLevel2(int level) {
		vibration_level_2 = level;
	}
	void SetRotationLevel(int level) {
		rotation_level = level;
	}
	void SetRotationDir(bool dir) {
		rotation_dir_clockwise = dir;
	}
	void SetBatteryLevel(int level) {
		battery_level = level;
	}
	void SetConnected(bool c) {
		connected = c;
	}
	void SetToyManager(CLovenseToyManager* tm) {
		toyManager = tm;
	}
	void SetID(std::string id) {
		ID = id;
	}
	void SetName(std::string name) {
		Name = name;
	}
	void SetDeviceType(LVSToyType type) {
		device_type = type;
	}
	void SetLightStatus(LightStatus status) {
		light_status = status;
	}
	void SetTracking(bool track) {
		is_tracking = track;
	}
	void SetTrackingTarget(std::string id, Toy* t) {
		TrackingTargets.insert(std::pair<std::string, Toy*>(id, t));
	}
	//getters
	int GetVibrationLevel() {
		return vibration_level;
	}
	int GetVibrationLevel2() {
		return vibration_level_2;
	}
	int GetRotationLevel() {
		return rotation_level;
	}
	bool GetRotationDir() {
		return rotation_dir_clockwise;
	}
	int GetBatteryLevel() {
		return battery_level;
	}
	bool GetConnected() {
		return connected;
	}
	std::string GetID() {
		return ID;
	}
	std::string GetName() {
		return Name;
	}
	LVSToyType GetDeviceType() {
		return device_type;
	}
	LightStatus GetLightStatus() {
		return light_status;
	}
	bool GetTracking() {
		return is_tracking;
	}
	Toy* GetTrackingTarget(std::string id) {
		return TrackingTargets.at(id);
	}
	
	
	
	/*  
		0 : bool
		1 : double
		2 : int
		3 : :std::string
		4 : void *
		5 : Table
		EX:
		std::get<2>(v) = (int)28;
	*/
	//to table
	Scripting_Language::Table ToTable() {
		Scripting_Language::Table t;
		Scripting_Language::Var v;
		std::get<Scripting_Language::e_string>(v) = ID;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("ID", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_string, v)));
		std::get<Scripting_Language::e_string>(v) = Name;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Name", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_string, v)));
		std::get<Scripting_Language::e_int>(v) = vibration_level;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Vibration_Level", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_int, v)));
		std::get<Scripting_Language::e_int>(v) = vibration_level_2;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Vibration_Level_2", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_int, v)));
		std::get<Scripting_Language::e_int>(v) = rotation_level;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Rotation_Level", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_int, v)));
		std::get<Scripting_Language::e_bool>(v) = rotation_dir_clockwise;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Rotation_Dir_Clockwise", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_bool, v)));
		std::get<Scripting_Language::e_int>(v) = battery_level;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Battery_Level", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_int, v)));
		std::get<Scripting_Language::e_bool>(v) = connected;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Connected", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_bool, v)));
		std::get<Scripting_Language::e_bool>(v) = is_tracking;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Is_Tracking", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_bool, v)));
		std::get<Scripting_Language::e_int>(v) = light_status;
		t.data.insert(std::pair<std::string, std::pair<int, Scripting_Language::Var>>("Light_Status", std::pair<int, Scripting_Language::Var>(Scripting_Language::e_int, v)));
		return t;
	}
	/*
	0 : bool
	1 : double
	2 : int
	3 : :std::string
	4 : void *
	5 : Table
	EX:
	std::get<2>(v) = (int)28;
	*/
	//from table
	void FromTable(Scripting_Language::Table t) {
		Scripting_Language::Var v;
		v = t.data["ID"].second;
		ID = std::get<Scripting_Language::e_string>(v);
		v = t.data["Name"].second;
		Name = std::get<Scripting_Language::e_string>(v);
		v = t.data["Vibration_Level"].second;
		vibration_level = std::get<Scripting_Language::e_int>(v);
		v = t.data["Vibration_Level_2"].second;
		vibration_level_2 = std::get<Scripting_Language::e_int>(v);
		v = t.data["Rotation_Level"].second;
		rotation_level = std::get<Scripting_Language::e_int>(v);
		v = t.data["Rotation_Dir_Clockwise"].second;
		rotation_dir_clockwise = std::get<Scripting_Language::e_bool>(v);
		v = t.data["Battery_Level"].second;
		battery_level = std::get<Scripting_Language::e_int>(v);
		v = t.data["Connected"].second;
		connected = std::get<Scripting_Language::e_bool>(v);
		v = t.data["Is_Tracking"].second;
		is_tracking = std::get<Scripting_Language::e_bool>(v);
		v = t.data["Light_Status"].second;
		light_status = (LightStatus)std::get<Scripting_Language::e_int>(v);
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