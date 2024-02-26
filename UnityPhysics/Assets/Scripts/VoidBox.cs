using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VoidBox : MonoBehaviour
{
    [SerializeField] private Transform respawnPoint;

    private void OnTriggerEnter(Collider other)
    {
        Player player = other.GetComponent<Player>();

        if (player)
            player.transform.position = respawnPoint.localPosition;

    }
}
