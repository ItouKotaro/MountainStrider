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

#endif // !_TERMINAL_MODE_H_
