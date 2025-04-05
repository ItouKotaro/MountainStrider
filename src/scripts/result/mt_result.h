//=============================================================
//
// ���U���g [mt_result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MT_RESULT_H_
#define _MT_RESULT_H_

#include "gameobject.h"
#include "scripts/shop/shop.h"

class CGameScene;

// ���U���g�̊��
class ResultBase
{
public:
	ResultBase();
	virtual ~ResultBase() {};

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	// ���ʊi�[
	struct ResultData
	{
		int time;			// �N���A����
		int highSpeed;	// �ō����x
		int action;			// �A�N�V����
		float mileage;		// ���s����
		float fuel;			// ����R��
		float endurance;	// ����ϋv�l
	};

protected:
	CGameScene* m_gameScene;
};


#endif // !_RESULT_H_
