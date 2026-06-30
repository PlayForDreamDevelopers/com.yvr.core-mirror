using System;
using System.IO;
using UnityEngine;
using YVR.Core;

public class SLAMRecorder : MonoBehaviour
{
    public Transform targetTF;
    public UnityEngine.UI.Text tipsText;
    private TimeSpan m_Timer;
    private FileStream m_FileStream = null;
    private StreamWriter m_WriteStream = null;

    private bool m_IsRecording = false;
    private bool isRecording
    {
        get => m_IsRecording;
        set
        {
            if (m_IsRecording == value) return;
            m_IsRecording = value;
            if (value)
            {
                m_Timer = new TimeSpan(0, 0, 0);
                m_FileStream = new FileStream(Path.Combine(Application.persistentDataPath, $"SLAM_{DateTime.Now.ToString("yyyy_MM_dd_hh_mm_ss")}.txt"), FileMode.OpenOrCreate, FileAccess.Write);
                m_WriteStream = new StreamWriter(m_FileStream);
                tipsText.text = "start recording";
                tipsText.transform.parent.parent.gameObject.SetActive(true);
                Invoke(nameof(HideTipsText), 2);
            }
            else
            {
                m_WriteStream.Flush();
                m_WriteStream.Close();
                m_FileStream.Close();
                tipsText.text = "end recording";
                tipsText.transform.parent.parent.gameObject.SetActive(true);
                Invoke(nameof(HideTipsText), 2);
                Debug.Log(Application.persistentDataPath);
            }
            YVRInput.SetControllerVibration(0.3f, 0.3f, ControllerType.Touch);
        }
    }

    void Update()
    {
        if (YVRInput.GetDown(YVRInput.VirtualButton.One) || Input.GetKeyDown(KeyCode.S))
        {
            isRecording = !isRecording;
        }

        if (isRecording)
        {
            m_Timer += TimeSpan.FromSeconds(Time.deltaTime);
            m_WriteStream.Write($"{m_Timer.TotalSeconds:N6} {targetTF.position.x:N6} {targetTF.position.y:N6} {targetTF.position.z:N6} {targetTF.rotation.x:N6} {targetTF.rotation.y:N6} {targetTF.rotation.z:N6} {targetTF.rotation.w:N6}\r\n");
        }
    }

    private void HideTipsText() => tipsText.transform.parent.parent.gameObject.SetActive(false);
}