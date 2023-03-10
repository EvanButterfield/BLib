#include "BLib.h"

int main()
{
    InitializePlatform(800, 450, L"Testing Grounds");
    InitializeRenderer(0.01f, 100.0f);
    
    struct vertex Data[] =
    {
        { { -0.00f, +0.75f }, { 25.0f, 50.0f }, { 1, 0, 0 } },
        { { +0.75f, -0.50f }, {  0.0f,  0.0f }, { 0, 1, 0 } },
        { { -0.75f, -0.50f }, { 50.0f,  0.0f }, { 0, 0, 1 } }
    };
    
    renderer_object Object = CreateRendererObject(Data, sizeof(Data));
    
    f32 X = 0.0f;
    f32 Angle = 0.0f;
    
    while(!ShouldClose)
    {
        f32 DeltaTime = UpdatePlatform();
        
        X += DeltaTime*100.0f;
        Angle += 10.0f*DeltaTime;
        
        BeginDrawing();
        {
            v2 Position = {X, 100.0f};
            v2 Scale = {150.0f, 100.0f};
            DrawObject(Object, Position, Scale, Angle*2.0f);
            
            v2 Position2 = {100.0f, 100.0f};
            v2 Scale2 = {50.0f, 50.0f};
            DrawObject(Object, Position2, Scale2, Angle);
        }
        EndDrawing();
    }
    
    return(0);
}