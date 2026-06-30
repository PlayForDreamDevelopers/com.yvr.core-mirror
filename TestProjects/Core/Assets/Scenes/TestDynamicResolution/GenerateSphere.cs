using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GenerateSphere : MonoBehaviour
{
    public int generateCount;
    public GameObject sphereResource;
    public Material sphereMat;

    // Start is called before the first frame update
    void Start()
    {
        for (int i = 0; i < generateCount; i++)
        {
            GameObject sphereGameObject = GameObject.Instantiate(sphereResource, this.transform.position, this.transform.rotation);
            sphereGameObject.transform.SetParent(this.transform);
            //force to generate material instance
            sphereGameObject.GetComponent<Renderer>().material = sphereMat;
        }
    }
}
