using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class TestEyesDevices : MonoBehaviour
{
    private InputDevice m_EyeDevice;

    public GameObject target;

    // Start is called before the first frame update
    private void Start()
    {
        Debug.Log("sss enter test eye devices");
        var connectedDevices = new List<InputDevice>();
        InputDevices.GetDevices(connectedDevices);
        connectedDevices.ForEach(device =>
        {
            if ((device.characteristics & InputDeviceCharacteristics.EyeTracking) == 0) return;

            Debug.Log($"aaa eyes device connected device characteristics:{device.characteristics},manufacturer:{device.manufacturer}");
            Debug.Log($"aaa product name is {device.name}");
            Debug.Log($"aaa product type ies {device.GetType()}");
            m_EyeDevice = device;
        });

        InputDevices.deviceConnected += OnDeviceConnected;
        InputDevices.deviceDisconnected += OnDeviceDisconnected;
    }

    private void OnDeviceConnected(InputDevice inputDevice)
    {
        if ((inputDevice.characteristics & InputDeviceCharacteristics.EyeTracking) != 0)
        {
            Debug.Log($"aaa eyes device connected device characteristics:{inputDevice.characteristics},manufacturer:{inputDevice.manufacturer}");
            Debug.Log($"aaa product name is {inputDevice.name}");
            Debug.Log($"aaa product type ies {inputDevice.GetType()}");

            m_EyeDevice = inputDevice;
        }
    }

    private void OnDeviceDisconnected(InputDevice inputDevice)
    {
        if ((inputDevice.characteristics & InputDeviceCharacteristics.EyeTracking) != 0)
        {
            Debug.LogError("eyes device disconnected");
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (!m_EyeDevice.isValid) return;

        m_EyeDevice.TryGetFeatureValue(CommonUsages.devicePosition, out Vector3 position);
        m_EyeDevice.TryGetFeatureValue(CommonUsages.deviceRotation, out Quaternion rotation);
        // m_Eyes.TryGetLeftEyePosition(out Vector3 position);
        // m_Eyes.TryGetLeftEyeRotation(out Quaternion rotation);

        Debug.Log($"sss position is {position}");
        target.transform.position = position + rotation * Vector3.forward;
        // Debug.Log($"left eye position rotation: {position}, {rotation}");
        //
        // m_Eyes.TryGetRightEyePosition(out Vector3 rPosition);
        // m_Eyes.TryGetRightEyeRotation(out Quaternion rRotation);
        // Debug.Log($"right eye position rotation: {rPosition}, {rRotation}");
        //
        // m_Eyes.TryGetFixationPoint(out Vector3 fixationPoint);
        // Debug.Log($"fixation eye position: {fixationPoint}");
    }
}