using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BrickControl : MonoBehaviour
{
    public Sprite redBrick;
    public Sprite yellowBrick;
    public Sprite greenBrick;
    public UIManager uI;
    public SpriteRenderer spr;
    
    // Start is called before the first frame update
    void Start()
    {
        uI = GameObject.FindWithTag("uI").GetComponent<UIManager>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnCollisionEnter2D(Collision2D col){
       
        if (col.gameObject.tag == "Ball" && spr.sprite == greenBrick){
            uI.calcScore();
            Destroy(gameObject);
            
        }

        if (col.gameObject.tag == "Ball" && spr.sprite == yellowBrick){
            uI.calcScore();
            spr.sprite = greenBrick;
            
        }

        if (col.gameObject.tag == "Ball" && spr.sprite == redBrick){
            uI.calcScore();
            spr.sprite = yellowBrick;
            
        }

        
        

    }
}
