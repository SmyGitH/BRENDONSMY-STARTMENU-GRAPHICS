using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Menu : MonoBehaviour
{
   public void Quit(){
       Application.Quit();
       Debug.Log("Quit Game");
   }

   public void Play(){
       SceneManager.LoadScene("PlayGame");
   }
}
