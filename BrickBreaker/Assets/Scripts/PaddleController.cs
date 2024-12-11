using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PaddleController : MonoBehaviour
{
    public Rigidbody2D rigidBody;
    public float speed;
    public float maxX;
    public UIManager uI;


    // Start is called before the first frame update
    void Start()
    {
        uI = GameObject.FindWithTag("uI").GetComponent<UIManager>();
    }

    // Update is called once per frame
    void Update()
    {
      if(uI.gameOver){
        Destroy(gameObject);
        return;
      }
    
      /*float x = Input.GetAxis("Horizontal");
      if ( x < 0){
        MoveLeft();
      }

      if( x > 0){
        MoveRight();
      }

      if (x == 0) {
        Stop();
      }*/

      Vector3 pos = transform.position;
      pos.x = Mathf.Clamp(pos.x,-maxX, maxX);
      transform.position = pos;

       
    
    }

    void FixedUpdate(){
        float tilt = Input.acceleration.x;
        transform.Translate( tilt * speed * Time.deltaTime, 0, 0);
    }

    public void MoveLeft(){
         rigidBody.velocity = new Vector2 (-speed, 0);
         Debug.Log("LEFT");
    }

    public void MoveRight(){
        rigidBody.velocity = new Vector2 (speed, 0);
        Debug.Log("right");
    }

    void Stop(){
        rigidBody.velocity = Vector2.zero;
    }
}
