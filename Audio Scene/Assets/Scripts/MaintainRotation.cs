using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MaintainRotation : MonoBehaviour
{
    private Quaternion rotation;
    private void Awake()
    {
        rotation = transform.localRotation;
    }
    void Update()
    {
        transform.rotation = rotation;
    }
}
