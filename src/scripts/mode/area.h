//=============================================================
//
// �G���A���[�h [area.h]
// Author: Ito Kotaro
// 
// �������ԕt���̃G���A����������āA���̃G���A�ɓ���ƃ|�C���g���ǉ�����Ă������[�h
// �Q�[�����̂ɐ������Ԃ������āA�I�����̃|�C���g�����ʂƂȂ�
//
//=============================================================
#ifndef _AREA_MODE_H_
#define _AREA_MODE_H_

#include "mode_manager.h"

// �G���A���[�h
class AreaMode : public ModeTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	// �|�C���g�����Z����
	void AddPoint(const int& point) { m_point += point; }
private:
	int m_point;				// �|�C���g
	float m_nextArea;	// ���̃G���A�𐶐����鎞��
};

// �G���A�R���|�[�l���g
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
