using UnityEngine;

public class SteeringSignal
{
    private float globalTime = 0.0f;
    private float deltaTime;
    private int frequency;
    private float phaseShift;

    public SteeringSignal(float sampleRate, int frequency, float phaseShift = 0.0f)
    {
        deltaTime = 1.0f / sampleRate;
        this.phaseShift = (phaseShift * Mathf.PI / 180.0f) / frequency;
        this.frequency = frequency;
    }

    private float Apply(float inValue)
    {
        float outValue = inValue * Mathf.Sin(globalTime * frequency * 2.0f * Mathf.PI + phaseShift);
        globalTime += deltaTime;
        return outValue;
    }

    public void ApplyFilter(float[] inData)
    {
        for (int i = 0; i < inData.Length; i++)
        {
            inData[i] = Apply(inData[i]);
        }
    }
}
