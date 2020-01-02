using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SimpleDrive : MonoBehaviour
{
    public Transform[] wayPoints;
    public float speed = 10f;
    public float rotationSpeed = 10f;

    private int currentWaypoint = 0;

    void Update()
    {
        var direction = wayPoints[currentWaypoint].position - transform.position;
        direction.Normalize();
        transform.position += direction * Time.deltaTime * speed;

        float rotationStep = Time.deltaTime * rotationSpeed;
        Vector3 newDirection = Vector3.RotateTowards(transform.forward, direction, rotationStep, 0.0f);
        transform.rotation = Quaternion.LookRotation(newDirection);

        if (Vector3.Distance(transform.position, wayPoints[currentWaypoint].position) <= 1.0f)
        {
            currentWaypoint++;
            currentWaypoint %= 4;
        }
    }
}
