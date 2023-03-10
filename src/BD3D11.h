/* date = March 3rd 2023 10:48 pm */

#ifndef _B_D3_D11_H
#define _B_D3_D11_H

#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>

typedef struct renderer_data
{
    ID3D11Device *Device;
    ID3D11DeviceContext *Context;
    
    IDXGISwapChain1 *SwapChain;
    
    ID3D11InputLayout *Layout;
    
    ID3D11VertexShader *VShader;
    ID3D11PixelShader *PShader;
    
    ID3D11Buffer *UBuffer;
    
    // TODO(evan): Should be by itself
    ID3D11ShaderResourceView *TextureView;
    
    ID3D11SamplerState *Sampler;
    
    ID3D11BlendState *Blender;
    
    ID3D11RasterizerState *RasterizerState;
    
    ID3D11DepthStencilState *DepthState;
    
    ID3D11RenderTargetView *RTView;
    ID3D11DepthStencilView *DSView;
    
    s32 CurrentWidth;
    s32 CurrentHeight;
    
    f32 Near;
    f32 Far;
} renderer_data;

typedef struct renderer_object
{
    ID3D11Buffer *VBuffer;
} renderer_object;

#endif //_B_D3_D11_H
