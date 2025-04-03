//=============================================================
//
// ダメージエフェクト [damage_effect.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "damage_effect.h"
#include "manager.h"

//=============================================================
// [DamageEffect] 初期化
//=============================================================
void DamageEffect::Init()
{
	// カメラを取得する
	m_camera = GameObject::Find("Camera");
}

//=============================================================
// [DamageEffect] 終了
//=============================================================
void DamageEffect::Uninit()
{
	for (auto itr = m_billboards.begin(); itr != m_billboards.end(); itr++)
	{
		(*itr)->Destroy();
	}

	// リストを削除する
	m_billboards.clear();
}

//=============================================================
// [DamageEffect] 更新
//=============================================================
void DamageEffect::Update()
{
	// 向かせる
	if (m_camera != nullptr)
	{
		transform->SetRot(0.0f, Benlib::PosAngle(transform->GetWPos(), m_camera->transform->GetWPos()), 0.0f);
	}

	// 上昇させる
	transform->Translate(0.0f, UP_MOVE, 0.0f);

	// 透明化
	for (auto itr = m_billboards.begin(); itr != m_billboards.end(); itr++)
	{
		D3DXCOLOR color = (*itr)->GetComponent<CBillboard>()->GetColor();
		color.a -= ALPHA;
		(*itr)->GetComponent<CBillboard>()->SetColor(color);
	}

	// 寿命
	m_time -= CManager::GetInstance()->GetDeltaTime();
	if (m_time <= 0.0f)
	{
		gameObject->Destroy();
	}
}

//=============================================================
// [DamageEffect] 描画
//=============================================================
void DamageEffect::Draw()
{

}

//=============================================================
// [DamageEffect] 生成
//=============================================================
void DamageEffect::Generate(const float& damage, const float& time)
{
	// 時間を設定する
	m_time = time;

	// 数字の割り出しに必要な計算
	int numInt = static_cast<int>(damage);
	int numFloat = static_cast<int>((damage - static_cast<float>(numInt)) / 0.1f);
	int digit = static_cast<int>(log10f(static_cast<float>(numInt))) + 2;	// 2はlog10の桁数補完と少数第一位の合計
	float widthLength = digit * SIZE / 2.0f;	// ビルボードの位置

	// 必要なビルボードを用意する
	for (int i = 0; i < digit; i++)
	{
		// 生成
		GameObject* billboard = new GameObject();
		billboard->AddComponent<CBillboard>()->Set(SIZE, SIZE);

		// 対象の数字を取得する
		int targetNum = static_cast<int>(numInt / powf(10, static_cast<float>(digit - i - 2)));
		numInt -= static_cast<int>(targetNum * powf(10, static_cast<float>(digit - i - 2)));

		// 最後のループ（小数点の数に置き換える）
		if (i == digit - 1)
		{
			// 小数点の数にターゲットを置き換える
			targetNum = numFloat;

			// ピリオドビルボードを生成する
			GameObject* periodBillboard = new GameObject();
			periodBillboard->AddComponent<CBillboard>()->Set(SIZE, SIZE);
			periodBillboard->GetComponent<CBillboard>()->SetTexture("data\\TEXTURE\\UI\\NUMBER\\period.png");
			periodBillboard->transform->SetPos(widthLength + SIZE / 2.0f, 0.0f, 0.0f);
			periodBillboard->SetParent(gameObject);
			m_billboards.emplace_back(periodBillboard);
		}

		// テクスチャパス
		char texturePath[MAX_PATH];
		sprintf(&texturePath[0], "data\\TEXTURE\\UI\\NUMBER\\%d.png", targetNum);

		// ビルボードの設定を行う
		billboard->GetComponent<CBillboard>()->SetTexture(texturePath);

		// ビルボードの位置
		billboard->transform->SetPos(widthLength, 0.0f, 0.0f);
		widthLength -= SIZE;
		billboard->SetParent(gameObject);
		
		// リストに追加する
		m_billboards.emplace_back(billboard);
	}

	
}
