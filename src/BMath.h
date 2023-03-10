/* date = March 4th 2023 9:37 am */

#ifndef _B_MATH_H
#define _B_MATH_H

#include <math.h>

typedef union v2
{
    struct
    {
        f32 x, y;
    };
    
    f32 e[2];
} v2;

typedef union v3
{
    struct
    {
        f32 x, y, z;
    };
    struct
    {
        f32 r, g, b;
    };
    
    f32 e[3];
} v3;

internal inline v3
V3AddValue(v3 Vec, f32 Value)
{
    v3 Result;
    Result.x = Vec.x + Value;
    Result.y = Vec.y + Value;
    Result.z = Vec.z + Value;
    
    return(Result);
}

internal inline v3
V3Scale(v3 Vec, f32 Scale)
{
    v3 Result;
    Result.x = Vec.x*Scale;
    Result.y = Vec.y*Scale;
    Result.z = Vec.z*Scale;
    
    return(Result);
}

internal inline v3
V3Multiply(v3 A, v3 B)
{
    v3 Result;
    Result.x = A.x*B.x;
    Result.y = A.y*B.y;
    Result.z = A.z*B.z;
    
    return(Result);
}

internal inline f32
V3MultiplyDot(v3 A, v3 B)
{
    f32 Result = A.x*B.x + A.y*B.y + A.z*B.z;
    return(Result);
}

internal inline f32
V3Length(v3 Vec)
{
    f32 Mult = V3MultiplyDot(Vec, Vec);
    
    f32 Result = 0.0f;
    if(Mult != 0)
    {
        Result = sqrtf(Mult);
    }
    
    return(Result);
}

internal v3
V3Normalize(v3 Vec)
{
    f32 Length = V3Length(Vec);
    
    f32 K = 0.0f;
    if(Length != 0)
    {
        K = 1.0f / V3Length(Vec);
    }
    
    v3 Result = V3Scale(Vec, K);
    return(Result);
}

typedef union v4
{
    struct
    {
        f32 x, y, z, w;
    };
    struct
    {
        f32 r, g, b, a;
    };
    
    f32 e[4];
} v4;

internal inline v4
V4Add(v4 A, v4 B)
{
    v4 Result;
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;
    Result.w = A.w + B.w;
    
    return(Result);
}

internal inline v4
V4Scale(v4 Vec, f32 Scale)
{
    v4 Result;
    Result.x = Vec.x*Scale;
    Result.y = Vec.y*Scale;
    Result.z = Vec.z*Scale;
    Result.w = Vec.w*Scale;
    
    return(Result);
}

typedef union mat4
{
    v4 e[4];
    f32 m[4][4];
} mat4;

internal inline mat4
Mat4Identity(void)
{
    mat4 Result =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    return(Result);
}

internal void
Mat4Multiply(mat4 *OutMat, mat4 A, mat4 B)
{
    mat4 Result =
    {
        A.m[0][0] * B.m[0][0] + A.m[0][1] * B.m[1][0] + A.m[0][2] * B.m[2][0] + A.m[0][3] * B.m[3][0],
        A.m[0][0] * B.m[0][1] + A.m[0][1] * B.m[1][1] + A.m[0][2] * B.m[2][1] + A.m[0][3] * B.m[3][1],
        A.m[0][0] * B.m[0][2] + A.m[0][1] * B.m[1][2] + A.m[0][2] * B.m[2][2] + A.m[0][3] * B.m[3][2],
        A.m[0][0] * B.m[0][3] + A.m[0][1] * B.m[1][3] + A.m[0][2] * B.m[2][3] + A.m[0][3] * B.m[3][3],
        A.m[1][0] * B.m[0][0] + A.m[1][1] * B.m[1][0] + A.m[1][2] * B.m[2][0] + A.m[1][3] * B.m[3][0],
        A.m[1][0] * B.m[0][1] + A.m[1][1] * B.m[1][1] + A.m[1][2] * B.m[2][1] + A.m[1][3] * B.m[3][1],
        A.m[1][0] * B.m[0][2] + A.m[1][1] * B.m[1][2] + A.m[1][2] * B.m[2][2] + A.m[1][3] * B.m[3][2],
        A.m[1][0] * B.m[0][3] + A.m[1][1] * B.m[1][3] + A.m[1][2] * B.m[2][3] + A.m[1][3] * B.m[3][3],
        A.m[2][0] * B.m[0][0] + A.m[2][1] * B.m[1][0] + A.m[2][2] * B.m[2][0] + A.m[2][3] * B.m[3][0],
        A.m[2][0] * B.m[0][1] + A.m[2][1] * B.m[1][1] + A.m[2][2] * B.m[2][1] + A.m[2][3] * B.m[3][1],
        A.m[2][0] * B.m[0][2] + A.m[2][1] * B.m[1][2] + A.m[2][2] * B.m[2][2] + A.m[2][3] * B.m[3][2],
        A.m[2][0] * B.m[0][3] + A.m[2][1] * B.m[1][3] + A.m[2][2] * B.m[2][3] + A.m[2][3] * B.m[3][3],
        A.m[3][0] * B.m[0][0] + A.m[3][1] * B.m[1][0] + A.m[3][2] * B.m[2][0] + A.m[3][3] * B.m[3][0],
        A.m[3][0] * B.m[0][1] + A.m[3][1] * B.m[1][1] + A.m[3][2] * B.m[2][1] + A.m[3][3] * B.m[3][1],
        A.m[3][0] * B.m[0][2] + A.m[3][1] * B.m[1][2] + A.m[3][2] * B.m[2][2] + A.m[3][3] * B.m[3][2],
        A.m[3][0] * B.m[0][3] + A.m[3][1] * B.m[1][3] + A.m[3][2] * B.m[2][3] + A.m[3][3] * B.m[3][3],
    };
    
    *OutMat = Result;
}

