#pragma once
#include "DEVICE.h"
#include <Lovense.h>
#include <LovenseDef.h>
#include <LovenseToyInfo.h>
#include <map>
#include <functional>
#include "Language.h"
#include <thread>
#include <chrono>
#include <json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

static enum LightStatus {
	L_on,
	L_off,
	L_flashing
};

std::string ParseToyType(std::string s) {
	//J:23 : D5BD303C275A;
	size_t pos = 0;
	pos = s.find(":");
	std::string token = s.substr(0, pos);
	s.erase(0, pos + std::string(":").length());

	pos = s.find(":");
	std::string id = s.substr(0, pos);
	s.erase(0, pos + std::string(":").length());
	
	//use json to find
	std::ifstream f("ToyTypes.json");
	json data = json::parse(f);
	if (!data["Types"][token][id].empty()) {
		return data["Types"][token][id].get<std::string>();
	}
	else {
		data["Types"][token][id] = "";
		std::ofstream o("ToyTypes.json");
		o << data;
		return "UNKNOWN";
	}
}
std::string ParseToyName(std::string s) {
	//use json to find
	std::ifstream f("ToyTypes.json");
	json data = json::parse(f);
	if (!data["Names"][s].empty())
	{
		return data["Names"][s].get<std::string>();
	}
	else {
		data["Names"][s] = "";
		std::ofstream o("ToyTypes.json");
		o << data;
		return "UNKNOWN";
	}
}

class Toy {
private:
	std::string ID, Name;
	int vibration_level = 0;
	int vibration_level_2 = 0;
	int rotation_level = 0;
	bool rotation_dir_clockwise = false;
	int battery_level = 100;
	bool connected = false;
	bool is_tracking = false;
	LightStatus light_status = L_off;
	LightStatus Alight_status = L_off;
	CLovenseToyManager* toyManager;
	std::map<std::string, Toy*> TrackingTargets;

	//bools for blocking requests
	bool RequestDeviceType_Returned=false;
	bool RequestFlashLight_Returned = false;
	bool RequestLightOn_Returned = false;
	bool RequestLightOff_Returned = false;
	bool RequestRotateDevice_Returned = false;
	bool RequestStartTracking_Returned = false;
	bool RequestStopTracking_Returned = false;
	bool RequestChangeRotationDir_Returned = false;
	bool RequestVibrate_Returned = false;
	bool RequestShutOff_Returned = false;
	bool RequestConnect_Returned = false;
	bool RequestDisConnect_Returned = false;
	bool RequestSucktion_Returned = false;
	bool RequestBattery_Returned = false;
	
public:

