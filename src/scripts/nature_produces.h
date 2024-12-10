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
	CNatureProduces(const std::string& name, const std::string& path, const D3DXVECTOR2& size, const float& damage = 0.0f, const float& offsetY = 0.0f, const float& adjacentDistance = 100.0f)
	{
		m_produceName = name;
		m_path = path;
		m_size = size;
		m_damage = damage;
		m_offsetY = offsetY;
		m_adjacentDistance = adjacentDistance;
	}

	// �o�C�N�ƃq�b�g�����Ƃ��̃C�x���g
	virtual void onHit(GameObject* gameObject) {}


	// �I�u�W�F�N�g�̐���
	GameObject* Generate(const Transform& transform);

	// �T�C�Y�̎擾
	D3DXVECTOR2 GetSize() { return m_size; }
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

protected:
	std::string m_path;				// �ݒu�v���n�u�̃p�X
	D3DXVECTOR2 m_size;			// �T�C�Y�iXZ�j
	float m_offsetY;						// Y�̃I�t�Z�b�g
	std::string m_produceName;	// �Y����
	float m_damage;					// �_���[�W�ʁi�q�b�g���j

	// �m��
	unsigned int m_chance;										// �W���m���i�����j
	std::vector<AdjacentRate> m_adjacentRates;		// �אڊm�����X�g
	float m_adjacentDistance;									// �אڋ���
};

// --------------------------------- ��Q�� ---------------------------------

// ��
class CProdTree00 : public CNatureProduces
{
public:
	CProdTree00() : CNatureProduces("tree00", "data\\PREFAB\\tree\\tree00.pref", {10.0f, 10.0f}, 10.0f) {}
};
class CProdTree01 : public CNatureProduces
{
public:
	CProdTree01() : CNatureProduces("tree01", "data\\PREFAB\\tree\\tree01.pref", { 10.0f, 10.0f }, 10.0f) {}
};
class CProdFallenTree00 : public CNatureProduces
{
public:
	CProdFallenTree00() : CNatureProduces("fallen_tree00", "data\\PREFAB\\tree\\fallen_tree.pref", { 10.0f, 10.0f }, 30.0f) {}
};

#endif // !_NATURE_PRODUCES_H_
