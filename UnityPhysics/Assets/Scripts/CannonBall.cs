using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class CannonBall : MonoBehaviour
{
    public float forceOfFire = 300f;

    private bool _canFire = true;
    private Rigidbody _rb = null;

    // Start is called before the first frame update
    void Awake()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.anyKeyDown && _canFire)
        {
            _rb.isKinematic = false;
            _rb.AddForce(transform.forward * forceOfFire);
            _canFire = false;
        }
    }
}
