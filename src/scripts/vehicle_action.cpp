//=============================================================
//
// バイクアクション [vehicle_action.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_action.h"

//=============================================================
// [VehicleAction] 初期化
//=============================================================
void VehicleAction::Init()
{
	m_isMeasure = false;
	m_rolling = 0.0f;
	m_rollCount = 0;
	m_beforeRot = 0.0f;
}

//=============================================================
// [VehicleAction] 更新
//=============================================================
void VehicleAction::Update()
{
	// 計測時の処理
	if (m_isMeasure)
	{
		// 現在の角度を取得する
		float currentRot = transform->GetRot().y;

		// 明らかに回転率がおかしいとき
		float rolling = fabsf(currentRot - m_beforeRot);
		m_beforeRot = currentRot;
		if (fabsf(rolling) > D3DX_PI)
		{
			return;
		}

		// 回転数を加算する
		m_rolling += rolling;

		if (m_rollCount < (m_rolling - fmod(m_rolling, D3DX_PI)) / static_cast<float>(D3DX_PI))
		{
			m_rollCount++;

			// 表示テキストを決める
			char pointText[24];
			if (m_rollCount % 2 == 0)
				sprintf(&pointText[0], "+%d回転", static_cast<int>(m_rollCount /2));
			else
				sprintf(&pointText[0], "+%.1f回転", m_rollCount * 0.5f);

			// 表示
			GameObject* addRollText = new GameObject();
			addRollText->transform->SetPos(1300.0f, 600.0f);
			addRollText->AddComponent<ActionPointText>();
			auto rollText = addRollText->GetComponent<ActionPointText>();
			rollText->SetText(pointText);
			rollText->SetFade(0.02f);
			rollText->SetCounter(120);
			rollText->SetUp(2.0f);
		}
	}
}

//=============================================================
// [VehicleAction] 飛び始め
//=============================================================
void VehicleAction::onFlyBegin()
{
	m_rolling = 0.0f;
	m_rollCount = 0;
	m_beforeRot = transform->GetRot().y;
	m_isMeasure = true;
}

//=============================================================
// [VehicleAction] 飛び終わり
//=============================================================
void VehicleAction::onFlyEnd()
{
	m_rolling = 0.0f;
	m_isMeasure = false;
}


//=============================================================
// [ActionPointText] 初期化
//=============================================================
void ActionPointText::Init()
{
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetFont("07鉄瓶ゴシック");
}

//=============================================================
// [ActionPointText] 終了
//=============================================================
void ActionPointText::Uninit()
{
	m_text->Uninit();
	delete m_text;
}

//=============================================================
// [ActionPointText] 更新
//=============================================================
void ActionPointText::Update()
{
	// 上昇とフェード
	m_text->transform->Translate(0.0f, -m_upValue, 0.0f);
	m_text->SetAlpha(m_text->GetAlpha() - m_fade);

	// カウンターをデクリメント
	m_counter--;
	if (m_counter <= 0)
	{ // 無くなったとき
		gameObject->Destroy();
	}

	m_text->Update();
}

//=============================================================
// [ActionPointText] 描画
//=============================================================
void ActionPointText::DrawUI()
{
	m_text->DrawUI();
}



//=============================================================
// [SumActionPointText] 初期化
//=============================================================
void SumActionPointText::Init()
{
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetFont("07鉄瓶ゴシック");
}

//=============================================================
// [SumActionPointText] 終了
//=============================================================
void SumActionPointText::Uninit()
{
	m_text->Uninit();
	delete m_text;
}

//=============================================================
// [SumActionPointText] 更新
//=============================================================
void SumActionPointText::Update()
{
	m_text->Update();
}

//=============================================================
// [SumActionPointText] 描画
//=============================================================
void SumActionPointText::DrawUI()
{
	m_text->DrawUI();
}