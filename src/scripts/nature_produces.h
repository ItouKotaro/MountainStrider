//=============================================================
//
// ���R�̎Y�� [nature_produces.h]
// Author: Ito Kotaro
//
// �y�m���̎d�g�݁z
// �������̊m�������ׂĉ��Z���A�����_���őI��
// 
//=============================================================
#ifndef _NATURE_PRODUCES_H_
#define _NATURE_PRODUCES_H_

#include "component.h"

// �אڊm��
struct AdjacentRate
{
	std::string produceName;			// �Y����
	float rate;						// �m��
};

// �������i���j
class CNatureProduces
{
public:
	CNatureProduces() :
		m_path(""),
		m_size(10.0f),
		m_offsetY(0.0f),
		m_produceName(""),
		m_damage(10.0f),
		m_angleRange(0.0f),
		m_isMatchInclination(true),
		m_chance(0),
		m_adjacentDistance(100.0f){}

	// �o�C�N�ƃq�b�g�����Ƃ��̃C�x���g
	virtual void onHit(GameObject* gameObject) {}


	// �I�u�W�F�N�g�̐���
	GameObject* Generate(const Transform& transform);
	// ���O�̐ݒ�
	void SetName(const std::string& name) { m_produceName = name; }
	// �p�X�̐ݒ�
	void SetPath(const std::string& path) { m_path = path; }
	// �T�C�Y�̐ݒ�
	void SetSize(const float& size) { m_size = size; }
	// �_���[�W�̐ݒ�
	void SetDamage(const float& damage) { m_damage = damage; }
	// �I�t�Z�b�gY�̐ݒ�
	void SetOffsetY(const float& offset) { m_offsetY = offset; }
	// �����_���p�x�̐ݒ�
	void SetAngleRange(const float& angle) { m_angleRange = angle; }
	// �X�Ίp�ɍ��킹�邩
	void SetIsMatchInclination(const bool& isMatchInclination) { m_isMatchInclination = isMatchInclination; }
	// �T�C�Y�̎擾
	float GetSize() { return m_size; }
	// �I�t�Z�b�gY�̎擾
	float GetOffsetY() { return m_offsetY; }
	// �Y�����̎擾
	std::string GetProduceName() { return m_produceName; }
	// �_���[�W�ʂ̎擾
	float GetDamage() { return m_damage; }

	// �m�����擾����
	unsigned int GetChance() { return m_chance; }
	// �m����ݒ肷��
	void SetChance(const int& chance) { m_chance = chance; }
	// �m���{�����擾����
	float GetAdjacentRate(const D3DXVECTOR3& pos);
	// �אڋ����̐ݒ�
	void SetAdjacentDistance(const float& distance) { m_adjacentDistance = distance; }
	// ���������Ƃ̗אڔ{���̐ݒ�
	void SetAdjacentRate(const std::string& produceName, const float& rate)
	{
		// ���ɃN���X���o�^����Ă��Ȃ���
		for (unsigned int i = 0; i < m_adjacentRates.size(); i++)
		{
			if (m_adjacentRates[i].produceName == produceName)
			{ // ��v����Ƃ�
				m_adjacentRates[i].rate = rate;	// �m��������������
				return;
			}
		}

		// �V�����o�^����
		AdjacentRate adjacent;
		adjacent.produceName = produceName;
		adjacent.rate = rate;
		m_adjacentRates.push_back(adjacent);
	}
	// �������̃N���X�̔{�����擾����
	float GetAdjacentObjectRate(const std::string& produceName)
	{
		for (unsigned int i = 0; i < m_adjacentRates.size(); i++)
		{
			if (m_adjacentRates[i].produceName == produceName)
			{ // ��v����Ƃ�
				return m_adjacentRates[i].rate;
			}
		}
		// ������Ȃ������Ƃ�
		return 1.0f;
	}

	// �����_���p�x
	float GetAngleRange() { return m_angleRange; }

	// �X�Ίp�ɍ��킹�邩
	bool IsMatchInclination() { return m_isMatchInclination; }

protected:
	std::string m_path;				// �ݒu�v���n�u�̃p�X
	float m_size;							// �T�C�Y
	float m_offsetY;						// Y�̃I�t�Z�b�g
	std::string m_produceName;	// �Y����
	float m_damage;					// �_���[�W�ʁi�q�b�g���j
	float m_angleRange;				// �����_���p�x
	bool m_isMatchInclination;		// �X�Ίp���l�����邩

	// �m��
	unsigned int m_chance;										// �W���m���i�����j
	std::vector<AdjacentRate> m_adjacentRates;		// �אڊm�����X�g
	float m_adjacentDistance;									// �אڋ���
};

// --------------------------------- ��Q�� ---------------------------------

//// ��
//class CProdTree00 : public CNatureProduces
//{
//public:
//	CProdTree00() : CNatureProduces("tree00", "data\\PREFAB\\tree\\tree00.pref", 10.0f, 10.0f, 0.0f, 0.5f, false) {}
//};
//class CProdTree01 : public CNatureProduces
//{
//public:
//	CProdTree01() : CNatureProduces("tree01", "data\\PREFAB\\tree\\tree01.pref", 10.0f, 10.0f, 0.0f, 0.5f, false) {}
//};
//class CProdFallenTree00 : public CNatureProduces
//{
//public:
//	CProdFallenTree00() : CNatureProduces("fallen_tree00", "data\\PREFAB\\tree\\fallen_tree.pref", 10.0f, 30.0f) {}
//};

#endif // !_NATURE_PRODUCES_H_
