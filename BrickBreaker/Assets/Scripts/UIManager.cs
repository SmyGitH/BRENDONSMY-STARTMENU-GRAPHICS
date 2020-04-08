using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    int score = 0;
    int lives = 3;
    public Text scoreText;
    public Text liveText;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

   public void calcScore(){
        score++;
        scoreText.text = "Score: " + score;
    }

    public void calcLives(){
        lives--;
        liveText.text = "Lives: " + lives;
    }


}
