#include "ParticleEditor.hlsli"

RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(128, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < MaxParticleCount)
    {
        uint index = id.x;
        uint seed = id.x + index * 1235;
        gParticles[index].isActive = 0;
        float GraceOfTimeMax = 100.0f;
        float GraceOfTimeMin = 1.0f;
        float GraceOfTime = Rand(seed, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
        gDeadIndexList.Append(index);
    }
}

[numthreads(128, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    uint seed = id.x + index * 1235;
    if (index >= MaxParticleCount)
    {
        return;
    }
    if (gParticles[index].isActive == 0)
    {
        return;
    }
    //if (Shot == false)
    //{
    //    return;
    //}
    const float dt = 1;
    bool isScaleChange = false;
    float3 Position = gParticles[index].position.xyz;
    float3 Velocity = gParticles[index].velocity.xyz;
    
    gParticles[index].GroupTimer = gParticles[index].GroupTimer - dt;
    if (ParticleGroup == 1)
    {
        if (ParticleGroupCount > index)
        {
            if (gParticles[index].GroupTimer <= 0)
            {
                gParticles[index].scale = 0;
                isScaleChange = true;
            }
        }
        else
        {
            gParticles[index].position.xyz = Position;
            if (gParticles[gParticles[index].GroupNumber].GroupTimer <= 0)
            {
                gParticles[index].position.xyz = Position;
            }
            else
            {
                Position = gParticles[gParticles[index].GroupNumber].position;
            }
        }
    }
    
    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    gParticles[index].MaxGroupTimer = gParticles[index].MaxGroupTimer - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        if (ParticleGroup == 1)
        {
            if (gParticles[index].MaxGroupTimer <= 0)
            {
                gParticles[index].isActive = 0;
                return;
            }
        }
        else
        {
            gParticles[index].isActive = 0;
            return;
        }
    }
    
    if (EndPointActive)
    {
        float3 EndPosPoint = EndPos.xyz;
        float3 PosToEndVec = normalize(EndPosPoint - Position);
        if (Interlocking)
        {
            // 生き残っているパーティクルを動かす.
            float4 BladeEndPos = normalize(EndPos - StartPos);
            BladeEndPos = normalize((StartPos + (BladeEndPos * InterlockingLength)) - gParticles[index].position);
            Velocity = gParticles[index].velocity.xyz;
            Velocity = lerp(Velocity, BladeEndPos.xyz, InterlockingStrength);
            Velocity = normalize(Velocity) * gParticles[index].Speed;
            Position += Velocity;
            gParticles[index].velocity.xyz = Velocity;
        }
        else
        {
            Velocity = normalize(lerp(Velocity, PosToEndVec, LerpStrength));
            gParticles[index].velocity.xyz = Velocity;
            Position += Velocity * gParticles[index].Speed;
        }
    }
    else
    {
        if (ParticleGroup == 0)
        {
            Position += Velocity * gParticles[index].Speed;
            Velocity.y = Velocity.y - GravityStrength;
            gParticles[index].velocity.xyz = normalize(Velocity);
        }
        else
        {
            if (ParticleGroupCount > index)
            {
                Position += Velocity * gParticles[index].Speed;
            }
            else
            {
                if (gParticles[index].GroupTimer <= 0)
                {
                    float3 vel = normalize(Velocity + gParticles[gParticles[index].GroupNumber].velocity.xyz);
                    Position += vel * gParticles[index].Speed;
                    Velocity.y = Velocity.y - GravityStrength;
                    gParticles[index].velocity.xyz = normalize(Velocity);
                }
            }
        }
    }
    
    float4 Color = EndColor - StartColor;
    float Life = gParticles[index].MaxLifeTime - gParticles[index].lifeTime;
    float Ratio = Life / gParticles[index].MaxLifeTime;
    Color *= Ratio;
    
    gParticles[index].ScaleKeep += ScaleTinker;
    float scale = 1.0f;
    if (RandomScale)
    {
        scale = gParticles[index].scale;
    }
    else
    {
        scale = Scale;
    }
    if (ScaleDownLifeTime)
    {
        scale = Scale * (gParticles[index].lifeTime / gParticles[index].MaxLifeTime);
    }
    
    if (CollisionON == 1)
    {
        for (int x = 0; x <= ColCount; ++x)
        {
            float3 center = ColPos[x].xyz;
            float3 scale = ColScale[x].xyz;
            
            // 箱の最小頂点と最大頂点を計算
            float3 minBox = center - scale;
            float3 maxBox = center + scale;
            
            // 点の位置
            float3 pointPos = Position;
            
            // 各軸に沿った内側の確認
            bool insideX = false;
            bool insideY = false;
            bool insideZ = false;
            
            if (pointPos.x >= minBox.x && pointPos.x <= maxBox.x)
            {
                insideX = true;
            }
            if (pointPos.y >= minBox.y && pointPos.y <= maxBox.y)
            {
                insideY = true;
            }
            if (pointPos.z >= minBox.z && pointPos.z <= maxBox.z)
            {
                insideZ = true;
            }
            
            // 点を押し出す処理
            if (insideX && insideY && insideZ)
            {
                // 各軸に対する最小距離を計算
                float distanceX = min(abs(pointPos.x - minBox.x), abs(maxBox.x - pointPos.x));
                float distanceY = min(abs(pointPos.y - minBox.y), abs(maxBox.y - pointPos.y));
                float distanceZ = min(abs(pointPos.z - minBox.z), abs(maxBox.z - pointPos.z));
        
                // 最も近い面を決定
                float minDistance = min(distanceX, min(distanceY, distanceZ));
        
                // 押し出しベクトルを計算
                float3 pushOutVector = 0;
                if (minDistance == distanceX)
                {
                    pushOutVector.x = (pointPos.x > center.x) ? distanceX : -distanceX;
                }
                else if (minDistance == distanceY)
                {
                    pushOutVector.y = (pointPos.y > center.y) ? distanceY : -distanceY;
                }
                else // Z軸が最小距離
                {
                    pushOutVector.z = (pointPos.z > center.z) ? distanceZ : -distanceZ;
                }
        
                // 点を押し出す
                pointPos += pushOutVector;
                Position = pointPos;
            }
        }
    }
    
    gParticles[index].color = StartColor + Color;
    gParticles[index].position.xyz = Position;
    
    if (GettingUpDownScale == 0)
    {
        gParticles[index].scale = scale + gParticles[index].ScaleKeep;
    }
    else
    {
        float HalfLife = gParticles[index].MaxLifeTime / 2;
        float NowLife = gParticles[index].MaxLifeTime - gParticles[index].lifeTime;
                
        uint UintLessHalf = gParticles[index].lifeTime / HalfLife;
        uint UintOverHalf = NowLife / (HalfLife + 1);
        
        float ScaleRatio = UintLessHalf * (NowLife / HalfLife) + UintOverHalf * (gParticles[index].lifeTime / HalfLife);
        
        gParticles[index].scale = Scale * ScaleRatio;
    }
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);
[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    
    if (gParticles[index].isActive > 0)
    {
        return;
    }
    uint seed = id.x + index * 1235;
    uint indexAdd = index * 1222;
    uint indexAdd2 = indexAdd * 12;
    uint indexAdd3 = indexAdd * 12;
    if (Shot == 0)
    {
        float GraceOfTimeMax = 100.0f;
        float GraceOfTimeMin = 1.0f;
        float GraceOfTime = Rand(seed, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
        return;
    }
    gParticles[index].isActive = 1;
    
    if (!EmitParticles)
    {
        const float dt = 1;
        if (gParticles[index].graceOfTime >= 0)
        {
            gParticles[index].graceOfTime = gParticles[index].graceOfTime - dt;
            return;
        }
    }
    
    if (isLoad)
    {
        float GraceOfTimeMax = 100.0f;
        float GraceOfTimeMin = 1.0f;
        float GraceOfTime = Rand(indexAdd, GraceOfTimeMax, GraceOfTimeMin);
        gParticles[index].graceOfTime = GraceOfTime;
    }
    
    float3 velocity;
    
    if (RandomVelocity)
    {
        
        velocity.x = nextRand(indexAdd) * nextRand1(seed) / VelocityAdjustment.x;
        velocity.y = nextRand(indexAdd) * nextRand1(seed) / VelocityAdjustment.y;
        velocity.z = nextRand(indexAdd) * nextRand1(seed) / VelocityAdjustment.z;
    
        velocity = normalize(velocity);
        
        gParticles[index].velocity.xyz = velocity;
    }
    else
    {
        velocity = normalize(EndPos.xyz - StartPos.xyz);
        
        float PI = 3.141592;
        
        float RadX = (Angle.x * (PI / 180)) * 100;
        float RadY = (Angle.y * (PI / 180)) * 100;
        float RadZ = (Angle.z * (PI / 180)) * 100;
        
        float RandomAngleMin = 0.0f;
        float RandomAngleX = (Rand(seed, RadX, RandomAngleMin) - (RadX / 2)) / 100;
        float4 rotationX = QuaternionFromAxisAngle(float3(1, 0, 0), RandomAngleX);
        
        float RandomAngleY = (Rand(indexAdd, RadY, RandomAngleMin) - (RadY / 2)) / 100;
        float4 rotationY = QuaternionFromAxisAngle(float3(0, 1, 0), RandomAngleY);
        
        float RandomAngleZ = (Rand(indexAdd2, RadZ, RandomAngleMin) - (RadZ / 2)) / 100;
        float4 rotationZ = QuaternionFromAxisAngle(float3(0, 0, 1), RandomAngleZ);
        
        float3 RotVelocity = RotateVectorByQuaternion(velocity, rotationX);
        RotVelocity = RotateVectorByQuaternion(RotVelocity, rotationY);
        RotVelocity = RotateVectorByQuaternion(RotVelocity, rotationZ);
        
        gParticles[index].velocity.xyz = RotVelocity;
    }
    
    if (ParticleGroup == 1)
    {
        gParticles[index].GroupNumber = Rand(seed, ParticleGroupCount, 0);
        gParticles[index].MaxGroupTimer = MaxGroupTimer;
        
        if (ParticleGroupCount > index)
        {
            if (RandomLife)
            {
                float RandomLife_ = Rand(seed, RandomLifeMinMax.y, RandomLifeMinMax.x);
                gParticles[index].lifeTime = RandomLife_ + GroupTimer;
            }
            else
            {
                gParticles[index].lifeTime = MaxGroupTimer;
            }
            
            if (RandomParticleExplosion == 0)
            {
                gParticles[index].GroupTimer = GroupTimer;
            }
            else
            {
                float RandomExplosionTimer_ = Rand(seed, RandomGroupTimerMinMax.y, RandomGroupTimerMinMax.x);
                gParticles[index].GroupTimer = RandomExplosionTimer_;
            }
        }
        else
        {
            if (RandomLife)
            {
                float RandomLife_ = Rand(seed, RandomLifeMinMax.y, RandomLifeMinMax.x);
                gParticles[index].lifeTime = RandomLife_ + GroupTimer;
            }
            else
            {
                gParticles[index].lifeTime = MaxGroupTimer;
            }
            
            if (RandomParticleExplosion == 0)
            {
                gParticles[index].GroupTimer = GroupTimer;
            }
            else
            {
                gParticles[index].GroupTimer = gParticles[gParticles[index].GroupNumber].GroupTimer;
            }
        }
        
    }
    else
    {
        if (RandomLife)
        {
            float RandomLife_ = Rand(seed, RandomLifeMinMax.y, RandomLifeMinMax.x);
            gParticles[index].lifeTime = RandomLife_;
        }
        else
        {
            gParticles[index].lifeTime = MaxLife;
        }
    }
    
    if (RandomSpeed)
    {
        float RandomSpeed_ = Rand(seed, RandomSpeedMinMax.y, RandomSpeedMinMax.x) / SpeedDivideSize;
        gParticles[index].Speed = RandomSpeed_;
    }
    else
    {
        gParticles[index].Speed = Speed;
    }
    
    if (RandomScale)
    {
        float RandomScale_ = Rand(seed, RandomScaleMinMax.y, RandomScaleMinMax.x) / ScaleDivideSize;
        gParticles[index].scale = RandomScale_;
    }
    else
    {
        gParticles[index].scale = Scale;
    }
    
    float3 NowPosition = StartPos.xyz;
    
    if (ShapeNumber == 0)
    {
        
    }
    else if (ShapeNumber == 1)
    {   
        NowPosition = GenerateRandomPoint(seed, indexAdd, indexAdd2, NowPosition, Width, Height, Depth);
        NowPosition *= ShapeScale.xyz;
    }
    else if (ShapeNumber == 2)
    {
        // 三角形の頂点
        float3 A = { -1, 0, -1 };
        float3 B = { 1, 0, -1 };
        float3 C = { 0, 1, 0 };
        float3 D = { 0, 0, 1 };
        
        A = normalize(A);
        B = normalize(B);
        C = normalize(C);
        D = normalize(D);
        
        float randomX = Rand1(seed, Width, 0.0);
        float randomY = Rand1(indexAdd, Width, 0.0);
        float randomZ = Rand1(indexAdd2, Height, 0.0);
        float randomW = Rand1(indexAdd3, Depth, 0.0);
        
        NowPosition += GenerateRandomPointInTetrahedron(seed, indexAdd, indexAdd2, indexAdd3, A, B, C, D);
        NowPosition *= ShapeScale.xyz;
    }
    else
    {
        //float3 RandomSeedF3 = (seed, indexAdd, indexAdd2);
        //NowPosition += RandomPointInSphere(Width, RandomSeedF3);
    }
    
    gParticles[index].MaxLifeTime = gParticles[index].lifeTime;
    gParticles[index].position.xyz = NowPosition;
    gParticles[index].color = StartColor;
    gParticles[index].ScaleKeep = 0;
    gParticles[index].PostEffectPow = PostEffectPow;

}
