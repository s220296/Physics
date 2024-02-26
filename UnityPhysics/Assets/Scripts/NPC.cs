using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class NPC : MonoBehaviour
{
    protected static Transform playerTransform = null;

    private Animator _animator = null;
    private NavMeshAgent _navAgent = null;

    private bool _isDead = false;

    [SerializeField] private float _deathDespawnTimer = 20f;

    private void Start()
    {
        _animator = GetComponent<Animator>();
        _navAgent = GetComponent<NavMeshAgent>();
        playerTransform = FindObjectOfType<Player>().transform;
    }

    // Update is called once per frame
    private void Update()
    {
        if(!_isDead) _navAgent.SetDestination(playerTransform.position);
    }

    public void Kill()
    {
        _animator.enabled = false;
        _isDead = true;
        _navAgent.isStopped = true;

        Destroy(gameObject, _deathDespawnTimer);
    }
}
