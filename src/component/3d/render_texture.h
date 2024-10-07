//=============================================================
//
// �����_�[�e�N�X�`�� [render_texture.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include "component.h"
#include "component/3d/camera.h"

// �����_�[�e�N�X�`���N���X
class CRenderTexture : public Component
{
public:
	void Init() override;													// ������
	void Uninit() override;													// �I��
	void OnDestroyOtherComponent(Component* other) override;

	bool BeginRender();
	void EndRender();
	HRESULT Create(const int& nW, const int& nH);
	HRESULT SetTargetCamera(CCamera* pCamera);			// �J�������폜���ꂽ�Ƃ��̏���
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pRenderTexture; }
	CCamera* GetTargetCamera() { return m_pTargetCamera; }
private:
	CCamera* m_pTargetCamera;										// �^�[�Q�b�g�̃J����
	int m_nWidth, m_nHeight;											// �e�N�X�`���T�C�Y
	LPDIRECT3DTEXTURE9 m_pRenderTexture;					// �����_�����O��̃e�N�X�`��
	LPDIRECT3DSURFACE9 m_pTextureSurface;					// �T�[�t�F�C�X	
	LPDIRECT3DSURFACE9 m_pDepthSurface;					// �[�x�o�b�t�@
};

#endif // !_RENDER_TEXTURE_H_
