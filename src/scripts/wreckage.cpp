//=============================================================
//
// 残骸 [wreckage.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "wreckage.h"

// 静的メンバ変数の初期化
const float CWreckage::FADE_VALUE = 0.03f;
const int CWreckage::START_TIME = 500;

//=============================================================
// [CWreckage] 初期化
//=============================================================
void CWreckage::Init()
{
	// メッシュリストを取得する
	std::vector<GameObject*> pMeshList = gameObject->GetChildren(true);
	for (unsigned int i = 0; i < pMeshList.size(); i++)
	{
		if (pMeshList[i]->GetComponent<CMesh>() != nullptr)
		{
			m_pMeshs.push_back(pMeshList[i]->GetComponent<CMesh>());
		}
	}

	// フェード値
	m_fFade = 1.0f;

	// 開始時間の設定
	m_dwStartTime = timeGetTime() + START_TIME;
}

//=============================================================
// [CWreckage] 更新
//=============================================================
void CWreckage::Update()
{
	// 開始時間
	if (timeGetTime() < m_dwStartTime)
	{
		return;
	}

	// フェード処理
	if (m_fFade > 0.0f)
	{
		m_fFade -= FADE_VALUE;
	}
	else
	{ // 終了
		gameObject->Destroy(true);
		return;
	}

	// モデルの不透明度設定
	for (unsigned int i = 0; i < m_pMeshs.size(); i++)
	{
		m_pMeshs[i]->SetAlpha(m_fFade);
	}
}