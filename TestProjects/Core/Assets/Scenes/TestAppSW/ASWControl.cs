using System.Collections;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.VFX;
using YVR.Core;

public class ASWControl : MonoBehaviour
{
    [SerializeField] private Text m_ASWStateText = null;
    private bool lock45Fps = false;

    private int frameCount = 0;

    private void Start()
    {
        StartCoroutine(FPSCoroutine());
    }


    public void Update()
    {
        if (YVRInput.GetDown(YVRInput.RawButton.A))
        {
            YVRManager.instance.SetAppSWEnable(!YVRManager.instance.GetAppSWEnable());
            //YVRManager.instance.qualityManager.aswEnable = !YVRManager.instance.qualityManager.aswEnable;
        }

        if (YVRInput.GetDown(YVRInput.RawButton.B))
        {
            lock45Fps = !lock45Fps;
            YVRManager.instance.qualityManager.vSyncCount
                = lock45Fps ? YVRQualityManager.VSyncCount.k2 : YVRQualityManager.VSyncCount.k1;
        }

        frameCount++;
    }

    private IEnumerator FPSCoroutine()
    {
        while (true)
        {
            yield return new WaitForSecondsRealtime(0.5f);
            if (m_ASWStateText != null)
            {
                m_ASWStateText.text = $"FPS: {frameCount / 0.5f} \n ASW Enable: {YVRPlugin.Instance.GetAppSWSwitch()}";
            }
            frameCount = 0;
        }
    }
}