//=============================================================
//
// リザルト [result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _MT_RESULT_H_
#define _MT_RESULT_H_

#include "gameobject.h"

class CGameScene;
class MountainResultManager
{
public:
	MountainResultManager(CGameScene* scene) { m_gameScene = scene; }
	void Init();
	void Uninit();
	void Update();
private:
	enum PROG_STATE
	{
		P_MT_TEXT,					// 踏破数テキスト
		P_FUEL_TEXT,				// 燃料増減テキスト
		P_FUEL_BAR,					// 燃料バー増減
		P_ENDURANCE_TEXT,	// 耐久値増減テキスト
		P_ENDURANCE_BAR,		// 耐久値バー増減
		P_END
	};
	PROG_STATE m_progState;
	int m_progCounter;

	GameObject* m_mtText;		// マウンテンテキスト（山の踏破数）

	GameObject* m_bg;				// 背景（ガウスにする予定）
	GameObject* m_seedText;		// シードテキスト
	GameObject* m_fuelBar;			// 燃料バー
	GameObject* m_enduranceBar;	// 耐久値バー

	CGameScene* m_gameScene;




	static float m_oldFuel;			// 前回の燃料
	static float m_oldEndurance;	// 前回の耐久値
	static UINT m_goalCount;		// 踏破回数
};

#endif // !_RESULT_H_
