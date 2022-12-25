#pragma once
//kinect
#include <NuiApi.h>
#include <NuiSkeleton.h>
#include <NuiSensor.h>
#include <NuiImageCamera.h>

namespace Kinect {
	struct Joint {
		Joint(double x, double y, double z, double w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		Joint(double x) {
			this->x = x;
			this->y = x;
			this->z = x;
			this->w = w;
		}
		Joint() {
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}

		double x, y, z, w;

		void Normalize() {
			double tmp = x + y + z + w;

			x = x / tmp;
			y = y / tmp;
			z = z / tmp;
			w = w / tmp;
		}
		Joint Normalized() {
			double tmp = x + y + z + w;
			Joint ret;
			ret.x = x / tmp;
			ret.y = y / tmp;
			ret.z = z / tmp;
			ret.w = w / tmp;
			return ret;
		}

		Joint operator+ (Joint v) {
			Joint ret;
			ret.x = this->x + v.x;
			ret.y = this->y + v.y;
			ret.z = this->z + v.z;
			ret.w = this->w + v.w;
			return ret;
		}
		Joint operator- (Joint v) {
			Joint ret;
			ret.x = this->x - v.x;
			ret.y = this->y - v.y;
			ret.z = this->z - v.z;
			ret.w = this->w - v.w;
			return ret;
		}
		Joint operator- () {
			x = -x;
			y = -y;
			z = -z;
			w = -w;
			return *this;
		}
		Joint operator* (Joint v) {
			Joint ret;
			ret.x = this->x * v.x;
			ret.y = this->y * v.y;
			ret.z = this->z * v.z;
			ret.w = this->w * v.w;
			return ret;
		}
		template <typename O>
		Joint operator/ (O v) {
			Joint ret;
			ret.x = this->x / v;
			ret.y = this->y / v;
			ret.z = this->z / v;
			ret.w = this->w / v;
			return ret;
		}
		template <typename O>
		Joint operator* (O v) {
			Joint ret;
			ret.x = this->x * v;
			ret.y = this->y * v;
			ret.z = this->z * v;
			ret.w = this->w * v;
			return ret;
		}
		template <typename O>
		Joint operator+ (O v) {
			Joint ret;
			ret.x = this->x + v;
			ret.y = this->y + v;
			ret.z = this->z + v;
			ret.w = this->w + v;
			return ret;
		}
		template <typename O>
		Joint operator- (O v) {
			Joint ret;
			ret.x = this->x - v;
			ret.y = this->y - v;
			ret.z = this->z - v;
			ret.w = this->w - v;
			return ret;
		}

		Joint negate() {
			Joint v = *this;
			v.x = -v.x;
			v.y = -v.y;
			v.z = -v.z;
			v.w = -v.w;
			return v;
		}
	};

	struct Bone {
		Joint Start;
		Joint End;
		void Update(Joint s, Joint e) {
			Start = s; End = e;
		}
		Joint Normalize() {
			Joint ret = End - Start;
			//ret = ret.Normalize();
			return ret.negate();
		}
	};

	typedef Joint BoneNormal;

