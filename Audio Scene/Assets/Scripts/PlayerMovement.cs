using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    public List<RandomAudioPlayer> footsteps;
    public CharacterController controller;

    public float speed = 12f;
    public float gravity = -9.81f;
    public float sprintMultiplier = 12f;

    public Transform groundCheck;
    public float groundDistance = 0.4f;
    public LayerMask groundMask;

    Vector3 velocity;
    bool isGrounded;

    void Update()
    {
        isGrounded = Physics.CheckSphere(groundCheck.position, groundDistance, groundMask);

        if(isGrounded && velocity.y < 0)
        {
            velocity.y = -2f;
        }

        float x = Input.GetAxis("Horizontal");
        float z = Input.GetAxis("Vertical");

        Vector3 move = transform.right * x + transform.forward * z;
        Vector3 moveVector = move * speed * (Input.GetKey(KeyCode.LeftShift) ? sprintMultiplier : 1f) * Time.deltaTime;
        if (moveVector.magnitude > 0.1f)
        {
            foreach (var footstep in footsteps)
            {
                footstep.PlayRandom();
            }
        }
        controller.Move(moveVector);

        velocity.y += gravity * Time.deltaTime;
        controller.Move(velocity * Time.deltaTime);
    }
}
