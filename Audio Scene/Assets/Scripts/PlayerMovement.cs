using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    public RandomAudioPlayer footsteps;
    public CharacterController controller;

    public float speed = 12f;
    public float sprintMultiplier = 12f;

    void Update()
    {
        float x = Input.GetAxis("Horizontal");
        float z = Input.GetAxis("Vertical");

        Vector3 move = transform.right * x + transform.forward * z;
        controller.Move(move * speed * (Input.GetKey(KeyCode.LeftShift) ? sprintMultiplier : 1f ) * Time.deltaTime);
    }
}
