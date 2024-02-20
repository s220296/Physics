using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sword : MonoBehaviour
{
    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (!collision.gameObject.CompareTag("NPC")) return;

        CharacterJoint cj = null;
        NPC npc = null;

        if(collision.gameObject.TryGetComponent<CharacterJoint>(out cj) && 
            collision.gameObject.name != "Spine2") // would be better to use tags
                                                   // if having multiple models
        {
            cj.breakForce = 1f;
        }
        npc = collision.gameObject.GetComponentInParent<NPC>();
        if(npc != null)
        {
            npc.Kill();
        }
    }
}
