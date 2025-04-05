//=============================================================
//
// リザルト [mt_result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MT_RESULT_H_
#define _MT_RESULT_H_

#include "gameobject.h"
#include "scripts/shop/shop.h"

class CGameScene;

// リザルトの基底
class ResultBase
{
public:
	ResultBase();
	virtual ~ResultBase() {};

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	// 結果格納
	struct ResultData
	{
		int time;			// クリア時間
		int highSpeed;	// 最高速度
		int action;			// アクション
		float mileage;		// 走行距離
		float fuel;			// 消費燃料
		float endurance;	// 消費耐久値
	};

protected:
	CGameScene* m_gameScene;
};


#endif // !_RESULT_H_
