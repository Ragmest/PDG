using System;
using UnityEngine;
using UnityEngine.Audio;

public class HelicopterSound : MonoBehaviour
{
    AudioSource source;

    [Header("Stearing signal")]
    [Range(0.0f, 360.0f)]
    public float phaseShift = 0;
    public int steeringFrequency = 0;
    private SteeringSignal steeringSignal;

    [Header("Clip info")]
    public int sampleRate = 44100;
    public int duration = 1;
    public int channels = 2;
    [Range(0.0f, 1.0f)]
    public float amplitude = 1.0f;

    System.Random rng = new System.Random();

    private void Start()
    {
        steeringSignal = new SteeringSignal(sampleRate, steeringFrequency, phaseShift);
        AudioClip buffer = AudioClip.Create("white noise", sampleRate * duration, channels, sampleRate, true, generateWhiteNoise);

        source = GetComponent<AudioSource>();
        if (source != null)
        {
            source.clip = buffer;
            source.loop = true;
            source.Play();
        }
    }

    void generateWhiteNoise(float[] data)
    {
        for (int i = 0; i < data.Length; i++)
            data[i] = (float)((rng.NextDouble() * 2.0) - 1.0) * amplitude;

        steeringSignal.ApplyFilter(data);
    }
}
