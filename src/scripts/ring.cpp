//=============================================================
//
// �����O [ring.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "ring.h"
#include "vehicle.h"
#include "shop/shop.h"
#include "wreckage.h"

#include "scene/game.h"

AudioClip PointRing::m_throughSE = nullptr;

//=============================================================
// [PointRing] �X�V
//=============================================================
void PointRing::Update()
{
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause()->GetPause())
		return;

	// ��]������
	transform->Rotate(0.0f, ROLL_ANGLE, 0.0f);
}

//=============================================================
// [PointRing] �g���K�[�ɓ������Ƃ�
//=============================================================
void PointRing::OnTriggerExit(GameObject* other)
{
	if (other->GetComponent<CVehicle>() != nullptr)
	{
		// ���𐶐�����
		if (!m_throughSE) m_throughSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\HIT\\ring.mp3");

		// �|�C���g��ǉ�����
		ShopManager::AddPoint(POINT);

		// ���X�g����폜����
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetDecoration()->RemoveData(gameObject);
		
		// ���X�ɔj������
		gameObject->AddComponent<CWreckage>();

		// ����炷
		gameObject->AddComponent<AudioSource>()->Play(m_throughSE);
	}
}