#include "YVRInputProvider.h"

UnitySubsystemErrorCode YVRInputProvider::Initialize()
{
	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::Start()
{
	m_Context.input->InputSubsystem_DeviceConnected(m_Handle, k_DeviceHMD);
	m_Context.input->InputSubsystem_DeviceConnected(m_Handle, k_DeviceLeftController);
	m_Context.input->InputSubsystem_DeviceConnected(m_Handle, k_DeviceRightController);

	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::Tick(UnityXRInputUpdateType updateType)
{
	yvrTrackingMgr.getPhysicsTrackingData(&m_YVRHeadPose, &m_YVRLEyePose, &m_YVRREyePose, &m_YVRLController,
		&m_YVRRController);
	yvrTrackingMgr.getControllerState(&m_YVRLeftControllerState, &m_YVRRightControllerState);
	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::FillDeviceDefinition(UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinition* definition)
{
	// Fill in your connected device information here when requested.  Used to create customized device states.
	auto& unityInputInterface = *m_Context.input;
	if (deviceId == k_DeviceHMD)
	{
		unityInputInterface.DeviceDefinition_SetName(definition, "Headset");
		unityInputInterface.DeviceDefinition_SetManufacturer(definition, "YVR");
		unityInputInterface.DeviceDefinition_SetSerialNumber(definition, "12345");
		unityInputInterface.DeviceDefinition_SetCharacteristics(
			definition,
			(UnityXRInputDeviceCharacteristics)(kUnityXRInputDeviceCharacteristicsHeadMounted | kUnityXRInputDeviceCharacteristicsTrackedDevice));

		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "headPosition", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageDevicePosition);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "headRotation", kUnityXRInputFeatureTypeRotation,
			kUnityXRInputFeatureUsageDeviceRotation);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "centerEyePosition", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageCenterEyePosition);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "centerEyeRotation", kUnityXRInputFeatureTypeRotation,
			kUnityXRInputFeatureUsageCenterEyeRotation);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "centerEyeAcceleration", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageCenterEyeAcceleration);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "centerEyeAngularAcceleration", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageCenterEyeAngularAcceleration);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "centerEyeVelocity", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageCenterEyeVelocity);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "centerEyeAngularVelocity", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageCenterEyeAngularVelocity);

		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "leftEyePosition", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageLeftEyePosition);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "leftEyeRotation", kUnityXRInputFeatureTypeRotation,
			kUnityXRInputFeatureUsageLeftEyeRotation);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "rightEyePosition", kUnityXRInputFeatureTypeAxis3D,
			kUnityXRInputFeatureUsageRightEyePosition);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "rightEyeRotation", kUnityXRInputFeatureTypeRotation,
			kUnityXRInputFeatureUsageRightEyeRotation);
		unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "userPresence", kUnityXRInputFeatureTypeBinary,
			kUnityXRInputFeatureUsageUserPresence);
	}
	else if (deviceId == k_DeviceLeftController)
	{
		unityInputInterface.DeviceDefinition_SetName(definition, "LeftController");
		unityInputInterface.DeviceDefinition_SetManufacturer(definition, "YVR");
		unityInputInterface.DeviceDefinition_SetSerialNumber(definition, "abc");
		unityInputInterface.DeviceDefinition_SetCharacteristics(
			definition,
			(UnityXRInputDeviceCharacteristics)(kUnityXRInputDeviceCharacteristicsTrackedDevice | kUnityXRInputDeviceCharacteristicsController |
				kUnityXRInputDeviceCharacteristicsHeldInHand | kUnityXRInputDeviceCharacteristicsLeft));
		SetDeviceDefinition(definition);
	}
	else if (deviceId == k_DeviceRightController)
	{
		unityInputInterface.DeviceDefinition_SetName(definition, "RightController");
		unityInputInterface.DeviceDefinition_SetManufacturer(definition, "YVR");
		unityInputInterface.DeviceDefinition_SetSerialNumber(definition, "def");
		unityInputInterface.DeviceDefinition_SetCharacteristics(
			definition,
			(UnityXRInputDeviceCharacteristics)(kUnityXRInputDeviceCharacteristicsTrackedDevice | kUnityXRInputDeviceCharacteristicsController |
				kUnityXRInputDeviceCharacteristicsHeldInHand | kUnityXRInputDeviceCharacteristicsRight));
		SetDeviceDefinition(definition);
	}

	return kUnitySubsystemErrorCodeSuccess;
}

