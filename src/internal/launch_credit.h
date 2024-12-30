//=============================================================
//
// 起動クレジット [launch_credit.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LAUNCH_CREDIT_H_
#define  _LAUNCH_CREDIT_H_

#include "component/2d/polygon.h"

// 起動クレジット
class LaunchCredit : public Component
{
public:
	LaunchCredit() : m_currentCreditID(-1), m_time(0.0f), m_showTime(2.0f), m_fadeTime(0.5f), m_alpha(1.0f), m_bgPoly(nullptr), m_state(STATE::NOT_STARTED){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	//@brief 開始する
	void Start();

	//@brief クレジットを追加する
	//@return クレジットID
	int AddCredit(const std::string& path, const D3DXVECTOR2& size, const float& scale = 1.0f);

	//@brief 表示時間の設定
	void SetShowTime(const float& time) { m_showTime = time; }

	//@brief フェード時間の設定
	void SetFadeTime(const float& time) { m_fadeTime = time; }

	//@brief 終了したか
	bool IsEnded() { return m_state == STATE::ENDED; }

	//@brief 表示中のクレジットIDを取得する
	//@return クレジットID
	int GetCurrentShow() { return m_currentCreditID; }

	//@brief クレジットの不透明度を取得する
	//@return 不透明度 (0.0~1.0)
	float GetCurrentAlpha() { return m_alpha; }

	//@brief 背景を指定して背景の不透明度を調整します
	void SetBG(CPolygon* polygon) { m_bgPoly = polygon; }
private:
	// クレジットロゴ
	struct CreditLogo
	{
		std::string path;			// パス
		D3DXVECTOR2 size;	// サイズ
		float scale;					// スケール
	};

	// 状態
	enum STATE
	{
		NOT_STARTED,	// 始まっていない
		MIDST,				// 進行中
		ENDED,				// 終了
	};

	SingleComponent<CPolygon>* m_creditLogo;		// クレジットロゴ
	std::vector<CreditLogo> m_credits;					// クレジットリスト

	// 内部処理
	float m_time;														// 時間
	int m_currentCreditID;										// 現在のクレジットID
	STATE m_state;													// 状態
	float m_alpha;													// 不透明度
	CPolygon* m_bgPoly;											// 背景ポリゴン

	// 設定項目
	float m_showTime;												// 表示時間
	float m_fadeTime;												// フェード時間
};

#endif // !_LAUNCH_CREDIT_H_
