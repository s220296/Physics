using Palmmedia.ReportGenerator.Core.Parser.Analysis;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

[RequireComponent(typeof(CharacterController))]
[RequireComponent(typeof(Animator))]
public class Player : MonoBehaviour
{
    private Animator _animator = null;
    private CharacterController _controller = null;

    private bool _isRagdolling = false;
    [SerializeField] private Transform hips = null;
    [SerializeField] private Transform sword = null;

    // sword doesnt hit if not swinging, do this
    private Sword _sword = null;

    public float forwardSpeed = 160f;
    public float rotationSpeed = 160f;
    public float pushPower = 2f;

    private Vector2 _swingStartPos = Vector2.zero;
    private Vector2 _swingEndPos = Vector2.zero;
    private bool _swinging = false;
    private bool _mouseDown = false;

    // Start is called before the first frame update
    void Start()
    {
        _animator = GetComponent<Animator>();
        _sword = sword.GetComponent<Sword>();
        _controller = GetComponent<CharacterController>();
    }

    // Update is called once per frame
    void Update()
    {
        float vertical = Input.GetAxis("Vertical");
        float horizontal = Input.GetAxis("Horizontal");

        if (!_isRagdolling) _controller.SimpleMove(transform.forward * (vertical * Time.deltaTime));
        if (!_isRagdolling) transform.Rotate(transform.up, horizontal * rotationSpeed * Time.deltaTime);
        if (_animator.enabled) _animator.SetFloat("Speed", vertical * forwardSpeed * Time.deltaTime);

        SwingCheck();
        InspectCheck();
    }

    private void InspectCheck()
    {
        if(Input.GetMouseButtonDown(1)) // if right click, shoot ray and get name of object clicked on
        {
            RaycastHit hit;
            bool rayHit = Physics.Raycast(Camera.main.ScreenPointToRay(new Vector3(Input.mousePosition.x, Input.mousePosition.y, 0)), out hit, 100f);
            if(rayHit)
            {
                Debug.Log(hit.collider.name);
                // Set UI text to hit.name
            }
        }
    }

    private void SwingCheck()
    {
        if(Input.GetMouseButtonDown(0) && !_swinging && !_mouseDown)
        {
            _swinging = true;
            _mouseDown = true;
            _swingStartPos = new Vector2(Input.mousePosition.x, Input.mousePosition.y);
        }
        else if(Input.GetMouseButtonUp(0) && _swinging && _mouseDown)
        {
            _mouseDown = false;
            _swingEndPos = new Vector2(Input.mousePosition.x, Input.mousePosition.y);
            if (Vector2.Distance(_swingStartPos, _swingEndPos) > 0.3f) StartCoroutine(SwingSword_CR());
            else _swinging = false;
        }
    }

    private IEnumerator SwingSword_CR()
    {
        _sword.EnableDestruction(true);

        Vector3 initialPos = sword.transform.localPosition;

        Vector2 swingDirection = (_swingEndPos - _swingStartPos).normalized;

        // 1.6 to each side from middle of frame
        // rotate blade towards direction

        // Get where sword should start outside of frame, begin there
        // Same for end of sword swing
        // lerp between two positions
        // rotate correctly
        Vector2 centerScreen = new Vector2(Screen.width / 2, Screen.height / 2);
        Vector2 edgePos = centerScreen;
        Vector2 endPos = centerScreen;
        while(edgePos.x > 0 && edgePos.x < Screen.width
            && edgePos.y > 0 && edgePos.y < Screen.height)
        {
            edgePos -= swingDirection;
            endPos += swingDirection;
        }

        Vector2 offset = new Vector2(1.6f, 1f);

        sword.localPosition += new Vector3(
            (edgePos.x / Screen.width) * offset.x - (offset.x * 0.5f),
            (edgePos.y / Screen.height) * offset.y - (offset.y * 0.5f),
            0);
        // add offset to sword swing and double swing distance
        Vector3 finalPos = new Vector3(
            (endPos.x / Screen.width) * (offset.x * 2f) - offset.x,
            (endPos.y / Screen.height) * (offset.y * 2f),
            sword.localPosition.z);

        float lingerTime = 0.1f;

        while(lingerTime > 0f)
        {
            sword.localPosition =
                Vector3.MoveTowards(sword.localPosition, finalPos, 7f * Time.deltaTime);

            if (sword.localPosition == finalPos)
                lingerTime -= Time.deltaTime;

            yield return null;
        }

        _sword.EnableDestruction(false);

        sword.transform.localPosition = initialPos;

        _swinging = false;
    }
}

