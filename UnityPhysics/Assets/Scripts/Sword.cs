using System.Collections;
using System.Collections.Generic;
using UnityEditor.Build;
using UnityEngine;

public class Sword : MonoBehaviour
{
    private bool _enableDestruction = false;
    // Update is called once per frame
    void Update()
    {
        
    }

    public void EnableDestruction(bool enableDestruction) { _enableDestruction = enableDestruction; }

    private void OnTriggerEnter(Collider collision)
    {
        if (!collision.gameObject.CompareTag("NPC") || !_enableDestruction) return;

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
