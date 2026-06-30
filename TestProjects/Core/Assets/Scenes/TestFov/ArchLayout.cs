using UnityEngine;

public class ArchLayout : MonoBehaviour
{
    public Transform[] transforms;
    public float startAngle = -50.0f;
    public float stepAngle = -0.5f;
    public float radius = 3.0f;

    [ContextMenu("Layout")]
    private void Layout()
    {
        float targetAngle = startAngle;

        for (int i = 0; i < transforms.Length; i++)
        {
            transforms[i].position = new Vector3(
                Mathf.Sin(Mathf.Deg2Rad * targetAngle) * radius,
                0,
                Mathf.Cos(Mathf.Deg2Rad * targetAngle) * radius);

            transforms[i].eulerAngles = Vector3.up * targetAngle;
            targetAngle += stepAngle;
        }
    }
}
