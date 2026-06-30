using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using YVR.Core;

public class MoveScript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (YVRInput.Get(YVRInput.RawButton.RThumbstickUp))
        {
            this.transform.Translate(Vector3.forward * Time.deltaTime * 2f);
        }

        if (YVRInput.Get(YVRInput.RawButton.RThumbstickDown))
        {
            this.transform.Translate(Vector3.back * Time.deltaTime * 2f);
        }

        if (YVRInput.Get(YVRInput.RawButton.RThumbstickLeft))
        {
            this.transform.Rotate(Vector3.down * Time.deltaTime * 60f);
        }

        if (YVRInput.Get(YVRInput.RawButton.RThumbstickRight))
        {
            this.transform.Rotate(Vector3.up * Time.deltaTime * 60f);
        }
    }
}
