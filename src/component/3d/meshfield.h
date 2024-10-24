//=============================================================
//
// ���b�V���t�B�[���h [meshfield.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "component.h"

// ���b�V���t�B�[���h
class CMeshField : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Draw() override;

	// ���b�V���t�B�[���h��p�ӂ���
	void Create(const int& x, const int& y, const float& spaceSize);

	// �w��̈ʒu�̍�����ύX����
	void SetHeight(const int& x, const int& y, const float& height);

private:
	int m_sizeX;
	int m_sizeY;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`��
};

#endif // !_MESH_FIELD_H_
