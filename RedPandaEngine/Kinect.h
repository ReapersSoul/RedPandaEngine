#pragma once
#include <windows.h>
//kinect
#include <NuiApi.h>
#include <NuiSkeleton.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <glm/glm.hpp>
#include <vector>

namespace Kinect {
	//a cpp inplimentation of the Kinect SDK's sensor
	class Sensor {
		INuiSensor* sensor;
		HANDLE colorStream, depthStream;
		NUI_SKELETON_FRAME skeletonFrame;
		NUI_IMAGE_FRAME colorFrame, depthFrame;
	public:
		//constructor		
		Sensor() {
			//get the sensor and check if successful
			int numSensors = 0;
			NuiGetSensorCount(&numSensors);
			if (numSensors == 0) {
				throw "No Kinect Sensor Found";
			}
			NuiCreateSensorByIndex(0, &sensor);
			//initialize the sensor
			sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR);
			//open the sensor
			sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStream);
			sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &depthStream);
			sensor->NuiSkeletonTrackingEnable(NULL, 0);
			sensor->NuiCameraElevationSetAngle(0);
		}
		
		//destructor
		~Sensor() {
			sensor->NuiShutdown();
		}
		
		//get the color frame
		NUI_IMAGE_FRAME getColorFrame() {
			sensor->NuiImageStreamGetNextFrame(colorStream, 0, &colorFrame);
			return colorFrame;
		}
		
		//get the depth frame
		NUI_IMAGE_FRAME getDepthFrame() {
			sensor->NuiImageStreamGetNextFrame(depthStream, 0, &depthFrame);
			return depthFrame;
		}
		
		//get the skeleton frame
		NUI_SKELETON_FRAME getSkeletonFrame() {
			sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
			return skeletonFrame;
		}
		
		//get skeleton
		NUI_SKELETON_DATA getSkeleton(int i) {
			return skeletonFrame.SkeletonData[i];
		}
		
		//set angle
		void setAngle(int angle) {
			sensor->NuiCameraElevationSetAngle(angle);
		}

	};
	
}