	struct Skelly {
		bool Tracked = false;
		Joint HandL;
		Joint HandR;
		Joint WristL;
		Joint WristR;
		Joint ElbowL;
		Joint ElbowR;
		Joint ShoulderL;
		Joint ShoulderR;
		Joint ShoulderC;
		Joint Spine;
		Joint HipL;
		Joint HipR;
		Joint HipC;
		Joint KneeL;
		Joint KneeR;
		Joint AnkleL;
		Joint AnkleR;
		Joint FootL;
		Joint FootR;
		Joint Head;
		Bone WristHandL;
		Bone WristHandR;
		Bone ElbowWristL;
		Bone ElbowWristR;
		Bone ShoulderElbowL;
		Bone ShoulderElbowR;
		Bone ShoulderCShoulderL;
		Bone ShoulderCShoulderR;
		Bone ShoulderCHead;
		Bone SpineShoulderC;
		Bone SpineHipC;
		Bone HipCHipL;
		Bone HipCHipR;
		Bone HipKneeL;
		Bone HipKneeR;
		Bone KneeAnkleL;
		Bone KneeAnkleR;
		Bone AnkleFootL;
		Bone AnkleFootR;
		BoneNormal WristHandL_Normal;
		BoneNormal WristHandR_Normal;
		BoneNormal ElbowWristL_Normal;
		BoneNormal ElbowWristR_Normal;
		BoneNormal ShoulderElbowL_Normal;
		BoneNormal ShoulderElbowR_Normal;
		BoneNormal ShoulderCShoulderL_Normal;
		BoneNormal ShoulderCShoulderR_Normal;
		BoneNormal ShoulderCHead_Normal;
		BoneNormal SpineShoulderC_Normal;
		BoneNormal SpineHipC_Normal;
		BoneNormal HipCHipL_Normal;
		BoneNormal HipCHipR_Normal;
		BoneNormal HipKneeL_Normal;
		BoneNormal HipKneeR_Normal;
		BoneNormal KneeAnkleL_Normal;
		BoneNormal KneeAnkleR_Normal;
		BoneNormal AnkleFootL_Normal;
		BoneNormal AnkleFootR_Normal;

		Bone* GetBonesArray() {
			Bone ret[19];
			ret[0] = WristHandL;
			ret[1] = WristHandR;
			ret[2] = ElbowWristL;
			ret[3] = ElbowWristR;
			ret[4] = ShoulderElbowL;
			ret[5] = ShoulderElbowR;
			ret[6] = ShoulderCShoulderL;
			ret[7] = ShoulderCShoulderR;
			ret[8] = ShoulderCHead;
			ret[9] = SpineShoulderC;
			ret[10] = SpineHipC;
			ret[11] = HipCHipL;
			ret[12] = HipCHipR;
			ret[13] = HipKneeL;
			ret[14] = HipKneeR;
			ret[15] = KneeAnkleL;
			ret[16] = KneeAnkleR;
			ret[17] = AnkleFootL;
			ret[18] = AnkleFootR;
			return ret;
		}
		Joint* GetJointsArray() {
			Joint ret[20];
			ret[0] = HandL;
			ret[1] = HandR;
			ret[2] = WristL;
			ret[3] = WristR;
			ret[4] = ElbowL;
			ret[5] = ElbowR;
			ret[6] = ShoulderL;
			ret[7] = ShoulderR;
			ret[8] = ShoulderC;
			ret[9] = HipL;
			ret[10] = HipR;
			ret[11] = HipC;
			ret[12] = KneeL;
			ret[13] = KneeR;
			ret[14] = AnkleL;
			ret[15] = AnkleR;
			ret[16] = FootL;
			ret[17] = FootR;
			ret[18] = Spine;
			ret[19] = Head;
			return ret;
		}

