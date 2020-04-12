using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelGenerator : MonoBehaviour
{
    public GameObject Ball;
    public GameObject Paddle;
    public GameObject RedBrick;
    public GameObject YellowBrick;
    public GameObject GreenBrick;
    public int level = 1;
   
    // Start is called before the first frame update
    void Start()
    {
        BuildLevel();
    }

    public void BuildLevel(){
        //Paddle Spawn
         Instantiate(Paddle,new Vector3(2.82f,-5.5f,0),transform.rotation);
        //Ball Spawn
         Instantiate(Ball,new Vector3(2.82f,-4.8f,0),transform.rotation);
        //Level Spawn
        if (level == 1){
           float xPos = 0f;
            for (int i = 0; i < 7; i++){
                Instantiate(RedBrick, new Vector3(-8.15f + xPos,4,0),transform.rotation);
                xPos += 2.7f;
                
            }

            
        }
         

    }
}
