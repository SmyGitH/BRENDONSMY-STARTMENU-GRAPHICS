using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class UIManager : MonoBehaviour
{
    public int score = 0;
    public int lives = 3;
    public Text scoreText;
    public Text liveText;
    public bool gameOver;
    public GameObject gameOverPanel;
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

    if(lives <= 0){
        lives = 0;
        GameOver();
        }
    }

    public void GameOver(){
        gameOver = true;
        gameOverPanel.SetActive(true);
    }

    public void Quit(){
       SceneManager.LoadScene("MainMenu");
       Debug.Log("Quit Game");
   }

   public void PlayAgain(){
       SceneManager.LoadScene("PlayGame");
   }


}
