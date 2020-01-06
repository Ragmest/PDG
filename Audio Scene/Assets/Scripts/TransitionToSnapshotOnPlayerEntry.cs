using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class TransitionToSnapshotOnPlayerEntry : MonoBehaviour
{
    public List<AudioMixerSnapshot> snapshots;
    public float overTime;

    void OnTriggerEnter(Collider other)
    {
        if (!other.CompareTag("Player")) return;

        foreach(var snapshot in snapshots)
        {
            snapshot.TransitionTo(overTime);
        }
    }
}
