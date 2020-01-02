using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class RandomAudioPlayer : MonoBehaviour
{
    public AudioClip[] sounds;

    private AudioSource audioSource;
    
    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
    }

    public void PlayRandom()
    {
        if(!audioSource.isPlaying)
        {
            audioSource.clip = sounds[Random.Range(0, sounds.Length - 1)];
            audioSource.Play();
        }
    }
}
