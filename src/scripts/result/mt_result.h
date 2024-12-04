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
class MountainResultManager
{
public:
	enum TYPE
	{
		TYPE_GAMEOVER,
		TYPE_CLEAR
	};

	MountainResultManager(CGameScene* scene) { m_gameScene = scene; }
	void Init(TYPE type);
	void Uninit();
	void Update();
	static void Reset();

	// 結果格納
	struct ResultData
	{
		int time;			// クリア時間
		int highSpeed;	// 最高速度
		int action;			// アクション
	};
	static void AddResult(ResultData data);
private:
	void InitClear();
	void InitGameOver();
	void UninitClear();
	void UninitGameOver();
	void UpdateClear();
	void UpdateGameOver();
	TYPE m_endType;

	void UpdateResultAnim();
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

	GameObject* m_bg;						// 背景
	GameObject* m_seedText;				// シードテキスト
	GameObject* m_dataView;				// データ表示
	GameObject* m_fuelView;				// 燃料表示
	GameObject* m_enduranceView;	// 耐久値表示
	GameObject* m_terrainImg;			// 地形画像
	CGameScene* m_gameScene;		// ゲームシーンの取得

	static float m_beforeFuel;			// 前回の燃料
	static float m_beforeEndurance;	// 前回の耐久値
	static UINT m_goalCount;		// 踏破回数
	static std::vector<ResultData> m_results;	// 結果

	ShopManager* m_shopManager;		// ショップ管理
};

#endif // !_RESULT_H_
