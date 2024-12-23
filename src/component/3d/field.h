//=============================================================
//
// フィールドコンポーネント [field.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "component.h"

class CField : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Set(float fWidth, float fDepth);
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_pTexture = texture; }
	void SetTexture(const std::string& sPath);
	void SetColor(D3DXCOLOR col);
	void SetLoopTexture(const int& num);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャ
	D3DXCOLOR m_color;									// 色
};

#endif // !_FIELD_H_
