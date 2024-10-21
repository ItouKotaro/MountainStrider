//=============================================================
//
// ユーティリティ [util.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "util.h"

// メッシュの頂点宣言
namespace UTIL
{
    struct MESHVERT
    {
        D3DXVECTOR3 Position;
        D3DXVECTOR3 Normal;
        D3DXVECTOR2 Tex;
        const static D3DVERTEXELEMENT9 Decl[4];
    };
    const D3DVERTEXELEMENT9 MESHVERT::Decl[4] =
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
}

//=============================================================
// 頂点・インデックス情報からメッシュに変換する
//=============================================================
LPD3DXMESH UTIL::ConvertToMesh(int numFace, int numVertices, int numIndices, VERTEX_3D* vertices, int* indices)
{
    // デバイスを取得する
    auto pDevice = CRenderer::GetInstance()->GetDevice();

	// メッシュを作成する
    LPD3DXMESH pMesh;
    HRESULT hr = D3DXCreateMesh(
        numFace,        // 面の数
        numVertices,   // 頂点の数
        FVF_VERTEX_3D,
        UTIL::MESHVERT::Decl,
        pDevice, // IDirect3DDevice9インターフェース
        &pMesh
    );
    if (FAILED(hr))
    { // 失敗
        return nullptr;
    }

    // 頂点データの設定
    void* pVertices;
    if (SUCCEEDED(pMesh->LockVertexBuffer(0, (void**)&pVertices)))
    {
        memcpy(pVertices, vertices, sizeof(VERTEX_3D) * numVertices);
        pMesh->UnlockVertexBuffer();
    }

    // インデックスデータの設定
    void* pIndices;
    if (SUCCEEDED(pMesh->LockIndexBuffer(0, (void**)&pIndices)))
    {
        memcpy(pIndices, indices, sizeof(int) * numIndices);
        pMesh->UnlockIndexBuffer();
    }

    // 作成したメッシュを返す
    return pMesh;
}