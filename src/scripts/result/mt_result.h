//=============================================================
//
// リザルト [mt_result.h]
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
		P_MTTEXT,					// 踏破数テキスト
		P_DATA,						// データの表示
		P_VEHICLE,					// バイクの情報表示
		P_END
	};
	PROG_STATE m_progState;	// 結果表示進行度
	int m_progCounter;				// カウンター

	GameObject* m_mtText;		// マウンテンテキスト（山の踏破数）

	GameObject* m_bg;				// 背景（ガウスにする予定）
	GameObject* m_seedText;		// シードテキスト
	GameObject* m_fuelBar;			// 燃料バー
	GameObject* m_enduranceBar;	// 耐久値バー
	GameObject* m_dataView;			// データ表示
	GameObject* m_terrainImg;

	CGameScene* m_gameScene;




	static float m_oldFuel;			// 前回の燃料
	static float m_oldEndurance;	// 前回の耐久値
	static UINT m_goalCount;		// 踏破回数
};

#endif // !_RESULT_H_
