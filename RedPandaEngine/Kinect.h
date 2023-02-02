#pragma once
#include <windows.h>
//kinect
#include <NuiApi.h>
#include <NuiSkeleton.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <glm/glm.hpp>
#include <vector>
#include "util.h"

namespace Kinect {
	class Bone {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
	class Skeleton {
		glm::vec4 joints[20];
		Bone bones[19];
		bool tracked;
		int NotTrackedCount=0;
		int NotTrackedThreshold = 10;
	public:
		void Update(NUI_SKELETON_DATA data) {
			for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
			{
				joints[i] = glm::vec4(data.SkeletonPositions[i].x, data.SkeletonPositions[i].y, data.SkeletonPositions[i].z, data.SkeletonPositions[i].w);
			}
			if (data.eTrackingState == NUI_SKELETON_TRACKED) {
				tracked = true;
			}
			else {
				if (NotTrackedCount == NotTrackedThreshold) {
					NotTrackedCount = 0;
					tracked = false;
				}
				else {
					NotTrackedCount++;
				}
			}
		}
		bool IsTracked() {
			return tracked;
		}

		glm::vec4 GetJoint(int i) {
			return joints[i];
		}
		glm::vec4* GetJointRef(int i) {
			return &joints[i];
		}
		void DrawJoints(float PointSize,std::function<glm::vec4(glm::vec4 point)> call) {
			glPointSize(PointSize);
			glBegin(GL_POINTS);
			bool vib = false;
			for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)
			{
				glm::vec4 point = call(joints[j]);
				glVertex3f(point.x, point.y, point.z);
			}
			glEnd();
		}

		void DrawBones(float LineWidth,std::function<Bone(Bone)> call) {

		}
	};

