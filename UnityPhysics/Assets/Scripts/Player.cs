using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CharacterController))]
[RequireComponent(typeof(Animator))]
public class Player : MonoBehaviour
{
    private CharacterController _controller = null;
    private Animator _animator = null;

    public Transform hips = null;
    private bool _isRagdolling = false;

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

        if (_controller.enabled) _controller.SimpleMove(transform.up * Time.deltaTime);
        if (_controller.enabled) transform.Rotate(transform.up, horizontal * rotationSpeed * Time.deltaTime);
        if (_animator.enabled) _animator.SetFloat("Speed", vertical * forwardSpeed * Time.deltaTime);

        if(Input.GetKeyDown(KeyCode.Alpha0))
        {
            if (!_isRagdolling)
            {
                _animator.enabled = false;
                _controller.enabled = false;
                _isRagdolling = true;
            }
            else
            {
                transform.position = new Vector3(
                    hips.transform.position.x,
                    transform.position.y,
                    hips.transform.position.z);

                _animator.enabled = true;
                _controller.enabled = true;
                _isRagdolling = false;
            }
        }
    }

}