internal void
CreateOrthographic(mat4 *OutMat,
                   f32 Left, f32 Right,
                   f32 Top, f32 Bottom,
                   f32 Near, f32 Far)
{
    mat4 Result = Mat4Identity();
    Result.m[0][0] = 2.0f / (Right - Left);
    Result.m[1][1] = 2.0f / (Top - Bottom);
    Result.m[2][2] = -2.0f / (Far - Near);
    Result.m[3][0] = -(Right + Left) / (Right - Left);
    Result.m[3][1] = -(Top + Bottom) / (Top - Bottom);
    Result.m[3][2] = -(Far + Near) / (Far - Near);
    
    *OutMat = Result;
}

internal void
TranslateMat4(mat4 *OutMat, v3 Translation)
{
    v4 First = V4Scale(OutMat->e[0], Translation.x);
    v4 Second = V4Scale(OutMat->e[1], Translation.y);
    v4 Third = V4Scale(OutMat->e[2], Translation.z);
    
    v4 Final = V4Add(First, Second);
    Final = V4Add(Final, Third);
    Final = V4Add(Final, OutMat->e[3]);
    
    OutMat->e[3] = Final;
}

internal void
RotateMat4(mat4 *OutMat, f32 Angle, v3 RawAxis)
{
    f32 C = cosf(Angle);
    f32 S = sinf(Angle);
    
    v3 Axis = V3Normalize(RawAxis);
    v3 Temp = V3Scale(Axis, 1.0f - C);
    
    mat4 Rotate = {0};
    Rotate.m[0][0] = C + Temp.e[0]*Axis.e[0];
    Rotate.m[0][1] = Temp.e[0]*Axis.e[1] + S*Axis.e[2];
    Rotate.m[0][2] = Temp.e[0]*Axis.e[2] - S*Axis.e[1];
    
    Rotate.m[1][0] = Temp.e[1]*Axis.e[0] - S*Axis.e[2];
    Rotate.m[1][1] = C + Temp.e[1]*Axis.e[1];
    Rotate.m[1][2] = Temp.e[1]*Axis.e[2] + S*Axis.e[0];
    
    Rotate.m[2][0] = Temp.e[2]*Axis.e[0] + S*Axis.e[1];
    Rotate.m[2][1] = Temp.e[2]*Axis.e[1] - S*Axis.e[0];
    Rotate.m[2][2] = C + Temp.e[2]*Axis.e[2];
    
    v4 First, Second, Third;
    
    mat4 Result = {0};
    
    First = V4Scale(OutMat->e[0], Rotate.m[0][0]);
    Second = V4Scale(OutMat->e[1], Rotate.m[0][1]);
    Third = V4Scale(OutMat->e[2], Rotate.m[0][2]);
    Result.e[0] = V4Add(V4Add(First, Second), Third);
    
    First = V4Scale(OutMat->e[0], Rotate.m[1][0]);
    Second = V4Scale(OutMat->e[1], Rotate.m[1][1]);
    Third = V4Scale(OutMat->e[2], Rotate.m[1][2]);
    Result.e[1] = V4Add(V4Add(First, Second), Third);
    
    First = V4Scale(OutMat->e[0], Rotate.m[2][0]);
    Second = V4Scale(OutMat->e[1], Rotate.m[2][1]);
    Third = V4Scale(OutMat->e[2], Rotate.m[2][2]);
    Result.e[2] = V4Add(V4Add(First, Second), Third);
    
    Result.e[3] = OutMat->e[3];
    
    *OutMat = Result;
}

internal void
ScaleMat4(mat4 *OutMat, v3 Scale)
{
    OutMat->e[0] = V4Scale(OutMat->e[0], Scale.x);
    OutMat->e[1] = V4Scale(OutMat->e[1], Scale.y);
    OutMat->e[2] = V4Scale(OutMat->e[2], Scale.z);
}

internal void
CreateTransform(mat4 *OutMat, v2 Position, v2 Scale, f32 Rotation, mat4 *Projection)
{
    mat4 Result = Mat4Identity();
    
    v3 Position3 = {Position.x, Position.y, 0.0f};
    TranslateMat4(&Result, Position3);
    
    // NOTE(evan): We get weird results when rotating {0, 0, 0}
    v3 Rotation3 = {0.0f, 0.0f, Rotation*DEG_TO_RAD};
    RotateMat4(&Result, V3Length(Rotation3), V3Normalize(Rotation3));
    
    v3 Scale3 = {Scale.x, Scale.y, 1.0f};
    ScaleMat4(&Result, Scale3);
    
    *OutMat = Result;
}

#endif //_B_MATH_H
