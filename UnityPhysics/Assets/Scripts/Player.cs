using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CharacterController))]
[RequireComponent(typeof(Animator))]
public class Player : MonoBehaviour
{
    private CharacterController _controller = null;
    private Animator _animator = null;

    public float forwardSpeed = 160f;
    public float rotationSpeed = 160f;
    public float pushPower = 2f;

    // Start is called before the first frame update
    void Start()
    {
        _controller = GetComponent<CharacterController>();
        _animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        float vertical = Input.GetAxis("Vertical");
        float horizontal = Input.GetAxis("Horizontal");

        _controller.SimpleMove(transform.up * Time.deltaTime);
        transform.Rotate(transform.up, horizontal * rotationSpeed * Time.deltaTime);
        _animator.SetFloat("Speed", vertical * forwardSpeed * Time.deltaTime);
    }

    private void OnControllerColliderHit(ControllerColliderHit hit)
    {
        Rigidbody body = hit.collider.attachedRigidbody;
        if(body == null || body.isKinematic || hit.moveDirection.y < -0.3f)
            return;

        Vector3 pushDirection = new Vector3(hit.moveDirection.x, 0, hit.moveDirection.y);
        body.velocity = pushDirection * pushPower;
    }
}
