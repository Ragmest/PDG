using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotor : MonoBehaviour
{
    public float speedX = 0f;
    public float speedY = 0f;
    public float speedZ = 0f;

    // Update is called once per frame
    void Update()
    {
        transform.Rotate(Time.deltaTime * speedX, Time.deltaTime * speedY, Time.deltaTime * speedZ);
    }
}
