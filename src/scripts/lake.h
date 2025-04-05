//=============================================================
//
// 湖 [lake.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _LAKE_H_
#define _LAKE_H_

#include "component.h"
#include "terrain.h"
#include "vehicle.h"

// 湖
class LakeField : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetSize(const float& x, const float& y);
	void SetTexture(const std::string& texturePath, const int& loopNum = 1, const D3DXVECTOR2& move = { 0.0f, 0.0f });
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_pTexture = texture; }
	void SetColor(const D3DXCOLOR& color);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャ
	D3DXCOLOR m_color;									// 色
	int m_loopNum;												// ループ回数
	D3DXVECTOR2 m_move;								// 移動値
	D3DXVECTOR2 m_texMove;							// テクスチャの移動情報
};

// 湖マネージャー
class LakeManager
{
public:
	LakeManager() : m_enabled(false), m_height(0.0f), m_fuelDamage(0.0f), m_enduranceDamage(0.0f), m_vehicle(nullptr), m_baseColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)){}
	void Init(Terrain* terrain, const std::string& path);
	void Uninit();
	void Update();
	bool IsEnabled() { return m_enabled; }
	D3DXCOLOR GetBaseColor() { return m_baseColor; }
	float GetHeight() { return m_height; }
private:
	Terrain* m_terrain;
	bool m_enabled;
	float m_height;
	float m_fuelDamage;
	float m_enduranceDamage;
	bool m_isUnderWater;
	D3DXCOLOR m_baseColor;
	CVehicle* m_vehicle;
	GameObject* m_audioPlayer;
	AudioClip m_underwaterSE;
	AudioClip m_diveWaterSE;

	const float LAKE_INDEPTH = 3.0f;
	const float LAKE_RESISTANCE = 0.99f;
	const float LAKE_FLOATING_POWER = 13000.0f;
};

#endif // !_LAKE_H_
