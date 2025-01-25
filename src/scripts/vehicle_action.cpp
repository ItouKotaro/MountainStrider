//=============================================================
//
// バイクアクション [vehicle_action.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_action.h"
#include "scripts/vehicle.h"
#include "scene/game.h"
#include "component/other/audio.h"

AudioClip actionSE;

//=============================================================
// [VehicleAction] 初期化
//=============================================================
void VehicleAction::Init()
{
	if (!actionSE) actionSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\BIKE\\action00.mp3", FMOD_2D);
	m_actionSound = new GameObject();
	m_actionSound->AddComponent<AudioSource>();

	m_isMeasure = false;
	m_rolling = 0.0f;
	m_rollCount = 0;
	m_beforeRot = 0.0f;

	// 合計ポイント用テキスト
	GameObject* sumObj = new GameObject("SumPoints", "UI");
	m_sumPoints = sumObj->AddComponent<SumActionPointText>();
	m_sumPoints->transform->SetPos(CRenderer::SCREEN_WIDTH-50.0f, 300.0f);
	m_sumPoints->Reset();
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
		float rolling = currentRot - m_beforeRot;
		m_beforeRot = currentRot;
		if (fabsf(rolling) > D3DX_PI)
		{
			return;
		}

		// 回転数を加算する
		m_rolling += rolling;

		if (m_rollCount < (fabsf(m_rolling) - fmod(fabsf(m_rolling), D3DX_PI)) / static_cast<float>(D3DX_PI))
		{
			m_rollCount++;

			// バイクを取得する
			auto pBike = gameObject->GetComponent<CVehicle>();

			// スコアを計算する
			int addScore = 0;
			addScore += static_cast<int>((m_rollCount * 20) * (static_cast<int>(pBike->GetSpeed()) / static_cast<float>(50.0f)));	// 基礎スコア

			// 合計値に加算する
			m_sumPoints->AddPoints(addScore);

			// 表示テキストを決める
			char pointText[24];
			if (m_rollCount % 2 == 0)
				sprintf(&pointText[0], "+%d回転", static_cast<int>(m_rollCount /2));
			else
				sprintf(&pointText[0], "+%.1f回転", m_rollCount * 0.5f);
			
			// 表示
			GameObject* addRollText = new GameObject();
			addRollText->transform->SetPos(1300.0f, 600.0f);
			auto rollText = addRollText->AddComponent<ActionPointText>();
			rollText->SetText(pointText);
			rollText->SetFade(0.02f);
			rollText->SetCounter(120);
			rollText->SetUp(2.0f);

			// 音を鳴らす
			m_actionSound->GetComponent<AudioSource>()->Play(actionSE);
			m_actionSound->GetComponent<AudioSource>()->GetChannel()->setPitch(0.5f + m_rollCount * 0.2f);
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
	// バイクのブースト
	float rollNorm = fmod(fabsf(m_rolling), D3DX_PI * 2.0f);
	if (m_rollCount > 0 && ((D3DX_PI * 1.6f <= rollNorm && rollNorm <= D3DX_PI * 2.0f) || rollNorm <= D3DX_PI * 0.4f))
	{
		btVector3 velocity = CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity();
		velocity.setY(0.0f);
		CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(velocity);
		CCollision::GetCollision(gameObject)->GetRigidBody()->applyCentralImpulse(
			btVector3(sinf(transform->GetWRot().y) * -15000.0f * (m_rollCount / 2), 0.0f, cosf(transform->GetWRot().y) * -15000.0f * (m_rollCount / 2)));
	}

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
	m_text->SetAlign(CText::CENTER);
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
	m_text->SetAlign(CText::RIGHT);
	m_text->SetFontSize(150);

	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetParent(gameObject);
	m_bg->SetColor(D3DCOLOR_RGBA(59, 33, 87, 255));


	m_points = 0;
	m_viewPoints = 0;
	m_textScale = 1.0f;
}

//=============================================================
// [SumActionPointText] 終了
//=============================================================
void SumActionPointText::Uninit()
{
	m_text->Uninit();
	delete m_text;

	m_bg->Uninit();
	delete m_bg;
}

//=============================================================
// [SumActionPointText] 更新
//=============================================================
void SumActionPointText::Update()
{
	// 表示ポイントを決める
	m_viewPoints += static_cast<int>((m_points - m_viewPoints) * 0.08f);

	// ポイントを表示する
	m_text->SetText(std::to_string(m_viewPoints));
	//m_text->SetFontSize(static_cast<int>(120 * m_textScale));

	// サイズを戻していく
	m_textScale += (1.0f - m_textScale) * 0.08f;

	// 透明にしていく
	if (m_text->GetAlpha() > 0.05f)
	{
		m_text->SetAlpha(m_text->GetAlpha() - 0.01f);
	}
	else
	{ // 終了
		// ポイントを加算する
		CGameScene::AddActionPoint(m_points);

		m_text->SetAlpha(0.0f);
		m_points = 0;
		m_viewPoints = 0;
	}

	// 背景のサイズと位置を調整する
	m_bg->transform->SetSize(m_bg->transform->GetSize().x + (std::to_string(m_viewPoints).length() * 85.0f - m_bg->transform->GetSize().x) * 0.08f, 140.0f);
	float anim = m_bg->transform->GetSize().x - (std::to_string(m_viewPoints).length()) * 85.0f;
	m_bg->transform->SetPos(std::to_string(m_viewPoints).length() * -85.0f - anim, -15.0f);

	// 透明度の変更
	D3DXCOLOR bgColor = m_bg->GetColor();
	bgColor.a = m_text->GetAlpha();
	m_bg->SetColor(bgColor);

	// 背景の更新
	m_bg->Update();
}

//=============================================================
// [SumActionPointText] 描画
//=============================================================
void SumActionPointText::DrawUI()
{
	m_bg->DrawUI();
	m_text->DrawUI();
}

//=============================================================
// [SumActionPointText] ポイント加算
//=============================================================
void SumActionPointText::AddPoints(const int& points)
{
	// ポイントを加算する
	m_points += points;
	m_textScale = 1.5f;
	m_text->SetAlpha(1.0f);
}
