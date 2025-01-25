//=============================================================
//
// ライトコンポーネント [light.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "light.h"
#include "manager.h"

//=============================================================
// [CD3DLight] 初期化
//=============================================================
void CD3DLight::Init()
{

}

//=============================================================
// [CD3DLight] 終了
//=============================================================
void CD3DLight::Uninit()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 有効なライトを取得
	std::vector<CD3DLight*> lightList = GetComponents<CD3DLight>(false, true);

	// 最後尾のライトを無効にする
	if (lightList.size() > 0)
	{
		pDevice->LightEnable(static_cast<DWORD>(lightList.size() - 1), FALSE);
	}
}

//=============================================================
// [CD3DLight] 更新
//=============================================================
void CD3DLight::Update()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 有効なライトを取得
	std::vector<CD3DLight*> lightList = GetComponents<CD3DLight>(false, true);

	for (int i = 0; i < (int)lightList.size(); i++)
	{
		// ライトを設定する
		pDevice->SetLight(i, &lightList[i]->m_Light);

		// ライトを有効にする
		pDevice->LightEnable(i, TRUE);
	}
}

//=============================================================
// [CD3DLight] デフォルトのライト設定
//=============================================================
void CD3DLight::SetDefaultD3DLight(GameObject* gameObject)
{
	CD3DLight* pLightComp = gameObject->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.22f, -0.87f, 0.44f));

	pLightComp = gameObject->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(-0.18f, 0.88f, -0.44f));

	pLightComp = gameObject->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.89f, 0.11f, 0.44f));
}


//=============================================================
// [CLight] コンストラクタ
//=============================================================
CLight::CLight(float fIntensity, D3DXCOLOR color, D3DXCOLOR volumeColor)
{
	m_col = color;
	m_volumeCol = volumeColor;
	m_fIntensity = fIntensity;
}
