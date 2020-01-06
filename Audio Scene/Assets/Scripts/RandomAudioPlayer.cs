using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class RandomAudioPlayer : MonoBehaviour
{
    public AudioClip[] sounds;
    public float playDelay = 0.5f;
    [Header("Pitch Randomization")]
    [Range(-3.0f, 3.0f)]
    public float basePitch = 1.0f;
    public float pitchDeviation = 0.0f;
    
    private AudioSource audioSource;
    private float playTimeout;

    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
        playTimeout = playDelay;
    }

    public void PlayRandom()
    {
        playTimeout -= Time.deltaTime;

        if (playTimeout < 0)
        {
            playTimeout = playDelay;
            audioSource.clip = sounds[Random.Range(0, sounds.Length)];
            audioSource.pitch = Random.Range(
                Mathf.Clamp(basePitch - pitchDeviation, -3.0f, 3.0f),
                Mathf.Clamp(basePitch + pitchDeviation, -3.0f, 3.0f));
            audioSource.Play();
        }
    }
}
