//=============================================================
//
// レンダーテクスチャ [render_texture.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include "component.h"
#include "component/3d/camera.h"

// レンダーテクスチャクラス
class CRenderTexture : public Component
{
public:
	void Init() override;													// 初期化
	void Uninit() override;													// 終了
	void OnDestroyOtherComponent(Component* other) override;

	bool BeginRender();
	void EndRender();
	HRESULT Create(const int& nW, const int& nH);
	HRESULT SetTargetCamera(CCamera* pCamera);			// カメラが削除されたときの処理
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pRenderTexture; }
	CCamera* GetTargetCamera() { return m_pTargetCamera; }
private:
	CCamera* m_pTargetCamera;										// ターゲットのカメラ
	int m_nWidth, m_nHeight;											// テクスチャサイズ
	LPDIRECT3DTEXTURE9 m_pRenderTexture;					// レンダリング先のテクスチャ
	LPDIRECT3DSURFACE9 m_pTextureSurface;					// サーフェイス	
	LPDIRECT3DSURFACE9 m_pDepthSurface;					// 深度バッファ
};

#endif // !_RENDER_TEXTURE_H_
