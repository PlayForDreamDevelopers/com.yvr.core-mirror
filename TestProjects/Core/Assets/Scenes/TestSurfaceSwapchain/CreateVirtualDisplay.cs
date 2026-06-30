using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using YVR.Core;

public class CreateVirtualDisplay : MonoBehaviour
{
    public YVRCompositeLayer compositeLayer;

    private bool alreadySetDisplay = false;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        IntPtr externalSurface = compositeLayer.GetExternalAndroidSurface();
        if (!alreadySetDisplay && externalSurface != IntPtr.Zero)
        {
            alreadySetDisplay = true;
            compositeLayer.SetSurfaceDimensions(1080, 1920);
            AndroidJavaObject surfaceObject = new AndroidJavaObject(externalSurface);
            //AndroidJavaObject surfaceClass = ClassForName("android.view.Surface");
            //AndroidJavaObject surface = Cast(surfaceObject, surfaceClass);
            SetDisplay(surfaceObject);
        }
    }

    private void SetDisplay(AndroidJavaObject surfaceObject)
    {  
        AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
        AndroidJavaObject activity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
        AndroidJavaObject virtualDisplayTest = new AndroidJavaObject("com.example.myapplication.VirtualPlayTest");
        Debug.LogError("LQR SetDisplay");
        //virtualDisplayTest.Call("Test"); 
        virtualDisplayTest.Call("CreateVirtualDisplay", surfaceObject, activity);
        //activity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
        //{
        //    //AndroidJNI.AttachCurrentThread();
        //    virtualDisplayTest.Call("CreateVirtualDisplay", surfaceObject, activity);
        //}));
    }
}
