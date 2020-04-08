using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BrickControl : MonoBehaviour
{
    public UIManager uI;
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
        if (col.gameObject.tag == "Ball"){
            uI.calcScore();
            Destroy(gameObject);
        }
    }
}
