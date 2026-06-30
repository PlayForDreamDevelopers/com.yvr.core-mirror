using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.Serialization;
using YVR.Core;

public class TestPlaneDetection : MonoBehaviour
{
    public GameObject planePrefab;
    public GameObject polygonPrefab;
    public Transform planeTarget;
    public Transform target;
    private Dictionary<ulong, MeshFilter> m_PolygonFilters = new();
    private Dictionary<ulong, GameObject> m_Planes = new();

    // Start is called before the first frame update
    void Start()
    {
        PremissionRequest.instance.ScenePermissionRequest();
        YVRPlugin.Instance.SetPassthrough(true);
        YVRPlaneDetectorMgr.instance.CreatePlaneDetector();
        YVRPlaneDetectorMgr.getPlanesAction += OnPlaneDetectionCallback;
    }

    private void OnPlaneDetectionCallback(List<YVRPlaneDetectorLocation> planes)
    {
        Debug.Log($"OnPlaneDetectionCallback planes count:{planes.Count}");
        foreach (var plane in planes)
        {
            Debug.Log($"plane :{plane.ToString()}");
            var polygon = YVRPlaneDetectorMgr.instance.GetPlanePolygonBuffer(plane);
            switch (plane.changeState)
            {
                case YVRPlaneChangeState.Added:
                    AddPolygon(plane, polygon);
                    AddPlane(plane);
                    break;
                case YVRPlaneChangeState.Update:
                    UpdatePolygon(plane, polygon);
                    UpdatePlane(plane);
                    break;
                case YVRPlaneChangeState.Removed:
                    DeletePolygon(plane.planeId);
                    DeletePlane(plane);
                    break;
            }
        }
    }

    private void AddPlane(YVRPlaneDetectorLocation plane)
    {
        if (!m_Planes.ContainsKey(plane.planeId))
        {
            GameObject planeGo = Instantiate(planePrefab, planeTarget);
            m_Planes.Add(plane.planeId, planeGo);
            UpdatePlane(plane);
            planeGo.SetActive(true);
        }
    }

    private void UpdatePlane(YVRPlaneDetectorLocation plane)
    {
        if (m_Planes.TryGetValue(plane.planeId,out GameObject planeGo))
        {
            planeGo.transform.position = plane.pose.position;
            planeGo.transform.rotation = plane.pose.orientation;
            Debug.Log($"plane.extents:{plane.extents}");
            planeGo.transform.localScale = new Vector3(plane.extents.x, 1, plane.extents.y);
        }
    }

    private void DeletePlane(YVRPlaneDetectorLocation plane)
    {
        if (m_Planes.TryGetValue(plane.planeId, out GameObject planeGo))
        {
            m_Planes.Remove(plane.planeId);
            Destroy(planeGo);
        }
    }

    private void AddPolygon(YVRPlaneDetectorLocation plane, List<YVRPlaneDetectorPolygonBuffer> buffers)
    {
        if (buffers.Count > 0)
        {
            if (!m_PolygonFilters.ContainsKey(plane.planeId))
            {
                MeshFilter meshFilter = Instantiate(polygonPrefab, target).AddComponent<MeshFilter>();
                meshFilter.gameObject.transform.position = plane.pose.position;
                meshFilter.gameObject.transform.rotation = plane.pose.orientation;
                meshFilter.gameObject.SetActive(true);
                meshFilter.mesh = new Mesh();
                m_PolygonFilters.Add(plane.planeId, meshFilter);
                GenerateMesh(meshFilter.mesh, buffers.First().pointVectors);
            }
        }
    }

    private void UpdatePolygon(YVRPlaneDetectorLocation plane, List<YVRPlaneDetectorPolygonBuffer> buffers)
    {
        m_PolygonFilters.TryGetValue(plane.planeId, out MeshFilter meshFilter);
        if (meshFilter != null)
        {
            meshFilter.gameObject.transform.position = plane.pose.position;
            meshFilter.gameObject.transform.rotation = plane.pose.orientation;
            GenerateMesh(m_PolygonFilters[plane.planeId].mesh, buffers.First().pointVectors);
        }
    }

    private void GenerateMesh(Mesh mesh, List<Vector2> planeVertices)
    {
        mesh.Clear();
        Vector3[] vertices = new Vector3[planeVertices.Count];
        for (int i = 0; i < planeVertices.Count; i++)
        {
            vertices[i] = new Vector3(planeVertices[i].x, 0, planeVertices[i].y);
        }

        mesh.vertices = vertices;
        var indices = GetIndices(vertices);
        mesh.SetIndices(indices,MeshTopology.Lines, 0);
    }

    private void DeletePolygon(ulong planeId)
    {
        if (m_PolygonFilters.TryGetValue(planeId, out var value))
            Destroy(value.gameObject);
    }

    private int[] GetIndices(Vector3[] vertices)
    {
        int[] indices = new int[vertices.Length * 2];
        for (int i = 0; i < vertices.Length; i++)
        {
            indices[i * 2] = i;
            indices[i * 2 + 1] = (i + 1) % vertices.Length;
        }

        return indices;
    }

    private void OnApplicationPause(bool pause)
    {
        if (pause)
            YVRPlaneDetectorMgr.instance.EndPlaneDetector();
        else
            YVRPlaneDetectorMgr.instance.CreatePlaneDetector();
    }
}