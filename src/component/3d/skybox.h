//=============================================================
//
// スカイボックス [skybox.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include "component.h"

// スカイボックス
class CSkybox
{
public:
	// 位置
	enum POS
	{
		UP,			// 上
		DOWN,		// 下
		FRONT,		// 正面
		BACK,		// 背後
		LEFT,		// 左
		RIGHT		// 右
	};

	// テクスチャタイプ
	enum TEXTURETYPE
	{
		SINGLE,		// 1枚
		SPLIT		// 分割（6枚）
	};

	CSkybox(Transform* transform) { m_pCameraTransform = transform; }
	void Init();
	void Uninit();
	void Draw();
	void SetTextureType(TEXTURETYPE textureType);
	void SetTexture(const int& idx, const std::string& path);
	void LoadSkybox(const std::string& sPath);

	static const float SKYBOX_SIZE;
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[6];		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture[6];				// テクスチャ
	D3DXCOLOR m_color;										// 色
	TEXTURETYPE m_textureType;							// テクスチャタイプ
	Transform* m_pCameraTransform;						// カメラのトランスフォーム値
};

#endif // !_SKY_BOX_H_
