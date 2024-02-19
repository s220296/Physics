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
        if(Input.anyKey == true && _timer >= 0)
        {
            Instantiate(prefab, transform.position, Quaternion.identity);
            _timer = 0;
        }

        _timer += Time.deltaTime;
    }
}
