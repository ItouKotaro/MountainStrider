//=============================================================
//
// メッシュフィールド [meshfield.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "component.h"

// メッシュフィールド
class CMeshField : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Draw() override;

	// メッシュフィールドを用意する
	void Create(const int& x, const int& y, const float& spaceSize);

	// 指定の位置の高さを変更する
	void SetHeight(const int& x, const int& y, const float& height);

private:
	int m_sizeX;
	int m_sizeY;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファ
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャ
};

#endif // !_MESH_FIELD_H_
