//=============================================================
//
// ���U���g�J���� [result_camera.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_CAMERA_H_
#define _RESULT_CAMERA_H_

#include "component.h"

// ���U���g�J�����̊Ǘ�
class ResultCamera : public Component
{
public:
	void Init();
	void Update();
	void RecordData();
	void Play();

	static const int RECORD_COUNT;
private:
	// �L�^�p�\����
	struct Record
	{
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
	};

	std::vector<Record> m_records;		// �L�^
	GameObject* m_pCamera;				// �J����

	int m_nextCounter;	// �L�^�J�E���^�[

	// �Đ�
	int m_recIdx;
	bool m_isPlay;
	D3DXVECTOR3 m_moving;
	float m_fade;
};

#endif // !_RESULT_CAMERA_H_
