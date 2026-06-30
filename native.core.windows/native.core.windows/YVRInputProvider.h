#pragma once

#include "Headers/XR/IUnityXRInput.h"
#include "ProviderContext.h"
#include "YVR_Lib_Types.h"
#include "YVRTrackingMgr.h"
#include "timer.h"

enum yvrButtonValue
{
	A = 1 << 0,
	B = 1 << 1,
	X = 1 << 2,
	Y = 1 << 3,
	Meun = 1 << 4,
	Home = 1 << 5,
	LTrigger = 1 << 6,
	RTrigger = 1 << 7,
	LGrip = 1 << 8,
	RGrip = 1 << 9,
	LThumbStick = 1 << 10,
	RThumbStick = 1 << 11,
};

/// Bit flags describing the current status of sensor tracking.
typedef enum yvrTrackingStatusPrivate_
{
	TRACKING_STATUS_INVALID = 0,      //< Orientation and Position invalid.
	TRACKING_STATUS_ORIENTATION_TRACKED = 1 << 0, //< Orientation is currently tracked.
	TRACKING_STATUS_POSITION_TRACKED = 1 << 1, //< Position is currently tracked.
	TRACKING_STATUS_ORIENTATION_VALID = 1 << 2, //< Orientation reported is valid.
	TRACKING_STATUS_POSITION_VALID = 1 << 3, //< Position reported is valid.
	TRACKING_STATUS_ALLIGNED = 1 << 4, //< Aligned with head.
	TRACKING_STATUS_PREDICTING = 1 << 5, //< Predicting.
} yvrTrackingStatusPrivate;

class YVRInputProvider : public ProviderImpl
{
public:
	YVRInputProvider(ProviderContext& ctx, UnitySubsystemHandle handle)
		: ProviderImpl(ctx, handle)
	{
	}
	virtual ~YVRInputProvider() {}

	UnitySubsystemErrorCode Initialize() override;
	UnitySubsystemErrorCode Start() override;

	UnitySubsystemErrorCode Tick(UnityXRInputUpdateType updateType);
	UnitySubsystemErrorCode FillDeviceDefinition(UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinition* definition);
	UnitySubsystemErrorCode UpdateDeviceState(UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType, UnityXRInputDeviceState* state);
	UnitySubsystemErrorCode HandleEvent(unsigned int eventType, UnityXRInternalInputDeviceId deviceId, void* buffer, unsigned int size);
	UnitySubsystemErrorCode TryGetDeviceStateAtTime(UnityXRTimeStamp time, UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceState* state);
	UnitySubsystemErrorCode OnHandleHapticImpulse(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, int channel,
		float amplitude, float duration);
	UnitySubsystemErrorCode OnHandleHapticBuffer(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, int channel,
		unsigned int bufferSize, const unsigned char* const buffer);
	UnitySubsystemErrorCode OnQueryHapticCapabilities(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId,
		UnityXRHapticCapabilities* capabilities);
	UnitySubsystemErrorCode OnHandleHapticStop(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId);
	UnitySubsystemErrorCode onHandleSetTrackingOriginMode(UnitySubsystemHandle handle, void* userData,
		UnityXRInputTrackingOriginModeFlags trackingOriginMode);
	UnitySubsystemErrorCode onQueryTrackingOriginMode(UnitySubsystemHandle handle, void* userData,
		UnityXRInputTrackingOriginModeFlags* trackingOriginMode);
	UnitySubsystemErrorCode onQuerySupportedTrackingOriginModes(UnitySubsystemHandle handle, void* userData,
		UnityXRInputTrackingOriginModeFlags* supportedTrackingOriginModes);

	void Stop() override;
	void Shutdown() override;
	void SetDeviceDefinition(UnityXRInputDeviceDefinition* definition);
	void SetXrHeadsetPoseData(UnityXRInputDeviceState* state, yvrPoseState* centerEyePose, yvrPoseState* leftEyePose, yvrPoseState* rightEyePose);
	void SetXrControllerPoseData(UnityXRInputDeviceState* state, yvrPoseState* controllerPose);
	void SetXRControllerTrackingState(UnityXRInputDeviceState* state, yvrController make);
	void SetXRLeftControllerState(UnityXRInputDeviceState* state, yvrControllerState controllerState);
	void SetXRRightControllerState(UnityXRInputDeviceState* state, yvrControllerState controllerState);
	void SetControllerVibration(yvrController mask, float frequency, float amplitude);
	bool IsUserPresent();
	bool IsControllerPositionTracked(yvrController mask);


private:
	static const int k_DeviceHMD = 0;
	static const int k_DeviceLeftController = 1;
	static const int k_DeviceRightController = 2;
	Timer m_Clock;

	uint64_t m_StartFrame;
	uint64_t m_CurrentFrameTime;
	uint64_t m_FrameIndex;
	uint16_t m_FrameSpacing = 11;

	yvrPoseState m_YVRHeadPose, m_YVRLEyePose, m_YVRREyePose, m_YVRLController, m_YVRRController;
	yvrControllerState m_YVRLeftControllerState, m_YVRRightControllerState;

};