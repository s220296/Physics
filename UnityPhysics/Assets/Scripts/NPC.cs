using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class NPC : MonoBehaviour
{
    private Animator _animator = null;
    private NavMeshAgent _navAgent = null;

    private bool _isDead = false;

    private void Start()
    {
        _animator = GetComponent<Animator>();
        _navAgent = GetComponent<NavMeshAgent>();
    }

    // Update is called once per frame
    private void Update()
    {
        if(!_isDead) _navAgent.SetDestination(Vector3.zero);
    }

    public void Kill()
    {
        _animator.enabled = false;
        _isDead = true;
        _navAgent.isStopped = true;
    }
}
