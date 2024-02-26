using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingPlatform : MonoBehaviour
{
    Transform _playerTransform;
    bool _playerOnPlatform;
    Vector3 _oldPosition;

    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.CompareTag("Player"))
        {
            _playerTransform = collision.gameObject.transform;

            while (_playerTransform.parent != null)
                _playerTransform = _playerTransform.parent;

            Debug.Log(_playerTransform.name);

            _playerOnPlatform = true;
        }
    }

    private void FixedUpdate()
    {
        Vector3 positionDelta = transform.position - _oldPosition;
        Debug.Log(positionDelta);

        if (_playerOnPlatform)
            _playerTransform.position += positionDelta;

        _oldPosition = transform.position;
    }

    private void OnCollisionExit(Collision collision)
    {
        if(collision.gameObject.CompareTag("Player"))
        {
            _playerOnPlatform = false;
        }
    }
}
