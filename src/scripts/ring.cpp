//=============================================================
//
// �����O [ring.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "ring.h"
#include "vehicle.h"
#include "shop/shop.h"
#include "destructible.h"

#include "scene/game.h"

//=============================================================
// [PointRing] �X�V
//=============================================================
void PointRing::Update()
{
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
		// �|�C���g��ǉ�����
		ShopManager::AddPoint(POINT);

		// �j������
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetDecoration()->RemoveData(gameObject);
		gameObject->Destroy();
	}
}