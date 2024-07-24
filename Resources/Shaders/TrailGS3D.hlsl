#include "Trail3D.hlsli"

static const int vnum = 4;
static const int uv_vnum = 4;

static const float4 offset_array_front[vnum] =
{
    float4(-1.000000, 1.000000, 0.000000, 0),
    float4(-1.000000, -1.000000, 0.000000, 0),
    float4(1.000000, 1.000000, 0.000000, 0),
    float4(1.000000, -1.000000, 0.000000, 0)
};

static const float4 offset_array_top[vnum] =
{
    float4(1.000000, 1.000000, 0.000000, 0),
    float4(-1.000000, 1.000000, 0.000000, 0),
    float4(1.000000, 1.000000, 0.000000, 0),
    float4(-1.000000, 1.000000, 0.000000, 0)
};

static const float4 offset_array_down[vnum] =
{
    float4(1.000000, -1.000000, 0.000000, 0),
    float4(-1.000000, -1.000000, 0.000000, 0),
    float4(1.000000, -1.000000, 0.000000, 0),
    float4(-1.000000, -1.000000, 0.000000, 0)
};

static const float4 offset_array_back[vnum] =
{
    float4(-1.000000, 1.000000, 0.000000, 0),
    float4(-1.000000, -1.000000, 0.000000, 0),
    float4(1.000000, 1.000000, 0.000000, 0),
    float4(1.000000, -1.000000, 0.000000, 0)
};

static const float4 offset_array_left[vnum] =
{
    float4(-1.000000, 1.000000, 0.000000, 0),
    float4(-1.000000, -1.000000, 0.000000, 0),
    float4(-1.000000, 1.000000, 0.000000, 0),
    float4(-1.000000, -1.000000, 0.000000, 0)
};

static const float4 offset_array_right[vnum] =
{
    float4(1.000000, 1.000000, 0.000000, 0),
    float4(1.000000, -1.000000, 0.000000, 0),
    float4(1.000000, 1.000000, 0.000000, 0),
    float4(1.000000, -1.000000, 0.000000, 0)
};

static const float2 uv_offset_array_right[uv_vnum] =
{
    float2(0.000000, 0.000000),
    float2(0.000000, 1.000000),
    float2(1.000000, 0.000000),
    float2(1.000000, 1.000000)
};

GSOutput make_vertex(line VSOutput input, float4 offset,int uv_count)
{
    GSOutput element;
    
    float4 billoffset = mul(matBillboard, offset);
    billoffset = billoffset * input.size;
    element.svpos = input.svpos + billoffset;
    element.svpos = mul(view, element.svpos);
    element.svpos = mul(projection, element.svpos);

    element.uv = uv_offset_array_right[uv_count];
    element.uv.x = input.uv.x;
    element.color = input.color;
    
    return element;
}


[maxvertexcount(36)]
void main(
	line VSOutput input[2] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    float4 offset;
    
    ////正面
    //for (int i = 0; i < 2; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //    {   
    //        offset = offset_array_front[i + j];
    //        GSOutput element = make_vertex(input[0], offset);
           
    //        output.Append(element);
    //    }
    //    output.RestartStrip();
    //}
    
    ////バック
    //for (int i = 0; i < 2; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //    {
    //        offset = offset_array_back[i + j];
    //        GSOutput element = make_vertex(input[1], offset);
           
    //        output.Append(element);
    //    }
    //    output.RestartStrip();
    //}
    
    
    
    
    //上
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            GSOutput element;
                        
            offset = offset_array_top[i + j];
            
            if (j < 2 - i)
            {
                element = make_vertex(input[1], offset, i + j);
            }
            else
            {
                element = make_vertex(input[0], offset, i + j);
            }
            output.Append(element);
        }
        output.RestartStrip();
    }
    
    //下
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            GSOutput element;
                        
            offset = offset_array_down[i + j];
            
            if (j < 2 - i)
            {
                element = make_vertex(input[1], offset, i + j);
            }
            else
            {
                element = make_vertex(input[0], offset, i + j);
            }
            output.Append(element);
        }
        output.RestartStrip();
    }
    
    //左
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            GSOutput element;
                        
            offset = offset_array_left[i + j];
            
            if (j < 2 - i)
            {
                element = make_vertex(input[1], offset, i + j);
            }
            else
            {
                element = make_vertex(input[0], offset, i + j);
            }
            output.Append(element);
        }
        output.RestartStrip();
    }
    
    
    
    //右
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            GSOutput element;
                        
            offset = offset_array_right[i + j];
            
            if (j < 2 - i)
            {
                element = make_vertex(input[1], offset, i + j);
            }
            else
            {
                element = make_vertex(input[0], offset, i + j);
            }
            output.Append(element);
        }
        output.RestartStrip();
    }
    
    

}