	//a abstraction of the Kinect SDK's sensor
	class Sensor {
		INuiSensor* sensor;
		HANDLE colorStream, depthStream;
		NUI_SKELETON_FRAME skeletonFrame;
		NUI_IMAGE_FRAME colorFrame, depthFrame;
		HANDLE NextColorFrameEvent, NextDepthFrameEvent;
	public:
		//constructor		
		Sensor() {
			//get the sensor and check if successful
			int numSensors = 0;
			NuiGetSensorCount(&numSensors);
			if (numSensors == 0) {
				PLOGE_(Util::Logs::Error) << "No Kinect Sensor Found";
				throw "No Kinect Sensor Found";
			}
			NuiCreateSensorByIndex(0, &sensor);
			//initialize the sensor
			sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_AUDIO | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_HIGH_QUALITY_COLOR);
			//open the sensor
			NextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,NUI_IMAGE_RESOLUTION_1280x960,0,2,NextColorFrameEvent,&colorStream);
			NextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			// Open a depth image stream to receive depth frames
			HRESULT hr=sensor->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_DEPTH,
				NUI_IMAGE_RESOLUTION_640x480,
				0,
				2,
				NextDepthFrameEvent,
				&depthStream);
			sensor->NuiSkeletonTrackingEnable(NULL, 0);
			sensor->NuiCameraElevationSetAngle(0);
		}

		//destructor
		~Sensor() {
			sensor->NuiShutdown();
		}

		//get the color frame
		void getColorFrame(std::function<void(void * frame,int size)> useFrame) {
			HRESULT hr;
			NUI_IMAGE_FRAME imageFrame;

			if (WAIT_OBJECT_0 == WaitForSingleObject(NextColorFrameEvent, 0))
			{
				// Attempt to get the color frame
				hr = sensor->NuiImageStreamGetNextFrame(colorStream, 0, &imageFrame);
				if (FAILED(hr))
				{
					return;
				}

				INuiFrameTexture* pTexture = imageFrame.pFrameTexture;
				NUI_LOCKED_RECT LockedRect;

				// Lock the frame data so the Kinect knows not to modify it while we're reading it
				pTexture->LockRect(0, &LockedRect, NULL, 0);

				// Make sure we've received valid data
				if (LockedRect.Pitch != 0)
				{
					useFrame((void*)LockedRect.pBits, LockedRect.size);
				}

				// We're done with the texture so unlock it
				pTexture->UnlockRect(0);

				// Release the frame
				sensor->NuiImageStreamReleaseFrame(colorStream, &imageFrame);
			}
		}

		//get the depth frame
		void getDepthFrame(std::function<void(void* frame, int size)> useFrame) {
			HRESULT hr;
			NUI_IMAGE_FRAME imageFrame;
			void* depthRGBX = malloc(640* 480 * 4);

			// Attempt to get the depth frame
			hr = sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame);
			if (FAILED(hr))
			{
				delete depthRGBX;
				return;
			}

			BOOL nearMode;
			INuiFrameTexture* pTexture;
			// Get the depth image pixel texture
			hr = sensor->NuiImageFrameGetDepthImagePixelFrameTexture(depthStream, &imageFrame, &nearMode, &pTexture);
			if (FAILED(hr))
			{
				goto ReleaseFrame;
			}

			NUI_LOCKED_RECT LockedRect;

			// Lock the frame data so the Kinect knows not to modify it while we're reading it
			pTexture->LockRect(0, &LockedRect, NULL, 0);

			// Make sure we've received valid data
			if (LockedRect.Pitch != 0)
			{
				// Get the min and max reliable depth for the current frame
				int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
				int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;

				BYTE* rgbrun = (BYTE*)depthRGBX;
				const NUI_DEPTH_IMAGE_PIXEL* pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL*>(LockedRect.pBits);

				// end pixel is start + width*height - 1
				const NUI_DEPTH_IMAGE_PIXEL* pBufferEnd = pBufferRun + (640* 480);

				while (pBufferRun < pBufferEnd)
				{
					// discard the portion of the depth that contains only the player index
					USHORT depth = pBufferRun->depth;

					// To convert to a byte, we're discarding the most-significant
					// rather than least-significant bits.
					// We're preserving detail, although the intensity will "wrap."
					// Values outside the reliable depth range are mapped to 0 (black).

					// Note: Using conditionals in this loop could degrade performance.
					// Consider using a lookup table instead when writing production code.
					BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? depth % 256 : 0);

					// Write out blue byte
					*(rgbrun++) = intensity;

					// Write out green byte
					*(rgbrun++) = intensity;

					// Write out red byte
					*(rgbrun++) = intensity;

					// We're outputting BGR, the last byte in the 32 bits is unused so skip it
					// If we were outputting BGRA, we would write alpha here.
					++rgbrun;

					// Increment our index into the Kinect's depth buffer
					++pBufferRun;
				}

				// Draw the data with Direct2D
				useFrame(depthRGBX, 640* 480 * 4);
			}

			// We're done with the texture so unlock it
			pTexture->UnlockRect(0);

			pTexture->Release();

		ReleaseFrame:
			// Release the frame
			delete depthRGBX;
			sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
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

		void DrawBone(int i, int j, int k, float LineWidth = 5) {
			NUI_SKELETON_DATA skeleton = getSkeleton(i);
			glm::vec3 point1 = glm::vec3(skeleton.SkeletonPositions[j].x, skeleton.SkeletonPositions[j].y, skeleton.SkeletonPositions[j].z);
			glm::vec3 point2 = glm::vec3(skeleton.SkeletonPositions[k].x, skeleton.SkeletonPositions[k].y, skeleton.SkeletonPositions[k].z);
			point1 -= glm::vec3(0, 0, 2);
			point2 -= glm::vec3(0, 0, 2);
			glm::vec3 color = Util::HSVtoRGB(1 / i, 1, .5);
			glLineWidth(LineWidth);
			glBegin(GL_LINES);
			glColor3f(color.x, color.y, color.z);
			glVertex3f(point1.x, point1.y, point1.z);
			glVertex3f(point2.x, point2.y, point2.z);
			glEnd();
		}
		
		void DrawSkeleton(int i, float PointSize, bool DrawJoints = true, bool DrawBones = true) {
			//get Skeleton and draw
			NUI_SKELETON_DATA skeleton = getSkeleton(i);
			if (DrawJoints) {
				for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)
				{
					glm::vec3 point = glm::vec3(skeleton.SkeletonPositions[j].x, skeleton.SkeletonPositions[j].y, skeleton.SkeletonPositions[j].z);
					point -= glm::vec3(0, 0, 2);
					glm::vec3 color = Util::HSVtoRGB(1 / i, 1, .5);

					glPointSize(PointSize);
					glBegin(GL_POINTS);
					glColor3f(color.x, color.y, color.z);
					glVertex3f(point.x, point.y, point.z);
					glEnd();
				}
			}
			if (DrawBones) {
				// Torso
				DrawBone(i, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
				DrawBone(i, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
				DrawBone(i, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
				DrawBone(i, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
				DrawBone(i, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
				DrawBone(i, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
				DrawBone(i, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

				// Left Arm
				DrawBone(i, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
				DrawBone(i, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
				DrawBone(i, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

				// Right Arm
				DrawBone(i, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
				DrawBone(i, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
				DrawBone(i, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

				// Left Leg
				DrawBone(i, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
				DrawBone(i, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
				DrawBone(i, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);
			}
		}
		//set angle
		void setAngle(int angle) {
			sensor->NuiCameraElevationSetAngle(angle);
		}

		//get angle
		int getAngle() {
			LONG angle;
			sensor->NuiCameraElevationGetAngle(&angle);
			return angle;
		}
		
	};
}