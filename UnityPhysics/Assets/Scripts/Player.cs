using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

[RequireComponent(typeof(CharacterController))]
[RequireComponent(typeof(Animator))]
public class Player : MonoBehaviour
{
    private CharacterController _controller = null;
    private Animator _animator = null;

    private bool _isRagdolling = false;
    [SerializeField] private Transform hips = null;
    [SerializeField] private Transform sword = null;

    public float forwardSpeed = 160f;
    public float rotationSpeed = 160f;
    public float pushPower = 2f;

    private Vector2 _swingStartPos;
    private Vector2 _swingEndPos;
    private bool _swinging;

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

        SwingCheck();
    }

    private void SwingCheck()
    {
        if(Input.GetMouseButtonDown(0) && !_swinging)
        {
            _swinging = true;
            _swingStartPos = new Vector2(Input.mousePosition.x, Input.mousePosition.y);
            Debug.Log(_swingStartPos);
        }
        else if(Input.GetMouseButtonUp(0) && _swinging)
        {
            _swingEndPos = new Vector2(Input.mousePosition.x, Input.mousePosition.y);
            StartCoroutine(SwingSword_CR());
        }
    }

    private IEnumerator SwingSword_CR()
    {
        Vector2 swingDirection = (_swingEndPos - _swingStartPos).normalized;

        float timer = 1.5f;
        // 1.6 to each side from middle of frame
        // rotate blade towards direction

        Vector2 centerScreen = new Vector2(Screen.width / 2, Screen.height / 2);

        while(timer > 0)
        {


            timer -= Time.deltaTime;

            yield return null;
        }

        _swinging = false;

        yield return null;
    }
}

