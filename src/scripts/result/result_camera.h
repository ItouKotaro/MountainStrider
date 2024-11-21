//=============================================================
//
// リザルトカメラ [result_camera.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_CAMERA_H_
#define _RESULT_CAMERA_H_

#include "component.h"

// リザルトカメラの管理
class ResultCamera : public Component
{
public:
	void Init();
	void Update();
	void RecordData();
	void Play();

	static const int RECORD_COUNT;
private:
	// 記録用構造体
	struct Record
	{
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
	};

	std::vector<Record> m_records;		// 記録
	GameObject* m_pCamera;				// カメラ

	int m_nextCounter;	// 記録カウンター

	// 再生
	int m_recIdx;
	bool m_isPlay;
	D3DXVECTOR3 m_moving;
	float m_fade;
};

#endif // !_RESULT_CAMERA_H_
