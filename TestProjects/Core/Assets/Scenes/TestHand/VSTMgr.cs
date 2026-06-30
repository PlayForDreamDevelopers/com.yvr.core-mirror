using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using YVR.Core;

public class VSTMgr : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        YVRPlugin.Instance.SetPassthrough(true);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
