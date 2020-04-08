using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallControl : MonoBehaviour
{
    public Rigidbody2D rigidBody;
    public float ballForce;
    public bool onPaddle;
    public Transform paddle;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (onPaddle){
            transform.position = paddle.position;
        }

        if(Input.GetKeyUp(KeyCode.Space)){
            onPaddle = false;
            rigidBody.AddForce(new Vector2(ballForce, ballForce));
        }
    }
    void OnCollisionEnter2D(Collision2D col){
        if (col.gameObject.tag == "Floor"){
            onPaddle = true;
        }
    }
}
