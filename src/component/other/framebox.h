//=============================================================
//
// フレームボックス [framebox.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _FRAMEBOX_H_
#define _FRAMEBOX_H_

#include "component.h"
#include "component/2d/polygon.h"

class FrameBox : public Component
{
public:
	FrameBox() : m_frameSize(5.0f), m_mainImg(nullptr), m_maskImg(nullptr){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	void SetTexture(const std::string& path);
	void SetFrameSize(const float& size) { m_frameSize = size; }
private:
	float m_frameSize;
	SingleComponent<CPolygon>* m_mainImg;
	SingleComponent<CPolygon>* m_maskImg;
};

#endif // !_FRAMEBOX_H_
