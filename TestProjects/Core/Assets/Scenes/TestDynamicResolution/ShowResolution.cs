using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.XR;

public class ShowResolution : MonoBehaviour
{
    public Text resolutionText;

    // Update is called once per frame
    void Update()
    {
        float viewportScale = XRSettings.renderViewportScale;
        int w = (int)((float)XRSettings.eyeTextureWidth * viewportScale);
        int h = (int)((float)XRSettings.eyeTextureHeight * viewportScale);
        string resString = $"Resolution:{w}x{h} Scale:{viewportScale.ToString("0.00")}";

        resolutionText.text = resString;
    }
}
