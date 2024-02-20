using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sword : MonoBehaviour
{
    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision collision)
    {
        CharacterJoint cj = null;
        Animator an = null;
        CharacterController cc = null;
        if(collision.gameObject.TryGetComponent<CharacterJoint>(out cj) && 
            collision.gameObject.name != "Spine2")
        {
            cj.breakForce = 1f;
        }
        an = collision.gameObject.GetComponentInParent<Animator>();
        if(an != null)
        {
            an.enabled = false;
        }
        cc = collision.gameObject.GetComponentInParent<CharacterController>();
        if(cc != null)
        {
            cc.enabled = false;
        }
    }
}
