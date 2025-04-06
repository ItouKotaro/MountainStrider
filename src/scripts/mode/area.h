//=============================================================
//
// エリアモード [area.h]
// Author: Ito Kotaro
// 
// 制限時間付きのエリアが生成されて、そのエリアに入るとポイントが追加されていくモード
// ゲーム自体に制限時間があって、終了時のポイントが結果となる
//
//=============================================================
#ifndef _AREA_MODE_H_
#define _AREA_MODE_H_

#include "mode_manager.h"

// エリアモード
class AreaMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	// ポイントを加算する
	void AddPoint(const int& point) { m_point += point; }
private:
	int m_point;				// ポイント
	float m_nextArea;	// 次のエリアを生成する時間
};

// エリアコンポーネント
class AreaPoint : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	GameObject* m_vehicle;
};

#endif // !_AREA_MODE_H_
