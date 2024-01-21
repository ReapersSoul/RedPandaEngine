

class Motor{
	//##Values at nominal voltage
	//measured in voltage
	double NominalVoltage;
	//measured in rpm
	double FreeSpeed;
	//measured in Amps
	double FreeCurrent;
	//measured in rpm
	double NominalSpeed;
	//measured in Newton Meters
	double NominalTorque;
	//measured in Amps
	double NominalCurrent;
	//measured in Newton Meters	
	double StallTorque;
	//measured in Amps
	double StallCurrent;
	//measured in percent
	double MaxEfficiency;

	//##Characteristics
	//measured in ohms
	double TerminalResistance;
	//measured in henrys
	double TerminalInductance;
	//measured in Newton Meters/Amp
	double TorqueConstant;
	//measured in rpm/Volt
	double SpeedConstant;
	//measured in rpm/Newton Meter
	double SpeedTorqueGradient;
	//measured in milliseconds
	double MechanicalTimeConstant;
	//measured in g*m^2
	double RotorInertia;

	//##Thermal Data
	//measured in K/W
	double ThermalResistanceHousingAmbient;
	//measured in K/W
	double ThermalResistanceWindingHousing;
	//measured in seconds
	double ThermalTimeConstantWinding;
	//measured in seconds
	double ThermalTimeConstantMotor;
	//measured in celsius
	double AmbientTemperature;
	//measured in celsius
	double MaxWindingTemperature;

	//##Mechanical Data
	//measured in rpm
	double MaxSpeed;
	//measured in Millimeters
	double AxialPlay;
	//measured in Newton
	double MaxAxialLoad;
	//measured in Newton
	double MaxRadialLoad;
	//measured in grams
	double weight;
	//measured in millimeters
	double HousingRadius;
	//measured in millimeters
	double HousingLength;
	//measured in millimeters
	double ShaftRadius;
	//measured in millimeters
	double ShaftLength;

	//##Properties
	//measured in degrees
	double CurrentRotation;
	
public:
	Motor(double NominalVoltage, double FreeSpeed, double FreeCurrent, double NominalSpeed, double NominalTorque, double NominalCurrent, double StallTorque, double StallCurrent, double TerminalResistance, double TerminalInductance, double TorqueConstant, double SpeedConstant, double SpeedTorqueGradient, double MechanicalTimeConstant, double weight);
	Motor();
	//getters
	double getNominalVoltage();
	double getFreeSpeed();
	double getFreeCurrent();
	double getNominalSpeed();
	double getNominalTorque();
	double getNominalCurrent();
	double getStallTorque();
	double getStallCurrent();
	double getMaxEfficiency();
	double getTerminalResistance();
	double getTerminalInductance();
	double getTorqueConstant();
	double getSpeedConstant();
	double getSpeedTorqueGradient();
	double getMechanicalTimeConstant();
	double getRotorInertia();
	double getThermalResistanceHousingAmbient();
	double getThermalResistanceWindingHousing();
	double getThermalTimeConstantWinding();
	double getThermalTimeConstantMotor();
	double getAmbientTemperature();
	double getMaxWindingTemperature();
	double getMaxSpeed();
	double getAxialPlay();
	double getMaxAxialLoad();
	double getMaxRadialLoad();
	double getWeight();

	//setters
	void setNominalVoltage(double NominalVoltage);
	void setFreeSpeed(double FreeSpeed);
	void setFreeCurrent(double FreeCurrent);
	void setNominalSpeed(double NominalSpeed);
	void setNominalTorque(double NominalTorque);
	void setNominalCurrent(double NominalCurrent);
	void setStallTorque(double StallTorque);
	void setStallCurrent(double StallCurrent);
	void setMaxEfficiency(double MaxEfficiency);
	void setTerminalResistance(double TerminalResistance);
	void setTerminalInductance(double TerminalInductance);
	void setTorqueConstant(double TorqueConstant);
	void setSpeedConstant(double SpeedConstant);
	void setSpeedTorqueGradient(double SpeedTorqueGradient);
	void setMechanicalTimeConstant(double MechanicalTimeConstant);
	void setRotorInertia(double RotorInertia);
	void setThermalResistanceHousingAmbient(double ThermalResistanceHousingAmbient);
	void setThermalResistanceWindingHousing(double ThermalResistanceWindingHousing);
	void setThermalTimeConstantWinding(double ThermalTimeConstantWinding);
	void setThermalTimeConstantMotor(double ThermalTimeConstantMotor);
	void setAmbientTemperature(double AmbientTemperature);
	void setMaxWindingTemperature(double MaxWindingTemperature);
	void setMaxSpeed(double MaxSpeed);
	void setAxialPlay(double AxialPlay);
	void setMaxAxialLoad(double MaxAxialLoad);
	void setMaxRadialLoad(double MaxRadialLoad);
	void setWeight(double weight);

	//draw
	void draw();

	//destructor
	~Motor();

};