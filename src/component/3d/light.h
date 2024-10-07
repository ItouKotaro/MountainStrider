//=============================================================
//
// ライトコンポーネント [light.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <component.h>

// D3Dライト（DirectXのライト）
class CD3DLight : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	// ライトの設定
	void SetLight(const D3DLIGHT9 light) {
		m_Light = light;
	}
	void SetType(const D3DLIGHTTYPE type) {
		m_Light.Type = type;
	}
	void SetDiffuse(D3DXCOLOR col) {
		m_Light.Diffuse = col;
	}
	void SetDirection(D3DXVECTOR3 dir) {
		m_Light.Direction = dir;
	}

	static void SetDefaultD3DLight(GameObject* gameObject);
private:
	D3DLIGHT9 m_Light;
};


// ライト
class CLight : public Component
{
public:
	CLight(float fIntensity = 10.0f, D3DXCOLOR color = { 1.0f, 1.0f, 1.0f, 1.0f }, D3DXCOLOR volumeColor = { 0.0f, 1.0f, 0.0f, 1.0f });

	// ライトの設定
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetVolumeColor(D3DXCOLOR col) { m_volumeCol = col; }
	void SetIntensity(const float& fIntensity) { if (fIntensity > 0.0f) m_fIntensity = fIntensity; }
	D3DXCOLOR GetColor() { return m_col; }
	D3DXCOLOR GetVolumeColor() { return m_volumeCol; }
	float GetIntensity() { return m_fIntensity; }
private:
	float m_fIntensity;						// 強さ
	D3DXCOLOR m_col;				// 色
	D3DXCOLOR m_volumeCol;		// シャドウボリュームの色
};

#endif // !_LIGHT_H_
