using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Car : MonoBehaviour
{
    [SerializeField] private HingeJoint frontLeftJoint;
    [SerializeField] private HingeJoint frontRightJoint;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
       frontLeftJoint.axis += new Vector3(0, 0, Input.GetAxis("Horizontal") * Time.deltaTime);
       frontRightJoint.axis += new Vector3(0, 0, Input.GetAxis("Horizontal") * Time.deltaTime);
    }
}
