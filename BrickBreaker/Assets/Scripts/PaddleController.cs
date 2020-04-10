using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PaddleController : MonoBehaviour
{
    public Rigidbody2D rigidBody;
    public float speed;
    public float maxX;


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
    
      float x = Input.GetAxis("Horizontal");
      if ( x < 0){
        MoveLeft();
      }

      if( x > 0){
        MoveRight();
      }

      if (x == 0) {
        Stop();
      }

      Vector3 pos = transform.position;
      pos.x = Mathf.Clamp(pos.x,-maxX, maxX);
      transform.position = pos;
    
    }

    void MoveLeft(){
         rigidBody.velocity = new Vector2 (-speed, 0);
    }

    void MoveRight(){
        rigidBody.velocity = new Vector2 (speed, 0);
    }

    void Stop(){
        rigidBody.velocity = Vector2.zero;
    }
}
