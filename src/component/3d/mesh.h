//=============================================================
//
// メッシュコンポーネント [mesh.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MESH_H_
#define _MESH_H_

#include <component.h>
#include "main.h"

// メッシュ
class CMesh : public Component
{
public:
	CMesh();
	CMesh(std::string sPath, bool bShadow = true);
	void Init();
	void Uninit();
	void Draw();
	HRESULT LoadMeshX(std::string sPath, bool bShadow = true);
	ID3DXMesh* GetShadowMesh() { return m_pShadowMesh; }
	LPD3DXMESH GetMesh() { return m_pMesh; }
	LPD3DXBUFFER GetBuffMat() { return m_pBuffMat; }
	DWORD GetNumMat() { return m_dwNumMat; }
	void SetTexture(int nIdx, LPDIRECT3DTEXTURE9 pTexture);
	std::vector<LPDIRECT3DTEXTURE9>* GetTextures() { return &m_apTexture; }
	void SetAlpha(const float& fAlpha);
	float* GetAlpha() { return m_fAlpha; }
private:
	float* m_fAlpha;																// 不透明度
	LPD3DXMESH m_pMesh;													// メッシュへのポインタ
	ID3DXMesh* m_pShadowMesh;											// 影メッシュ
	LPD3DXBUFFER m_pBuffMat;												// マテリアルへのポインタ
	DWORD m_dwNumMat;														// マテリアルの数
	std::vector<LPDIRECT3DTEXTURE9> m_apTexture;				// テクスチャ
};

#endif // !_MESH_H_
