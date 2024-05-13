
float EasingIn(float start, float end, float time, float max_time)
{
    time /= max_time;
    float move = end - start;
    return start + (move * time * time);
}

float3 InVec3(float3 start, float3 end, float time, float max_time)
{
    float3 In;
   
    In.x = EasingIn(start.x, end.x, time, max_time);
    In.y = EasingIn(start.y, end.y, time, max_time);
    In.z = EasingIn(start.z, end.z, time, max_time);

    return In;
}