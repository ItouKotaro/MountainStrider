//=============================================================
//
// 終端モード [terminal.h]
// Author: Ito Kotaro
// 
// ワールドの端まで到達するとゴールする
//
//=============================================================
#ifndef _TERMINAL_MODE_H_
#define _TERMINAL_MODE_H_

#include "mode_manager.h"

// 終端モード
class TerminalMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void OnResultEvent() override;

	// リザルトデータを取得する
	std::vector<ResultBase::ResultData>& GetResultData() { return m_resultDatas; }
private:
	D3DXVECTOR3 CalcNearGoal();
	GameObject* m_directionObj;
	std::vector<ResultBase::ResultData> m_resultDatas;
	const float EXTENSION_DISTANCE = 150.0f;
};


// ターミナルモードのリザルト
class TerminalResult : public ResultBase
{
public:
	int GetAverageTime();
	int GetAverageAction();
	int GetNumOfStep();
	float GetBeforeFuel() { return m_beforeFuel; }
	float GetBeforeEndurance() { return m_beforeEndurance; }
protected:
	void FinalResult(bool isSuccess);

	static float m_beforeFuel;							// 前回の燃料
	static float m_beforeEndurance;					// 前回の耐久値
	GameObject* m_page;			// ページ管理

	// 最終結果用
	GameObject* m_scoreText;			// スコア
	GameObject* m_timeRate;				// 時間の評価
	GameObject* m_actionRate;			// アクションの評価
	GameObject* m_fuelRate;				// 燃費の評価
	GameObject* m_timeValue;			// 時間表示
	GameObject* m_actionValue;			// アクション表示
	GameObject* m_fuelValue;				// 燃費表示

	const int CLEAR_POINT = 2500;
	const int END_POINT = 2000;
};

// クリア時のリザルト
class ClearTerminalResult : public TerminalResult
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
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

	GameObject* m_bg;						// 背景
	GameObject* m_mtText;				// マウンテンテキスト（山の踏破数）
	GameObject* m_seedText;				// シードテキスト
	GameObject* m_dataView;				// データ表示
	GameObject* m_fuelView;				// 燃料表示
	GameObject* m_enduranceView;	// 耐久値表示
	GameObject* m_terrainImg;			// 地形画像
	GameObject* m_descriptionUI;		// 説明UI

	// 音
	AudioClip m_bgm;
	AudioClip m_clickSE;
	GameObject* m_bgmObj;
	float m_volumeFade;
	const float BGM_VOLUME = 0.2f;
	const float BGM_FADE = 0.001f;
	const char* DESC_END_TEXTURE = "data\\TEXTURE\\RESULT\\desc_end.png";
	const char* DESC_NEXT_TEXTURE = "data\\TEXTURE\\RESULT\\desc_next.png";
	const D3DXVECTOR2 DESC_SIZE = { 620.0f, 297.0f };

	// ボタン
	GameObject* m_endButton;
	GameObject* m_nextButton;

	ShopManager* m_shopManager;		// ショップ管理
};


// ゲームオーバー時のリザルト
class GameOverTerminalResult : public TerminalResult
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	// リザルトアニメーションの更新
	void UpdateResultAnim();

	enum PROG_STATE
	{
		P_FUEL,							// バイクの燃料
		P_ENDURANCE,				// バイクの耐久値
		P_END
	};
	PROG_STATE m_progState;	// 結果表示進行度
	int m_progCounter;				// カウンター

	GameObject* m_mtText;				// マウンテンテキスト（ゲームオーバー表示）

	GameObject* m_bg;						// 背景
	GameObject* m_seedText;				// シードテキスト
	GameObject* m_dataView;				// データ表示
	GameObject* m_fuelView;				// 燃料表示
	GameObject* m_enduranceView;	// 耐久値表示
	GameObject* m_terrainImg;			// 地形画像

	// 音
	AudioClip m_bgm;
	GameObject* m_bgmObj;
	float m_volumeFade;
	const float BGM_VOLUME = 0.6f;
	const float BGM_FADE = 0.004f;
};

#endif // !_TERMINAL_MODE_H_
