using System;
using AOT;
using UnityEngine.Internal;
using YVR.Utilities;

namespace YVR.Core
{
    /// <summary>
    /// The manager for events
    /// </summary>
    public class YVREventsManager
    {
        /// <summary>
        /// Occurs when head gained tracking.
        /// </summary>
        public event Action onTrackingAcquired = null;

        /// <summary>
        /// Occurs when head lost tracking.
        /// </summary>
        public event Action onTrackingLost = null;

        /// <summary>
        /// Occurs when an HMD is put on the user's head.
        /// </summary>
        public event Action onHMDMounted = null;

        /// <summary>
        /// Occurs when an HMD is taken off the user's head.
        /// </summary>
        public event Action onHMDUnMounted = null;

        /// <summary>
        /// Occurs when recenter occurred
        /// </summary>
        public event Action onRecenterOccurred = null;

        /// <summary>
        /// Occurs when application focus gained
        /// </summary>
        public event Action onFocusGained = null;

        /// <summary>
        /// Occurs when application focus lost
        /// </summary>
        public event Action onFocusLost = null;

        /// <summary>
        /// Occurs when application is visible
        /// </summary>
        public event Action onVisibilityGained = null;

        /// <summary>
        /// Occurs when application is completely obscured by other content
        /// </summary>
        public event Action onVisibilityLost = null;

        /// <summary>
        /// Occurs at the update function of every frame
        /// </summary>
        public event Action onUpdate = null;

        public event Action<ActiveInputDevice> onInputDeviceChange = null;

        private bool m_WasHMDTracking = false;
        private bool m_WasUserPresent = false;
        private ActiveInputDevice m_PreInputDevice = ActiveInputDevice.None;

        private static Action<YVREventType> s_EventCallback = null;

        public void Initialize()
        {
            YVRPlugin.Instance.SetEventCallback(OnEventCallback);
            s_EventCallback += TriggerEventCallback;
        }

        [ExcludeFromDocs]
        public void Update()
        {
            onUpdate?.Invoke();
            TriggerTrackingEvent();
            TriggerUserPresentEvent();
            CheckInputDeviceType();
        }

        private void TriggerEventCallback(YVREventType code)
        {
            if (code == YVREventType.VisibilityGained)
                onVisibilityGained?.SafeInvoke();
            else if (code == YVREventType.VisibilityLost)
                onVisibilityLost?.SafeInvoke();
            else if (code == YVREventType.FocusGained)
                onFocusGained?.SafeInvoke();
            else if (code == YVREventType.FocusLost)
                onFocusLost?.SafeInvoke();
            else if (code == YVREventType.Recenter)
                onRecenterOccurred?.SafeInvoke();
        }

        private void TriggerTrackingEvent()
        {
            bool isPositionTracked = YVRCameraRig.GetPositionTracked();

            if (!m_WasHMDTracking && isPositionTracked)
                onTrackingAcquired?.SafeInvoke();
            if (m_WasHMDTracking && !isPositionTracked)
                onTrackingLost?.SafeInvoke();

            m_WasHMDTracking = isPositionTracked;
        }

        private void TriggerUserPresentEvent()
        {
            bool isUserPresent = YVRManager.instance.hmdManager.isUserPresent;

            if (!m_WasUserPresent && isUserPresent)
                onHMDMounted?.SafeInvoke();
            if (m_WasUserPresent && !isUserPresent)
                onHMDUnMounted?.SafeInvoke();

            m_WasUserPresent = isUserPresent;
        }

        [MonoPInvokeCallback(typeof(Action<int>))]
        private static void OnEventCallback(int eventCode)
        {
            s_EventCallback?.Invoke((YVREventType)eventCode);
        }

        private void CheckInputDeviceType()
        {
            ActiveInputDevice currentInputDevice = ActiveInputDevice.None;
            YVRPlugin.Instance.GetCurrentInputDevice(ref currentInputDevice);
            if (currentInputDevice != m_PreInputDevice)
            {
                onInputDeviceChange?.Invoke(currentInputDevice);
            }

            m_PreInputDevice = currentInputDevice;
        }
    }
}
