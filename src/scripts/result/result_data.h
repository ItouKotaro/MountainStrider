//=============================================================
//
// データ表示 [result_data.h]
// Author: Ito Kotaro
// 
// クリアタイム、最高速度、アクション
// 
//=============================================================
#ifndef _RESULT_DATA_H_
#define _RESULT_DATA_H_

#include "component.h"

// データ表示管理コンポーネント
class ResultDataView : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	static const D3DXVECTOR2 TITLE_BG_SIZE;
private:
	GameObject* m_timeTitleBG;
	GameObject* m_timeTitleText;
	GameObject* m_timeDataBG;
	GameObject* m_timeDataText;
};



#endif // !_RESULT_DATA_H_
