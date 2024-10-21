//=============================================================
//
// ���[�e�B���e�B [util.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "util.h"

// ���b�V���̒��_�錾
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
// ���_�E�C���f�b�N�X��񂩂烁�b�V���ɕϊ�����
//=============================================================
LPD3DXMESH UTIL::ConvertToMesh(int numFace, int numVertices, int numIndices, VERTEX_3D* vertices, int* indices)
{
    // �f�o�C�X���擾����
    auto pDevice = CRenderer::GetInstance()->GetDevice();

	// ���b�V�����쐬����
    LPD3DXMESH pMesh;
    HRESULT hr = D3DXCreateMesh(
        numFace,        // �ʂ̐�
        numVertices,   // ���_�̐�
        FVF_VERTEX_3D,
        UTIL::MESHVERT::Decl,
        pDevice, // IDirect3DDevice9�C���^�[�t�F�[�X
        &pMesh
    );
    if (FAILED(hr))
    { // ���s
        return nullptr;
    }

    // ���_�f�[�^�̐ݒ�
    void* pVertices;
    if (SUCCEEDED(pMesh->LockVertexBuffer(0, (void**)&pVertices)))
    {
        memcpy(pVertices, vertices, sizeof(VERTEX_3D) * numVertices);
        pMesh->UnlockVertexBuffer();
    }

    // �C���f�b�N�X�f�[�^�̐ݒ�
    void* pIndices;
    if (SUCCEEDED(pMesh->LockIndexBuffer(0, (void**)&pIndices)))
    {
        memcpy(pIndices, indices, sizeof(int) * numIndices);
        pMesh->UnlockIndexBuffer();
    }

    // �쐬�������b�V����Ԃ�
    return pMesh;
}