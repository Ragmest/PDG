using UnityEngine.Audio;
using UnityEngine;

[System.Serializable]
public class Sound
{
    [Range(0f, 1f)]
    public float volume = 1f;
    [Range(0.1f, 3f)]
    public float pitch = 1f;

    [HideInInspector]
    public AudioClip clip;
}
