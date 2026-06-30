using UnityEngine;

public class DirectionMove : MonoBehaviour
{
    [SerializeField]
    private Vector3 m_MoveAxis = Vector3.left;
    [SerializeField]
    private float m_SwitchingTime = 5.0f;
    [SerializeField]
    private float m_Speed = 1.0f;

    private bool m_ToMovePositive = true;
    private float m_LastSwitchingTime = 0.0f;

    private void Start() { m_LastSwitchingTime = Time.time; }

    private void Update()
    {
        transform.position += m_MoveAxis * ((m_ToMovePositive ? 1.0f : -1.0f) * Time.deltaTime * m_Speed);
        if (Time.time - m_LastSwitchingTime > m_SwitchingTime)
        {
            m_ToMovePositive = !m_ToMovePositive;
            m_LastSwitchingTime = Time.time;
        }
    }
}