void YVRInputProvider::SetDeviceDefinition(UnityXRInputDeviceDefinition* definition) {
	IUnityXRInputInterface unityInputInterface = *m_Context.input;

	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "devicePosition", kUnityXRInputFeatureTypeAxis3D,
		kUnityXRInputFeatureUsageDevicePosition);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "deviceRotation", kUnityXRInputFeatureTypeRotation,
		kUnityXRInputFeatureUsageDeviceRotation);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "deviceAcceleration", kUnityXRInputFeatureTypeAxis3D,
		kUnityXRInputFeatureUsageDeviceAcceleration);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "deviceAngularAcceleration", kUnityXRInputFeatureTypeAxis3D,
		kUnityXRInputFeatureUsageDeviceAngularAcceleration);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "deviceVelocity", kUnityXRInputFeatureTypeAxis3D,
		kUnityXRInputFeatureUsageDeviceVelocity);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "deviceAngularVelocity", kUnityXRInputFeatureTypeAxis3D,
		kUnityXRInputFeatureUsageDeviceAngularVelocity);

	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "isTracked", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageIsTracked);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "trackingState", kUnityXRInputFeatureTypeDiscreteStates,
		kUnityXRInputFeatureUsageTrackingState);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "primaryButton", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsagePrimaryButton);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "primaryTouched", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsagePrimaryTouch);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "secondaryButton", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageSecondaryButton);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "secondaryTouched", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageSecondaryTouch);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "triggerPressed", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageTriggerButton);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "triggerTouch", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageIndexTouch);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "trigger", kUnityXRInputFeatureTypeAxis1D,
		kUnityXRInputFeatureUsageTrigger);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "gripPressed", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageGripButton);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "menuButton", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsageMenuButton);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "thumbstickClicked", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsagePrimary2DAxisClick);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "thumbstick", kUnityXRInputFeatureTypeAxis2D,
		kUnityXRInputFeatureUsagePrimary2DAxis);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "thumbstickTouched", kUnityXRInputFeatureTypeBinary,
		kUnityXRInputFeatureUsagePrimary2DAxisTouch);
	unityInputInterface.DeviceDefinition_AddFeatureWithUsage(definition, "batteryLevel", kUnityXRInputFeatureTypeAxis1D,
		kUnityXRInputFeatureUsageBatteryLevel);
}

UnitySubsystemErrorCode YVRInputProvider::UpdateDeviceState(UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType, UnityXRInputDeviceState* state)
{
	//Called by Unity when it needs a current device snapshot

	switch (updateType)
	{
	case kUnityXRInputUpdateTypeBeforeRender:
	{
		if (deviceId == k_DeviceHMD)
		{
			if (m_StartFrame == 0)
			{
				m_StartFrame = m_Clock.elapsed();
			}

			m_CurrentFrameTime = m_Clock.elapsed();
			if (m_CurrentFrameTime < m_StartFrame + m_FrameSpacing * m_FrameIndex)
			{
				this_thread::sleep_for(std::chrono::milliseconds(m_StartFrame + m_FrameSpacing * m_FrameIndex - m_CurrentFrameTime));
			}

			m_FrameIndex++;

			SetXrHeadsetPoseData(state, &m_YVRHeadPose, &m_YVRLEyePose, &m_YVRREyePose);
		}
		else if (deviceId == k_DeviceLeftController)
		{
			SetXrControllerPoseData(state, &m_YVRLController);
			SetXRControllerTrackingState(state, yvrController::kControllerLeft);
			SetXRLeftControllerState(state, m_YVRLeftControllerState);
		}
		else if (deviceId == k_DeviceRightController)
		{
			SetXrControllerPoseData(state, &m_YVRRController);
			SetXRControllerTrackingState(state, yvrController::kControllerRight);
			SetXRRightControllerState(state, m_YVRRightControllerState);
		}
		break;
	}
	default:
		break;
	}


	return kUnitySubsystemErrorCodeSuccess;
}

// Reduce memory copy from yvr data to Unity data
UnityXRVector3* Convert2UnityVector3(yvrVector3f* yvrVector3) { return (UnityXRVector3*)(void*)yvrVector3; }
UnityXRVector4* Convert2UnityVector4(yvrQuatf* yvrQuat) { return (UnityXRVector4*)(void*)yvrQuat; }

UnityXRVector2* Convert2UnityVector2(float* array2[2]) { return (UnityXRVector2*)(void*)array2; }

