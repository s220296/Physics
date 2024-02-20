using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    public Transform prefab;
    private float _timer = 0;

    // Update is called once per frame
    void Update()
    {
        if(_timer >= 1)
        {
            Spawn();
            _timer = 0;
        }

        _timer += Time.deltaTime;
    }

    public void Spawn()
    {
        Instantiate(prefab, transform.position, Quaternion.identity);
    }
}