		void UpdateSkelly(NUI_SKELETON_DATA& data) {
			//joints
			HandL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].w);
			HandR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].w);
			WristL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].w);
			WristR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].w);
			ElbowL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].w);
			ElbowR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].w);
			ShoulderL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].w);
			ShoulderR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].w);
			ShoulderC = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].w);
			Spine = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].w);
			HipL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].w);
			HipR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].w);
			HipC = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].w);
			KneeL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].w);
			KneeR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].w);
			AnkleL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].w);
			AnkleR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].w);
			FootL = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].w);
			FootR = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].w);
			Head = Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].w);
			//bones
			WristHandL.Update(WristL, HandL);
			WristHandR.Update(WristR, HandR);
			ElbowWristL.Update(ElbowL, WristL);
			ElbowWristR.Update(ElbowR, WristR);
			ShoulderElbowL.Update(ShoulderL, ElbowL);
			ShoulderElbowR.Update(ShoulderR, ElbowR);
			ShoulderCShoulderL.Update(ShoulderC, ShoulderL);
			ShoulderCShoulderR.Update(ShoulderC, ShoulderR);
			ShoulderCHead.Update(ShoulderC, Head);
			SpineShoulderC.Update(Spine, ShoulderC);
			SpineHipC.Update(Spine, HipC);
			HipCHipL.Update(HipC, HipL);
			HipCHipR.Update(HipC, HipR);
			HipKneeL.Update(HipL, KneeL);
			HipKneeR.Update(HipR, KneeR);
			KneeAnkleL.Update(KneeL, AnkleL);
			KneeAnkleR.Update(KneeR, AnkleR);
			AnkleFootL.Update(AnkleL, FootL);
			AnkleFootR.Update(AnkleR, FootR);
			//bone direction/normal
			WristHandL_Normal = WristHandL.Normalize();
			WristHandR_Normal = WristHandR.Normalize();
			ElbowWristL_Normal = ElbowWristL.Normalize();
			ElbowWristR_Normal = ElbowWristR.Normalize();
			ShoulderElbowL_Normal = ShoulderElbowL.Normalize();
			ShoulderElbowR_Normal = ShoulderElbowR.Normalize();
			ShoulderCShoulderL_Normal = ShoulderCShoulderL.Normalize();
			ShoulderCShoulderR_Normal = ShoulderCShoulderR.Normalize();
			ShoulderCHead_Normal = ShoulderCHead.Normalize();
			SpineShoulderC_Normal = SpineShoulderC.Normalize();
			SpineHipC_Normal = SpineHipC.Normalize();
			HipCHipL_Normal = HipCHipL.Normalize();
			HipCHipR_Normal = HipCHipR.Normalize();
			HipKneeL_Normal = HipKneeL.Normalize();
			HipKneeR_Normal = HipKneeR.Normalize();
			KneeAnkleL_Normal = KneeAnkleL.Normalize();
			KneeAnkleR_Normal = KneeAnkleR.Normalize();
			AnkleFootL_Normal = AnkleFootL.Normalize();
			AnkleFootR_Normal = AnkleFootR.Normalize();
		}
	};

	NUI_SKELETON_FRAME ourframe;
	Skelly People[6];
	INuiSensor * Sensor;
	HANDLE                  m_hNextSkeletonEvent;


	bool Init() {
		INuiSensor* pNuiSensor;

		int iSensorCount = 0;
		HRESULT hr = NuiGetSensorCount(&iSensorCount);
		if (FAILED(hr))
		{
			return hr;
		}

		// Look at each Kinect sensor
		for (int i = 0; i < iSensorCount; ++i)
		{
			// Create the sensor so we can check status, if we can't create it, move on to the next
			hr = NuiCreateSensorByIndex(i, &pNuiSensor);
			if (FAILED(hr))
			{
				continue;
			}

			// Get the status of the sensor, and if connected, then we can initialize it
			hr = pNuiSensor->NuiStatus();
			if (S_OK == hr)
			{
				Sensor = pNuiSensor;
				break;
			}

			// This sensor wasn't OK, so release it since we're not using it
			pNuiSensor->Release();
		}

		if (NULL != Sensor)
		{
			// Initialize the Kinect and specify that we'll be using skeleton
			hr = Sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
			if (SUCCEEDED(hr))
			{
				// Create an event that will be signaled when skeleton data is available
				m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

				// Open a skeleton stream to receive skeleton data
				hr = Sensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
			}
		}

		if (NULL == Sensor || FAILED(hr))
		{
			return false;
		}

		return true;
	}

	void UpdateSkeletons() {
		NuiSkeletonGetNextFrame(0,&ourframe); //Get a frame and stuff it into ourframe
		for (int i = 0; i < 6; i++) {
			People[i].Tracked = ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED;
			if (People[i].Tracked) {
				People[i].UpdateSkelly(ourframe.SkeletonData[i]);
			}
			//std::cout << "Person[" << i << "]: x:" << People[i].HandR.x << " y:" << People[i].HandR.y << " z:" << People[i].HandR.z<<std::endl;
		}
		//system("cls");
	}
}