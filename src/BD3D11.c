#include "BD3D11.h"

global renderer_data RendererData;

struct vertex
{
    f32 Position[2];
    f32 Uv[2];
    f32 Color[3];
};

internal void
InitializeRenderer(f32 Near, f32 Far)
{
    RendererData.Near = Near;
    RendererData.Far = Far;
    
    HRESULT Result;
    
    {
        UINT Flags = 0;
        
#if BLIB_DEBUG
        Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        
        D3D_FEATURE_LEVEL Levels[] = {D3D_FEATURE_LEVEL_11_0};
        Result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, Flags, Levels, ArrayCount(Levels),
                                   D3D11_SDK_VERSION, &RendererData.Device, 0, &RendererData.Context);
        AssertHR(Result);
    }
    
#if BLIB_DEBUG
    {
        ID3D11InfoQueue *Info;
        ID3D11Device_QueryInterface(RendererData.Device, &IID_ID3D11InfoQueue, &Info);
        ID3D11InfoQueue_SetBreakOnSeverity(Info, D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
        ID3D11InfoQueue_SetBreakOnSeverity(Info, D3D11_MESSAGE_SEVERITY_ERROR, true);
        ID3D11InfoQueue_Release(Info);
    }
#endif
    
    {
        IDXGIDevice *DXGIDevice;
        Result = ID3D11Device_QueryInterface(RendererData.Device, &IID_IDXGIDevice, &DXGIDevice);
        AssertHR(Result);
        
        IDXGIAdapter *DXGIAdapter;
        Result = IDXGIDevice_GetAdapter(DXGIDevice, &DXGIAdapter);
        AssertHR(Result);
        
        IDXGIFactory2 *Factory;
        Result = IDXGIAdapter_GetParent(DXGIAdapter, &IID_IDXGIFactory2, &Factory);
        AssertHR(Result);
        
        DXGI_SWAP_CHAIN_DESC1 Desc = {0};
        Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Desc.SampleDesc.Count = 1;
        Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        Desc.BufferCount = 2;
        Desc.Scaling = DXGI_SCALING_NONE;
        Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        
        Result = IDXGIFactory2_CreateSwapChainForHwnd(Factory, (IUnknown *)RendererData.Device, PlatformData.Window,
                                                      &Desc, 0, 0, &RendererData.SwapChain);
        AssertHR(Result);
        
        IDXGIFactory_MakeWindowAssociation(Factory, PlatformData.Window, DXGI_MWA_NO_ALT_ENTER);
        
        IDXGIFactory2_Release(Factory);
        IDXGIAdapter_Release(DXGIAdapter);
        IDXGIDevice_Release(DXGIDevice);
    }
    
    {
        D3D11_INPUT_ELEMENT_DESC Desc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(struct vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(struct vertex, Uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(struct vertex, Color), D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        
        const char HLSL[] =
            "#line " STR(__LINE__) "\n"
            "struct vs_input \n"
            "{\n"
            "float2 Position : POSITION;\n"
            "float2 Uv       : TEXCOORD;\n"
            "float3 Color    : COLOR;\n"
            "};\n"
            "struct ps_input \n"
            "{\n"
            "float4 Position : SV_POSITION;\n"
            "float2 Uv       : TEXCOORD;\n"
            "float4 Color    : COLOR;\n"
            "};\n"
            "cbuffer cbuffer0 : register(b0)\n"
            "{\n"
            "row_major float4x4 Transform;\n"
            "}\n"
            "sampler sampler0 : register(s0);\n"
            "Texture2D<float4> texture0 : register(t0);\n"
            "ps_input Vertex(vs_input Input)\n"
            "{\n"
            "ps_input Output;\n"
            "Output.Position = mul(float4(Input.Position, 0, 1), Transform);\n"
            "Output.Position.z = 0;\n"
            "Output.Uv = Input.Uv;\n"
            "Output.Color = float4(Input.Color, 1);\n"
            "return Output;\n"
            "}\n"
            "float4 Pixel(ps_input Input) : SV_TARGET \n"
            "{\n"
            "float4 Texture = texture0.Sample(sampler0, Input.Uv);\n"
            "return Input.Color*Texture;\n"
            "}\n";
        
        UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR|D3DCOMPILE_ENABLE_STRICTNESS|D3DCOMPILE_WARNINGS_ARE_ERRORS;
#if BLIB_DEBUG
        Flags |= D3DCOMPILE_DEBUG|D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        Flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
        
        ID3DBlob *Error;
        
        ID3DBlob *VBlob;
        Result = D3DCompile(HLSL, sizeof(HLSL), 0, 0, 0, "Vertex", "vs_5_0", Flags, 0, &VBlob, &Error);
        if(FAILED(Result))
        {
            const char *Message = ID3D10Blob_GetBufferPointer(Error);
            OutputDebugStringA(Message);
            Assert(!"Failed to compile vertex shader!");
        }
        
        ID3DBlob *PBlob;
        Result = D3DCompile(HLSL, sizeof(HLSL), 0, 0, 0, "Pixel", "ps_5_0", Flags, 0, &PBlob, &Error);
        if(FAILED(Result))
        {
            const char *Message = ID3D10Blob_GetBufferPointer(Error);
            OutputDebugStringA(Message);
            Assert(!"Failed to compile pixel shader");
        }
        
        ID3D11Device_CreateVertexShader(RendererData.Device,
                                        ID3D10Blob_GetBufferPointer(VBlob), ID3D10Blob_GetBufferSize(VBlob),
                                        0, &RendererData.VShader);
        ID3D11Device_CreatePixelShader(RendererData.Device,
                                       ID3D10Blob_GetBufferPointer(PBlob), ID3D10Blob_GetBufferSize(PBlob),
                                       0, &RendererData.PShader);
        ID3D11Device_CreateInputLayout(RendererData.Device, Desc, ArrayCount(Desc),
                                       ID3D10Blob_GetBufferPointer(VBlob), ID3D10Blob_GetBufferSize(VBlob),
                                       &RendererData.Layout);
        
        ID3D10Blob_Release(PBlob);
        ID3D10Blob_Release(VBlob);
    }
    
    {
        D3D11_BUFFER_DESC Desc = {0};
        Desc.ByteWidth = 4*4*sizeof(f32);
        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        
        ID3D11Device_CreateBuffer(RendererData.Device, &Desc, 0, &RendererData.UBuffer);
    }
    
    {
        u32 Pixels[] =
        {
            0xFFFFFFFF
        };
        UINT Width = 1;
        UINT Height = 1;
        
        D3D11_TEXTURE2D_DESC Desc = {0};
        Desc.Width = Width;
        Desc.Height = Height;
        Desc.MipLevels = 1;
        Desc.ArraySize = 1;
        Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Desc.SampleDesc.Count = 1;
        Desc.Usage = D3D11_USAGE_IMMUTABLE;
        Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        
        D3D11_SUBRESOURCE_DATA Data = {0};
        Data.pSysMem = Pixels;
        Data.SysMemPitch = Width*sizeof(u32);
        
        ID3D11Texture2D *Texture;
        ID3D11Device_CreateTexture2D(RendererData.Device, &Desc, &Data, &Texture);
        ID3D11Device_CreateShaderResourceView(RendererData.Device, (ID3D11Resource *)Texture, 0, &RendererData.TextureView);
        ID3D11Texture2D_Release(Texture);
    }
    
    {
        D3D11_SAMPLER_DESC Desc = {0};
        Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        
        ID3D11Device_CreateSamplerState(RendererData.Device, &Desc, &RendererData.Sampler);
    }
    
    {
        D3D11_BLEND_DESC Desc =
        {
            .RenderTarget[0] =
            {
                .BlendEnable = true,
                .SrcBlend = D3D11_BLEND_SRC_ALPHA,
                .DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
                .BlendOp = D3D11_BLEND_OP_ADD,
                .SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA,
                .DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA,
                .BlendOpAlpha = D3D11_BLEND_OP_ADD,
                .RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
            }
        };
        
        ID3D11Device_CreateBlendState(RendererData.Device, &Desc, &RendererData.Blender);
    }
    
    {
        D3D11_RASTERIZER_DESC Desc = {0};
        Desc.FillMode = D3D11_FILL_SOLID;
        Desc.CullMode = D3D11_CULL_NONE;
        
        ID3D11Device_CreateRasterizerState(RendererData.Device, &Desc, &RendererData.RasterizerState);
    }
    
    {
        D3D11_DEPTH_STENCIL_DESC Desc = {0};
        Desc.DepthEnable = false;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        Desc.DepthFunc = D3D11_COMPARISON_LESS;
        Desc.StencilEnable = false;
        Desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        Desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        
        ID3D11Device_CreateDepthStencilState(RendererData.Device, &Desc, &RendererData.DepthState);
    }
    
    RendererData.RTView = 0;
    RendererData.DSView = 0;
}

internal renderer_object
CreateRendererObject(struct vertex *Data, s32 DataSize)
{
    renderer_object Result;
    
    D3D11_BUFFER_DESC Desc = {0};
    Desc.ByteWidth = DataSize;
    Desc.Usage = D3D11_USAGE_IMMUTABLE;
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA Initial = {0};
    Initial.pSysMem = Data;
    
    ID3D11Device_CreateBuffer(RendererData.Device, &Desc, &Initial, &Result.VBuffer);
    
    return(Result);
}

internal void
BeginDrawing(void)
{
    HRESULT Result;
    
    if(RendererData.RTView == 0 ||
       PlatformData.Width != RendererData.CurrentWidth ||
       PlatformData.Height != RendererData.CurrentHeight)
    {
        if(RendererData.RTView)
        {
            ID3D11DeviceContext_ClearState(RendererData.Context);
            ID3D11RenderTargetView_Release(RendererData.RTView);
            ID3D11DepthStencilView_Release(RendererData.DSView);
            RendererData.RTView = 0;
            RendererData.DSView = 0;
        }
        
        if(PlatformData.Width != 0 && PlatformData.Height != 0)
        {
            Result = IDXGISwapChain1_ResizeBuffers(RendererData.SwapChain, 0,
                                                   PlatformData.Width, PlatformData.Height, DXGI_FORMAT_UNKNOWN, 0);
            if(FAILED(Result))
            {
                // TODO(evan): Log this instead of asserting
                Assert(!"Failed to resize swap chain!");
            }
            
            ID3D11Texture2D *BackBuffer;
            IDXGISwapChain1_GetBuffer(RendererData.SwapChain, 0, &IID_ID3D11Texture2D, &BackBuffer);
            ID3D11Device_CreateRenderTargetView(RendererData.Device, (ID3D11Resource *)BackBuffer, 0, &RendererData.RTView);
            ID3D11Texture2D_Release(BackBuffer);
            
            D3D11_TEXTURE2D_DESC DepthDesc = {0};
            DepthDesc.Width = PlatformData.Width;
            DepthDesc.Height = PlatformData.Height;
            DepthDesc.MipLevels = 1;
            DepthDesc.ArraySize = 1;
            DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
            DepthDesc.SampleDesc.Count = 1;
            DepthDesc.Usage = D3D11_USAGE_DEFAULT;
            DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            
            ID3D11Texture2D *Depth;
            ID3D11Device_CreateTexture2D(RendererData.Device, &DepthDesc, 0, &Depth);
            ID3D11Device_CreateDepthStencilView(RendererData.Device, (ID3D11Resource *)Depth, 0, &RendererData.DSView);
            ID3D11Texture2D_Release(Depth);
        }
        
        RendererData.CurrentWidth = PlatformData.Width;
        RendererData.CurrentHeight = PlatformData.Height;
    }
    
    if(RendererData.RTView)
    {
        D3D11_VIEWPORT Viewport = {0};
        Viewport.Width = (f32)PlatformData.Width;
        Viewport.Height = (f32)PlatformData.Height;
        Viewport.MaxDepth = 1;
        
        f32 Color[] = {0.392f, 0.584f, 0.929f, 1.0f};
        ID3D11DeviceContext_ClearRenderTargetView(RendererData.Context, RendererData.RTView, Color);
        ID3D11DeviceContext_ClearDepthStencilView(RendererData.Context, RendererData.DSView,
                                                  D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
        
        ID3D11DeviceContext_IASetInputLayout(RendererData.Context, RendererData.Layout);
        ID3D11DeviceContext_IASetPrimitiveTopology(RendererData.Context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
        ID3D11DeviceContext_VSSetConstantBuffers(RendererData.Context, 0, 1, &RendererData.UBuffer);
        ID3D11DeviceContext_VSSetShader(RendererData.Context, RendererData.VShader, 0, 0);
        
        ID3D11DeviceContext_RSSetViewports(RendererData.Context, 1, &Viewport);
        ID3D11DeviceContext_RSSetState(RendererData.Context, RendererData.RasterizerState);
        
        ID3D11DeviceContext_PSSetSamplers(RendererData.Context, 0, 1, &RendererData.Sampler);
        
        // TODO(evan): Object specific
        ID3D11DeviceContext_PSSetShaderResources(RendererData.Context, 0, 1, &RendererData.TextureView);
        
        ID3D11DeviceContext_PSSetShader(RendererData.Context, RendererData.PShader, 0, 0);
        
        ID3D11DeviceContext_OMSetBlendState(RendererData.Context, RendererData.Blender, 0, ~0U);
        ID3D11DeviceContext_OMSetDepthStencilState(RendererData.Context, RendererData.DepthState, 0);
        ID3D11DeviceContext_OMSetRenderTargets(RendererData.Context, 1, &RendererData.RTView, RendererData.DSView);
    }
}

internal void
DrawObject(renderer_object Object, v2 Position, v2 Scale, f32 Rotation)
{
    u32 Stride = sizeof(struct vertex);
    u32 Offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(RendererData.Context, 0, 1, &Object.VBuffer, &Stride, &Offset);
    
    {
        mat4 Projection;
        CreateOrthographic(&Projection,
                           0.0f, (f32)PlatformData.Width,
                           0.0f, (f32)PlatformData.Height,
                           RendererData.Near, RendererData.Far);
        
        mat4 Transform = Mat4Identity();
        CreateTransform(&Transform, Position, Scale, Rotation, &Projection);
        
        mat4 M;
        Mat4Multiply(&M, Transform, Projection);
        
        D3D11_MAPPED_SUBRESOURCE Mapped;
        ID3D11DeviceContext_Map(RendererData.Context, (ID3D11Resource *)RendererData.UBuffer,
                                0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
        memcpy(Mapped.pData, &M, sizeof(M));
        ID3D11DeviceContext_Unmap(RendererData.Context, (ID3D11Resource *)RendererData.UBuffer, 0);
    }
    
    ID3D11DeviceContext_Draw(RendererData.Context, 3, 0);
}

internal void
EndDrawing(void)
{
    b32 VSync = true;
    HRESULT Result = IDXGISwapChain1_Present(RendererData.SwapChain, VSync, 0);
    if(Result == DXGI_STATUS_OCCLUDED)
    {
        // NOTE(evan): Window is minimized, cannot vsync - instead sleep a little
        if(VSync)
        {
            Sleep(10);
        }
    }
    else if(FAILED(Result))
    {
        // TODO(evan); Log this instead of asserting
        Assert(!"Failed to present swap chain. Device lost?");
    }
}