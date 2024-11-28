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
	void Reset();

	// 結果格納
	struct ResultData
	{
		int time;			// クリア時間
		int highSpeed;	// 最高速度
		int action;			// アクション
	};
	static void AddResult(ResultData data);
private:
	enum PROG_STATE
	{
		P_MTTEXT,					// 踏破数テキスト
		P_FUEL,							// バイクの燃料
		P_ENDURANCE,				// バイクの耐久値
		P_END
	};
	PROG_STATE m_progState;	// 結果表示進行度
	int m_progCounter;				// カウンター

	GameObject* m_mtText;		// マウンテンテキスト（山の踏破数）

	GameObject* m_bg;						// 背景（ガウスにする予定）
	GameObject* m_seedText;				// シードテキスト
	GameObject* m_dataView;				// データ表示
	GameObject* m_fuelView;				// 燃料表示
	GameObject* m_enduranceView;	// 耐久値表示

	GameObject* m_terrainImg;

	CGameScene* m_gameScene;


	static float m_beforeFuel;			// 前回の燃料
	static float m_beforeEndurance;	// 前回の耐久値
	static UINT m_goalCount;		// 踏破回数
	static std::vector<ResultData> m_results;	// 結果
};

#endif // !_RESULT_H_