void YVRInputProvider::SetXrHeadsetPoseData(UnityXRInputDeviceState* state, yvrPoseState* centerEyePose, yvrPoseState* leftEyePose,
	yvrPoseState* rightEyePose)
{
	IUnityXRInputInterface unityInputInterface = *m_Context.input;

	unityInputInterface.DeviceState_SetAxis3DValue(state, 0, *(Convert2UnityVector3(&centerEyePose->pose.position)));
	unityInputInterface.DeviceState_SetRotationValue(state, 1, *(Convert2UnityVector4(&centerEyePose->pose.rotation)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 2, *(Convert2UnityVector3(&centerEyePose->pose.position)));
	unityInputInterface.DeviceState_SetRotationValue(state, 3, *(Convert2UnityVector4(&centerEyePose->pose.rotation)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 4, *(Convert2UnityVector3(&centerEyePose->linearAcceleration)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 5, *(Convert2UnityVector3(&centerEyePose->angularAcceleration)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 6, *(Convert2UnityVector3(&centerEyePose->linearVelocity)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 7, *(Convert2UnityVector3(&centerEyePose->angularVelocity)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 8, *(Convert2UnityVector3(&leftEyePose->pose.position)));
	unityInputInterface.DeviceState_SetRotationValue(state, 9, *(Convert2UnityVector4(&leftEyePose->pose.rotation)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 10, *(Convert2UnityVector3(&rightEyePose->pose.position)));
	unityInputInterface.DeviceState_SetRotationValue(state, 11, *(Convert2UnityVector4(&rightEyePose->pose.rotation)));
	unityInputInterface.DeviceState_SetBinaryValue(state, 12, IsUserPresent());
}

void YVRInputProvider::SetXrControllerPoseData(UnityXRInputDeviceState* state, yvrPoseState* controllerPose)
{
	IUnityXRInputInterface unityInputInterface = *m_Context.input;

	unityInputInterface.DeviceState_SetAxis3DValue(state, 0, *(Convert2UnityVector3(&controllerPose->pose.position)));
	unityInputInterface.DeviceState_SetRotationValue(state, 1, *(Convert2UnityVector4(&controllerPose->pose.rotation)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 2, *(Convert2UnityVector3(&controllerPose->linearAcceleration)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 3, *(Convert2UnityVector3(&controllerPose->angularAcceleration)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 4, *(Convert2UnityVector3(&controllerPose->linearVelocity)));
	unityInputInterface.DeviceState_SetAxis3DValue(state, 5, *(Convert2UnityVector3(&controllerPose->angularVelocity)));
}

void YVRInputProvider::SetXRControllerTrackingState(UnityXRInputDeviceState* state, yvrController mask)
{
	IUnityXRInputInterface unityInputInterface = *m_Context.input;
	int trackingState = kUnityXRInputTrackingStatePosition | kUnityXRInputTrackingStateRotation;
	bool isTracking = IsControllerPositionTracked(mask);

	unityInputInterface.DeviceState_SetBinaryValue(state, 6, isTracking);
	unityInputInterface.DeviceState_SetDiscreteStateValue(state, 7, trackingState);
}

void YVRInputProvider::SetXRRightControllerState(UnityXRInputDeviceState* state, yvrControllerState controllerState)
{
	IUnityXRInputInterface unityInputInterface = *m_Context.input;

	unityInputInterface.DeviceState_SetBinaryValue(state, 8, (controllerState.Buttons & yvrButtonValue::A) != 0);             //A
	unityInputInterface.DeviceState_SetBinaryValue(state, 9, (controllerState.Touches & yvrButtonValue::A) != 0);             //A touch
	unityInputInterface.DeviceState_SetBinaryValue(state, 10, (controllerState.Buttons & yvrButtonValue::B) != 0);            //B
	unityInputInterface.DeviceState_SetBinaryValue(state, 11, (controllerState.Touches & yvrButtonValue::B) != 0);            //B touch
	unityInputInterface.DeviceState_SetBinaryValue(state, 12, controllerState.IndexTrigger > 0);                            //IndexTrigger
	unityInputInterface.DeviceState_SetBinaryValue(state, 13, (controllerState.Touches & yvrButtonValue::RTrigger) != 0);//IndexTriggerTouch
	unityInputInterface.DeviceState_SetAxis1DValue(state, 14, (float)controllerState.IndexTrigger);                                //trigger
	unityInputInterface.DeviceState_SetBinaryValue(state, 15, (controllerState.Buttons & yvrButtonValue::RGrip) != 0);       //GripButton
	unityInputInterface.DeviceState_SetBinaryValue(state, 16, (controllerState.Buttons & yvrButtonValue::Home) != 0);         //home
	unityInputInterface.DeviceState_SetBinaryValue(state, 17, (controllerState.Buttons & yvrButtonValue::RThumbStick) != 0);  //thumbstickClick
	unityInputInterface.DeviceState_SetAxis2DValue(state, 18, *(UnityXRVector2*)(void*)controllerState.Thumbstick);         //thumbstick
	unityInputInterface.DeviceState_SetBinaryValue(state, 19, (controllerState.Touches & yvrButtonValue::RThumbStick) != 0);  //thumbstickTouch
	unityInputInterface.DeviceState_SetAxis1DValue(state, 20, (float)controllerState.BatteryPercentRemaining / 100);          //batteryLevel
}

void YVRInputProvider::SetXRLeftControllerState(UnityXRInputDeviceState* state, yvrControllerState controllerState)
{
	IUnityXRInputInterface unityInputInterface = *m_Context.input;

	unityInputInterface.DeviceState_SetBinaryValue(state, 8, (controllerState.Buttons & yvrButtonValue::X) != 0);             //X
	unityInputInterface.DeviceState_SetBinaryValue(state, 9, (controllerState.Touches & yvrButtonValue::X) != 0);             //X touch
	unityInputInterface.DeviceState_SetBinaryValue(state, 10, (controllerState.Buttons & yvrButtonValue::Y) != 0);            //Y
	unityInputInterface.DeviceState_SetBinaryValue(state, 11, (controllerState.Touches & yvrButtonValue::Y) != 0);            //Y touch
	unityInputInterface.DeviceState_SetBinaryValue(state, 12, controllerState.IndexTrigger > 0);                            //IndexTrigger
	unityInputInterface.DeviceState_SetBinaryValue(state, 13, (controllerState.Touches & yvrButtonValue::LTrigger) != 0);//IndexTriggerTouch
	unityInputInterface.DeviceState_SetAxis1DValue(state, 14, (float)controllerState.IndexTrigger);                                //trigger
	unityInputInterface.DeviceState_SetBinaryValue(state, 15, (controllerState.Buttons & yvrButtonValue::LGrip) != 0);       //GripButton
	unityInputInterface.DeviceState_SetBinaryValue(state, 16, (controllerState.Buttons & yvrButtonValue::Meun) != 0);         //Meun
	unityInputInterface.DeviceState_SetBinaryValue(state, 17, (controllerState.Buttons & yvrButtonValue::LThumbStick) != 0);  //thumbstickClick
	unityInputInterface.DeviceState_SetAxis2DValue(state, 18, *(UnityXRVector2*)(void*)controllerState.Thumbstick);         //thumbstick
	unityInputInterface.DeviceState_SetBinaryValue(state, 19, (controllerState.Touches & yvrButtonValue::LThumbStick) != 0);  //thumbstickTouch
	unityInputInterface.DeviceState_SetAxis1DValue(state, 20, (float)controllerState.BatteryPercentRemaining / 100);          //batteryLevel
}

UnitySubsystemErrorCode YVRInputProvider::OnHandleHapticImpulse(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, int channel, float amplitude, float duration)
{
	yvrController controller = deviceId == 1 ? yvrController::kControllerLeft : yvrController::kControllerRight;
	SetControllerVibration(controller, amplitude, duration);
	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::OnHandleHapticBuffer(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, int channel, unsigned int bufferSize, const unsigned char* const buffer)
{
	// TODO
	return kUnitySubsystemErrorCodeFailure;
}

UnitySubsystemErrorCode YVRInputProvider::OnQueryHapticCapabilities(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRHapticCapabilities* capabilities)
{
	capabilities->numChannels = 1;
	capabilities->supportsImpulse = true;
	capabilities->supportsBuffer = true;
	capabilities->bufferFrequencyHz = 1;
	capabilities->bufferMaxSize = 0;
	capabilities->bufferOptimalSize = 0;

	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::OnHandleHapticStop(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId)
{
	yvrController controller = deviceId == 1 ? yvrController::kControllerLeft : yvrController::kControllerRight;
	//yvrlib_SetControllerVibration(controller, 0, 0);

	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::onHandleSetTrackingOriginMode(UnitySubsystemHandle handle, void* userData,
	UnityXRInputTrackingOriginModeFlags trackingOriginMode)
{
	int trackingSpace = 0;
	switch (trackingOriginMode)
	{
	case kUnityXRInputTrackingOriginModeDevice:
		trackingSpace = (int)XR_REFERENCE_SPACE_TYPE_LOCAL;
		break;
	case kUnityXRInputTrackingOriginModeFloor:
		trackingSpace = (int)XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR_EXT;
		break;
	case kUnityXRInputTrackingOriginModeTrackingReference:
		trackingSpace = (int)XR_REFERENCE_SPACE_TYPE_STAGE;
		break;
	default:
		break;
	}
	std::string v = std::to_string(kUnityXRInputTrackingOriginModeDevice);
	std::string logMsg = " onHandleSetTrackingOriginMode:" + v;
	Write(logMsg.c_str());

	if (plugin.previewToolService->trankingSpace != trackingSpace)
	{
		plugin.previewToolService->SetTrankingSpace(trackingSpace);
	}
	plugin.previewToolService->trankingSpace = trackingSpace;
	const IUnityXRInputInterface unityInputInterface = *m_Context.input;
	unityInputInterface.InputSubsystem_TrackingOriginUpdated(handle);
	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::onQueryTrackingOriginMode(UnitySubsystemHandle handle, void* userData,
	UnityXRInputTrackingOriginModeFlags* trackingOriginMode)
{
	const XrReferenceSpaceType trackingSpace = (XrReferenceSpaceType)plugin.previewToolService->trankingSpace;
	switch (trackingSpace)
	{
	case XR_REFERENCE_SPACE_TYPE_LOCAL:
		*trackingOriginMode = kUnityXRInputTrackingOriginModeDevice;
		break;
	case XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR_EXT:
		*trackingOriginMode = kUnityXRInputTrackingOriginModeFloor;
		break;
	case XR_REFERENCE_SPACE_TYPE_STAGE:
		*trackingOriginMode = kUnityXRInputTrackingOriginModeTrackingReference;
		break;
	default:
		break;
	}
	std::string v = std::to_string(*trackingOriginMode);
	std::string logMsg = "onQueryTrackingOriginMode:" + v;
	Write(logMsg.c_str());
	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::onQuerySupportedTrackingOriginModes(UnitySubsystemHandle handle, void* userData,
	UnityXRInputTrackingOriginModeFlags* supportedTrackingOriginModes)
{

	Write("onQuerySupportedTrackingOriginModes begin");
	*supportedTrackingOriginModes = static_cast<UnityXRInputTrackingOriginModeFlags>(
		kUnityXRInputTrackingOriginModeUnknown | kUnityXRInputTrackingOriginModeDevice |
		kUnityXRInputTrackingOriginModeFloor |
		kUnityXRInputTrackingOriginModeTrackingReference);
	return kUnitySubsystemErrorCodeSuccess;
}


UnitySubsystemErrorCode YVRInputProvider::HandleEvent(unsigned int eventType, UnityXRInternalInputDeviceId deviceId, void* buffer, unsigned int size)
{
	/// Simple, generic method callback to inform the plugin or individual devices of events occurring within unity
	return kUnitySubsystemErrorCodeFailure;
}

UnitySubsystemErrorCode YVRInputProvider::TryGetDeviceStateAtTime(UnityXRTimeStamp time, UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceState* state)
{
	/// Unity calls this when requesting a state at a specific time in the past
	return kUnitySubsystemErrorCodeSuccess;
}

bool YVRInputProvider::IsUserPresent()
{
	return true;
}

bool YVRInputProvider::IsControllerPositionTracked(yvrController mask)
{
	return mask == yvrController::kControllerLeft ? (m_YVRLController.poseStatus & yvrTrackingStatusPrivate::TRACKING_STATUS_POSITION_TRACKED)
		: (m_YVRRController.poseStatus & yvrTrackingStatusPrivate::TRACKING_STATUS_POSITION_TRACKED);
}

void YVRInputProvider::Stop()
{
	m_Context.input->InputSubsystem_DeviceDisconnected(m_Handle, k_DeviceHMD);
	m_Context.input->InputSubsystem_DeviceDisconnected(m_Handle, k_DeviceLeftController);
	m_Context.input->InputSubsystem_DeviceDisconnected(m_Handle, k_DeviceRightController);
}

void YVRInputProvider::Shutdown()
{

}

void YVRInputProvider::SetControllerVibration(yvrController mask, float frequency, float amplitude)
{
	//yvrlib_SetControllerVibration(mask, frequency, amplitude);
}

// Binding to C-API below here
static UnitySubsystemErrorCode UNITY_INTERFACE_API InputInitialize(UnitySubsystemHandle handle, void* userData)
{
	auto& ctx = GetProviderContext(userData);

	ctx.yvrInputProvider = new YVRInputProvider(ctx, handle);

	UnityXRInputProvider inputProvider{};
	inputProvider.userData = &ctx;

	inputProvider.Tick = [](UnitySubsystemHandle handle, void* userData, UnityXRInputUpdateType updateType) -> UnitySubsystemErrorCode {
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->Tick(updateType);
	};

	inputProvider.FillDeviceDefinition = [](UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinition* definition) -> UnitySubsystemErrorCode {
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->FillDeviceDefinition(deviceId, definition);
	};

	inputProvider.UpdateDeviceState = [](UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType, UnityXRInputDeviceState* state) -> UnitySubsystemErrorCode {
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->UpdateDeviceState(deviceId, updateType, state);
	};

	inputProvider.HandleEvent = [](UnitySubsystemHandle handle, void* userData, unsigned int eventType, UnityXRInternalInputDeviceId deviceId, void* buffer, unsigned int size) -> UnitySubsystemErrorCode {
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->HandleEvent(eventType, deviceId, buffer, size);
	};

	inputProvider.TryGetDeviceStateAtTime = [](UnitySubsystemHandle handle, void* userData, UnityXRTimeStamp time, UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceState* state) -> UnitySubsystemErrorCode {
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->TryGetDeviceStateAtTime(time, deviceId, state);
	};

	inputProvider.HandleHapticImpulse = [](UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, int channel,
		float amplitude, float duration) -> UnitySubsystemErrorCode
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->OnHandleHapticImpulse(handle, userData, deviceId, channel, amplitude, duration);
	};

	inputProvider.QueryHapticCapabilities = [](UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId,
		UnityXRHapticCapabilities* capabilities) -> UnitySubsystemErrorCode
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->OnQueryHapticCapabilities(handle, userData, deviceId, capabilities);
	};

	inputProvider.HandleHapticStop = [](UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId) -> UnitySubsystemErrorCode
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->OnHandleHapticStop(handle, userData, deviceId);
	};

	inputProvider.HandleHapticBuffer = [](UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, int channel,
		unsigned int bufferSize, const unsigned char* const buffer)
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->OnHandleHapticBuffer(handle, userData, deviceId, channel, bufferSize, buffer);
	};

	inputProvider.HandleSetTrackingOriginMode = [](UnitySubsystemHandle handle, void* userData,
		UnityXRInputTrackingOriginModeFlags trackingOriginMode) -> UnitySubsystemErrorCode
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->onHandleSetTrackingOriginMode(handle, userData, trackingOriginMode);
	};

	inputProvider.QueryTrackingOriginMode = [](UnitySubsystemHandle handle, void* userData,
		UnityXRInputTrackingOriginModeFlags* trackingOriginMode) -> UnitySubsystemErrorCode
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->onQueryTrackingOriginMode(handle, userData, trackingOriginMode);
	};

	inputProvider.QuerySupportedTrackingOriginModes =
		[](UnitySubsystemHandle handle, void* userData, UnityXRInputTrackingOriginModeFlags* supportedTrackingOriginModes) -> UnitySubsystemErrorCode
	{
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->onQuerySupportedTrackingOriginModes(handle, userData, supportedTrackingOriginModes);
	};

	ctx.input->RegisterInputProvider(handle, &inputProvider);

	return ctx.yvrInputProvider->Initialize();
}

UnitySubsystemErrorCode LoadInput(ProviderContext& context)
{
	context.input = context.interfaces->Get<IUnityXRInputInterface>();
	if (context.input == nullptr)
		return kUnitySubsystemErrorCodeFailure;

	UnityLifecycleProvider inputLifecycleHandler{};
	inputLifecycleHandler.userData = &context;

	inputLifecycleHandler.Initialize = &InputInitialize;

	inputLifecycleHandler.Start = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode {
		auto& ctx = GetProviderContext(userData);
		return ctx.yvrInputProvider->Start();
	};

	inputLifecycleHandler.Stop = [](UnitySubsystemHandle handle, void* userData) -> void {
		auto& ctx = GetProviderContext(userData);
		ctx.yvrInputProvider->Stop();
	};

	inputLifecycleHandler.Shutdown = [](UnitySubsystemHandle handle, void* userData) -> void {
		auto& ctx = GetProviderContext(userData);
		ctx.yvrInputProvider->Shutdown();

		delete ctx.yvrInputProvider;
	};

	return context.input->RegisterLifecycleProvider("YVR XR SDK", "Tracking", &inputLifecycleHandler);
}