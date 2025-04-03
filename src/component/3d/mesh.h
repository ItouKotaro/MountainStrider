//=============================================================
//
// ���b�V���R���|�[�l���g [mesh.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MESH_H_
#define _MESH_H_

#include <component.h>
#include "main.h"
#include "component/3d/camera.h"

class CDataMesh;

// ���b�V��
class CMesh : public Component
{
public:
	CMesh();
	CMesh(std::string sPath, bool bShadow = true);
	void Init();
	void Uninit();
	void Draw();
	HRESULT LoadMeshX(std::string sPath, bool bShadow = true);
	HRESULT SetMesh(LPD3DXMESH pMesh);
	ID3DXMesh* GetShadowMesh() { return m_pShadowMesh; }
	LPD3DXMESH GetMesh() { return m_pMesh; }
	LPD3DXBUFFER GetBuffMat() { return m_pBuffMat; }
	DWORD GetNumMat() { return m_dwNumMat; }
	void SetTexture(int nIdx, LPDIRECT3DTEXTURE9 pTexture);
	std::vector<LPDIRECT3DTEXTURE9>* GetTextures() { return &m_apTexture; }
	void SetAlpha(const float& fAlpha);
	float* GetAlpha() { return m_fAlpha; }
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }
	void SetCustomColor(const bool& enabled) { m_customColor = enabled; }

	static void SetCamera(CCamera* camera) { m_camera = camera; }

	static const float CAMERA_ALPHA_DISTANCE;
private:
	bool m_customColor;
	D3DXCOLOR m_color;
	float* m_fAlpha;																	// �s�����x
	LPD3DXMESH m_pMesh;													// ���b�V���ւ̃|�C���^
	ID3DXMesh* m_pShadowMesh;											// �e���b�V��
	LPD3DXBUFFER m_pBuffMat;												// �}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;														// �}�e���A���̐�
	CDataMesh* m_pData;														// �f�[�^
	std::vector<LPDIRECT3DTEXTURE9> m_apTexture;			// �e�N�X�`��

	static CCamera* m_camera;
};

#endif // !_MESH_H_
