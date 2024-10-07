//=============================================================
//
// �I�u�W�F�N�g�̃e�N�X�`�������_�[ [render_objects_texture.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RENDER_OBJECTS_TEXTURE_H_
#define _RENDER_OBJECTS_TEXTURE_H_

#include "component.h"

class CRenderObjectsTexture : public Component
{
public:
	void Init() override;
	void Uninit() override;

	HRESULT Create(const int& nW, const int& nH);
	void AddObject(GameObject* pAddObj);
	void RemoveObject(GameObject* pRemoveObj);
	void DrawTexture();

	LPDIRECT3DTEXTURE9 GetTexture() { return m_pRenderTexture; }
private:
	LPDIRECT3DTEXTURE9 m_pRenderTexture;		// �����_�����O��̃e�N�X�`��
	LPDIRECT3DSURFACE9 m_pTextureSurface;		// �T�[�t�F�C�X	
	std::vector<GameObject*> m_pRenderList;
};

#endif // !_RENDER_OBJECTS_TEXTURE_H_
