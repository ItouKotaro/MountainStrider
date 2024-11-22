//=============================================================
//
// リザルトカメラ [result_camera.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_camera.h"

const int ResultCamera::RECORD_COUNT = 60;		// 記録頻度

//=============================================================
// [ResultCamera] 初期化
//=============================================================
void ResultCamera::Init()
{
	m_isPlay = false;
	m_nextCounter = 20;
	m_recIdx = 0;
	m_fade = 1.0f;
	m_records.clear();

	// カメラを取得する
	m_pCamera = GameObject::Find("Camera");
}

//=============================================================
// [ResultCamera] 更新
//=============================================================
void ResultCamera::Update()
{
	// 例外（記録が無いとき）
	if (m_records.size() <= 0)
	{
		return;
	}

	if (m_isPlay)
	{ // 再生中
		Record rec = m_records[m_recIdx];

		// 移動させる
		m_pCamera->transform->Translate(m_moving * m_fade);

		// 回転量を決める
		D3DXQUATERNION objective_q;
		D3DXQUATERNION current_q = m_pCamera->transform->GetWQuaternion();
		D3DXQuaternionSlerp(&objective_q, &current_q, &rec.rot, (1.0f - m_fade));
		m_pCamera->transform->SetQuaternion(objective_q);

		// フェード
		m_fade *= 0.998f;

		// カウンター処理
		if (Benlib::PosDistance(m_pCamera->transform->GetWPos(), rec.pos) < 50.0f)
		{
			m_recIdx++;

			if (m_recIdx >= m_records.size())
			{ // 最大に到達したとき
				m_recIdx = 0;	// 戻す
			}
			m_fade = 1.0f;

			// 移動量を決める
			m_moving = (m_records[m_recIdx].pos - m_pCamera->transform->GetWPos()) / 60;
		}
	}
}

//=============================================================
// [ResultCamera] 記録する
//=============================================================
void ResultCamera::RecordData()
{
	m_nextCounter--;

	if (m_nextCounter <= 0)
	{
		// カメラの情報を記録する
		Record rec;

		// 位置
		rec.pos = m_pCamera->transform->GetWPos();
		rec.pos.y += 150.0f;

		// 向き
		rec.rot = m_pCamera->transform->GetWQuaternion();

		// 追加する
		m_records.push_back(rec);

		// カウンターを元に戻す
		m_nextCounter = RECORD_COUNT;
	}
}

//=============================================================
// [ResultCamera] 再生
//=============================================================
void ResultCamera::Play()
{
	m_isPlay = true;

	if (m_records.size() > 0)
	{
		m_moving = (m_records[0].pos - m_pCamera->transform->GetWPos()) / 60;
	}
}