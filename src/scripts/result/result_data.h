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

#include "component/2d/polygon.h"
#include "component/2d/bar.h"
#include "component/2d/text.h"

// データ表示管理コンポーネント
class ResultDataView : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	void SetTime(const int& time);
	void SetHighSpeed(const int& kmh);
	void SetAction(const int& score);

	static const D3DXVECTOR2 TITLE_BG_SIZE;
	static const float SPACE_SIZE;
private:
	SingleComponent<CPolygon>* m_timeTitleBG;
	SingleComponent<CText>* m_timeTitleText;
	SingleComponent<CPolygon>* m_timeDataBG;
	SingleComponent<CText>* m_timeDataText;

	SingleComponent<CPolygon>* m_speedTitleBG;
	SingleComponent<CText>* m_speedTitleText;
	SingleComponent<CPolygon>* m_speedDataBG;
	SingleComponent<CText>* m_speedDataText;

	SingleComponent<CPolygon>* m_actionTitleBG;
	SingleComponent<CText>* m_actionTitleText;
	SingleComponent<CPolygon>* m_actionDataBG;
	SingleComponent<CText>* m_actionDataText;
};



#endif // !_RESULT_DATA_H_
