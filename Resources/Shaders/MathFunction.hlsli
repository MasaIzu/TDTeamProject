
float length(float3 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float3 nomalize(float3 vec)
{
    float len = length(vec);
    if (len != 0)
    {
        return vec /= len;
    }
    return vec;
}