using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallControl : MonoBehaviour
{
    public UIManager uI;
    public Rigidbody2D rigidBody;
    public float ballForce;
    public bool onPaddle;
    public Transform paddle;
    public GameObject mod;
    // Start is called before the first frame update
    void Start()
    {
        uI = GameObject.FindWithTag("uI").GetComponent<UIManager>();
    }

    // Update is called once per frame
    void Update()
    {
        if (onPaddle){
            transform.position = paddle.position;
        }

        if(Input.GetKeyUp(KeyCode.Space)){
            Instantiate(mod,new Vector3(5,5,0),transform.rotation);
            onPaddle = false;
            rigidBody.velocity = new Vector2(ballForce, ballForce);
        }
    }
    void OnCollisionEnter2D(Collision2D col){
        if (col.gameObject.tag == "Floor"){
            uI.calcLives();
            onPaddle = true;
        }
    }
}
