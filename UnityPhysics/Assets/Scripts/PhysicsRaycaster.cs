using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Camera))]
public class PhysicsRaycaster : MonoBehaviour
{
    public float force = 100f;
    public int layerMask;
    public Text output;

    Camera _camera;

    // Start is called before the first frame update
    private void Awake()
    {
        _camera = GetComponent<Camera>();
        
        //string[] layers = { "BrickWall" };
        // layerMask = LayerMask.GetMask(layers);
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetMouseButtonDown(0))
        {
            RaycastHit hit;
            Ray ray = _camera.ScreenPointToRay(Input.mousePosition);
            if(Physics.Raycast(ray, out hit, 1000))
            {
                output.text = hit.transform.gameObject.name;
            }
        }
    }
}