	//constructor
	Toy(CLovenseToyManager* tm, st_lovense_toy_info* info) {
		toyManager = tm;
		ID = info->toy_id;
		Name = ParseToyName(std::string(info->toy_name));
		connected = info->toy_connected;
		battery_level = info->toy_battery;
	}
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
	void SetLightStatus(LightStatus status) {
		light_status = status;
	}
	void SetTracking(bool track) {
		is_tracking = track;
	}
	void SetTrackingTarget(std::string id, Toy* t) {
		TrackingTargets.insert(std::pair<std::string, Toy*>(id, t));
	}
	void SetRequestDeviceType_Returned(bool b) {
		RequestDeviceType_Returned = b;
	}
	void SetRequestFlashLight_Returned(bool b) {
		RequestFlashLight_Returned = b;
	}
	void SetRequestLightOn_Returned(bool b) {
		RequestLightOn_Returned = b;
	}
	void SetRequestLightOff_Returned(bool b) {
		RequestLightOff_Returned = b;
	}
	void SetRequestRotateDevice_Returned(bool b) {
		RequestRotateDevice_Returned = b;
	}
	void SetRequestStartTracking_Returned(bool b) {
		RequestStartTracking_Returned = b;
	}
	void SetRequestStopTracking_Returned(bool b) {
		RequestStopTracking_Returned = b;
	}
	void SetRequestChangeRotationDir_Returned(bool b) {
		RequestChangeRotationDir_Returned = b;
	}
	void SetRequestVibrate_Returned(bool b) {
		RequestVibrate_Returned = b;
	}
	void SetRequestShutOff_Returned(bool b) {
		RequestShutOff_Returned = b;
	}
	void SetRequestConnect_Returned(bool b) {
		RequestConnect_Returned = b;
	}
	void SetRequestDisConnect_Returned(bool b) {
		RequestDisConnect_Returned = b;
	}
	void SetRequestSucktion_Returned(bool b) {
		RequestSucktion_Returned = b;
	}
	void SetRequestBattery_Returned(bool b) {
		RequestBattery_Returned = b;
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
	LightStatus GetLightStatus() {
		return light_status;
	}
	bool GetTracking() {
		return is_tracking;
	}
	Toy* GetTrackingTarget(std::string id) {
		return TrackingTargets.at(id);
	}
	//requests
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
	
	//blocking requests
	void BlockingRequestDeviceType() {
		//set bool
		RequestDeviceType_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_GET_DEVICE_TYPE, 0);
		while (!RequestDeviceType_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestFlashLight() {
		RequestFlashLight_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_FLASH, 0);
		while (!RequestFlashLight_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestLightOn() {
		RequestLightOn_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_LIGHT_ON, 0);
		while (!RequestLightOn_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	void BlockingRequestLightOff() {
		RequestLightOff_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_LIGHT_OFF, 0);
		while (!RequestLightOff_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestRotateDevice(int level, int clockwise = 0) {
		RequestRotateDevice_Returned = false;
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
		while (!RequestRotateDevice_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestStartTracking() {
		RequestStartTracking_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_START_MOVE, 0);
		while (!RequestStartTracking_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestStopTracking() {
		RequestStopTracking_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_STOP_MOVE, 0);
		while (!RequestStopTracking_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestChangeRotationDir() {
		RequestChangeRotationDir_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_ROTATE_CHANGE, 0);
		while (!RequestChangeRotationDir_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestVibrate(int level, int Vibe = 0) {
		RequestVibrate_Returned = false;
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
		while (!RequestVibrate_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestShutOff() {
		RequestShutOff_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_POWER_OFF, 0);
		while (!RequestShutOff_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestConnect() {
		RequestConnect_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_CONNECT_TOY, 0);
		toyManager->ConnectToToy(ID.c_str());
		while (!RequestConnect_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	void BlockingRequestDisConnect() {
		RequestDisConnect_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_DISCONNECT_TOY, 0);
		toyManager->DisConnectedToy(ID.c_str());
		while (!RequestDisConnect_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestSucktion(int mode, int seconds) {
		RequestSucktion_Returned = false;
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
		while (!RequestSucktion_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void BlockingRequestBattery() {
		RequestBattery_Returned = false;
		toyManager->SendCommand(ID.c_str(), CLovenseToy::CmdType::COMMAND_GET_BATTERY, 0);
		while (!RequestBattery_Returned) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	
	
	//result
	void OnCmdResult(CLovenseToy::CmdType cmd, const char* result, CLovenseToy::Error errorCode) {
		if (errorCode == CLovenseToy::Error::TOYERR_SUCCESS) {
			switch (cmd)
			{
			case CLovenseToy::CmdType::COMMAND_GET_DEVICE_TYPE:
				RequestDeviceType_Returned = true;
				Name = ParseToyType(std::string(result));
				break;
			case CLovenseToy::CmdType::COMMAND_FLASH:
				light_status = LightStatus::L_flashing;
				break;
			case CLovenseToy::CmdType::COMMAND_LIGHT_ON:
				RequestLightOn_Returned = true;
				light_status = LightStatus::L_on;
				break;
			case CLovenseToy::CmdType::COMMAND_LIGHT_OFF:
				RequestLightOff_Returned = true;
				light_status = LightStatus::L_off;
				break;
			case CLovenseToy::CmdType::COMMAND_ROTATE:
				RequestRotateDevice_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_ROTATE_CLOCKWISE:
				RequestRotateDevice_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_ROTATE_ANTI_CLOCKWISE:
				RequestRotateDevice_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_ROTATE_CHANGE:
				RequestChangeRotationDir_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_VIBRATE:
				RequestVibrate_Returned = true;
				vibration_level = atoi(result);
				break;
			case CLovenseToy::CmdType::COMMAND_VIBRATE1:
				RequestVibrate_Returned = true;
				vibration_level = atoi(result);
				break;
			case CLovenseToy::CmdType::COMMAND_VIBRATE2:
				RequestVibrate_Returned = true;
				vibration_level_2 = atoi(result);
				break;
			case CLovenseToy::CmdType::COMMAND_POWER_OFF:
				RequestShutOff_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_AIR_IN:
				RequestSucktion_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_AIR_OUT:
				RequestSucktion_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_AIR_AUTO:
				RequestSucktion_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_GET_BATTERY:
				RequestBattery_Returned = true;
				battery_level = atoi(result);
				break;
			case CLovenseToy::CmdType::COMMAND_START_MOVE:
				RequestStartTracking_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_STOP_MOVE:
				RequestStopTracking_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_ALIGHT_OFF:
				Alight_status = LightStatus::L_off;
				break;
			case CLovenseToy::CmdType::COMMAND_ALIGHT_ON:
				Alight_status = LightStatus::L_on;
				break;
			case CLovenseToy::CmdType::COMMAND_CONNECT_TOY:
				if (result = "OK")
					connected = true;
				else
					connected = false;
				RequestConnect_Returned = true;
				break;
			case CLovenseToy::CmdType::COMMAND_DISCONNECT_TOY:
				if (result = "OK")
					connected = true;
				else
					connected = false;
				RequestDisConnect_Returned = true;
				break;
			};
		}
	}

	void UpdateToyInfo() {
		//make blocking requests
		BlockingRequestDeviceType();
		BlockingRequestBattery();
	}
	
};

class ToyManager :public ILovenseSDKNotify {
	CLovenseToyManager* toyManager;
	std::map<std::string, Toy*> Toys;
	bool searching = false;
public:
	ToyManager() {
		toyManager = GetLovenseToyManager();
		toyManager->SetDeveloperToken("SCfh7CamO5irTBgvB53z2hdAYjmq2SJLiUNFLrB1SvOQu9hyemQL5lUimQZTeIqT");
		toyManager->RegisterEventCallBack(this);
	}
	//deconstructor
	~ToyManager() {
		//toys
		for (auto& toy : Toys) {
			delete toy.second;
		}
		//manager
		ReleaseLovenseToyManager();
	}
	//callbacks
	void LovenseDidSearchStart() {
		searching = true;
	}

	/*Call when toy searching toy*/
	void LovenseSearchingToys(lovense_toy_info_t* info) {
		Toy* toy = new Toy(toyManager,info);
		Toys.insert(std::pair<std::string, Toy*>(toy->GetID(), toy));
	}

	/*Call when Something went wrong*/
	void LovenseErrorOutPut(int errorCode, const char* errorMsg) {
		
	}

	/*Call when toy search end*/
	void LovenseDidSearchEnd() {
		searching = false;
	}

	/*Call when send cmd start*/
	void LovenseDidSendCmdStart() {
		
	}

	/*Call when send cmd return*/
	void LovenseSendCmdResult(const char* szToyID, CLovenseToy::CmdType cmd, const char* result, CLovenseToy::Error errorCode) {
		Toys.at(szToyID)->OnCmdResult(cmd, result, errorCode);
	}

	/*Call when send cmd end*/
	void LovenseDidSendCmdEnd() {
		
	}

	/*Call when toy connected, or disconnected*/
	void LovenseToyConnectedStatus(const char* szToyID, bool isConnected) {
		Toys.at(szToyID)->SetConnected(isConnected);
	}

	//methods
	void StartSearch() {
		toyManager->StartSearchToy();
	}
	void StopSearch() {
		toyManager->StopSearchToy();
	}
	void Blocking_SearchFor(int seconds) {
		toyManager->StartSearchToy();
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
		toyManager->StopSearchToy();
		while (searching) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	void NonBlocking_SearchFor(int seconds) {
		toyManager->StartSearchToy();
		std::thread([this, seconds]() {
			std::this_thread::sleep_for(std::chrono::seconds(seconds));
			toyManager->StopSearchToy();
			}).detach();
	}
	void PrintToyList() {
		//print toy name, id, and toy type
		for (auto& toy : Toys) {
			std::cout << "Toy Name: " << toy.second->GetName() << std::endl;
			std::cout << "Toy ID: " << toy.second->GetID() << std::endl;
			//std::cout << "Toy Type: " << Toy::ToyTypeToString(toy.second->GetDeviceType()) << std::endl;
			std::cout << "Toy Connected: " << toy.second->GetConnected() << std::endl;
			std::cout << "Toy Battery: " << toy.second->GetBatteryLevel() << std::endl;
			std::cout << "Toy Vibration: " << toy.second->GetVibrationLevel() << std::endl;
			std::cout << "Toy Vibration 2: " << toy.second->GetVibrationLevel2() << std::endl;
			//std::cout << "Toy Light: " << Toy::LightStatusToString(toy.second->GetLightStatus()) << std::endl;
			std::cout << std::endl;
		}
	}

	Toy* GetToy(std::string id) {
		return Toys.at(id);
	}
	Toy* GetToy(int index) {
		auto it = Toys.begin();
		std::advance(it, index);
		return it->second;
	}
	
	std::map<std::string, Toy*> GetToys() {
		return Toys;
	}

	void ConnectAll() {
		for (auto& toy : Toys) {
			toy.second->RequestConnect();
		}
	}
	
	void DisconnectAll() {
		for (auto& toy : Toys) {
			toy.second->RequestDisConnect();
		}
	}
	
	void UpdateAllToyInfo() {
		for (auto& toy : Toys) {
			toy.second->UpdateToyInfo();
		}
	}
	
	//vibrate all
	void VibrateAll(int level) {
		for (auto& toy : Toys) {
			toy.second->RequestVibrate(level);
		}
	}
	
	//vibrate all for n seconds
	void VibrateAll(int level, int seconds) {
		for (auto& toy : Toys) {
			toy.second->RequestVibrate(level);
		}
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
		for (auto& toy : Toys) {
			toy.second->RequestVibrate(0);
		}
	}
	//vibrate all for n seconds
	void NonBlockingVibrateAll(int level, int seconds) {
		for (auto& toy : Toys) {
			toy.second->RequestVibrate(level);
		}
		std::thread([this, seconds]() {
			std::this_thread::sleep_for(std::chrono::seconds(seconds));
			for (auto& toy : Toys) {
				toy.second->RequestVibrate(0);
			}
			}).detach();
	}
	
};