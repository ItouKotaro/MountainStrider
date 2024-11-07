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
	std::string className;	// �N���X��
	float rate;						// �m��
};

// �Q�[���I�u�W�F�N�g�ǐ՗p�R���|�[�l���g
class CProduceTag : public Component
{
public:
	CProduceTag(const std::string& className) { m_className = className; }
	std::string GetName() { return m_className; }
private:
	std::string m_className;
};

// ���R�̎Y���i���j
class CNatureProduces
{
public:
	CNatureProduces(const std::string& path, const D3DXVECTOR2& size, const float& offsetY)
	{
		m_path = path;
		m_size = size;
		m_offsetY = offsetY;
	}

	// �I�u�W�F�N�g�̐���
	void Generate(const Transform& transform);

	// �T�C�Y�̎擾
	D3DXVECTOR2 GetSize() { return m_size; }
	// �I�t�Z�b�gY�̎擾
	float GetOffsetY() { return m_offsetY; }

	// �m�����擾����
	unsigned int GetChance() { return m_chance; }
	// �m����ݒ肷��
	void SetChance(const int& chance) { m_chance = chance; }
	// �m���{�����擾����
	float GetAdjacentRate(const D3DXVECTOR3& pos);
	// �אڔ{���̐ݒ�
	template<class T> void SetAdjacentRate(const float& rate)
	{
		// ���ɃN���X���o�^����Ă��Ȃ���
		for (unsigned int i = 0; i < m_adjacentRates.size(); i++)
		{
			if (m_adjacentRates[i].className == typeid(T).name())
			{ // ��v����Ƃ�
				m_adjacentRates[i].rate = rate;	// �m��������������
				return;
			}
		}

		// �V�����o�^����
		AdjacentRate adjacent;
		adjacent.className = typeid(T).name();
		adjacent.rate = rate;
		m_adjacentRates.push_back(adjacent);
	}

protected:
	std::string m_path;		// �ݒu�v���n�u�̃p�X
	D3DXVECTOR2 m_size;	// �T�C�Y�iXZ�j
	float m_offsetY;				// Y�̃I�t�Z�b�g

	// �m��
	unsigned int m_chance;										// �W���m���i�����j
	std::vector<AdjacentRate> m_adjacentRates;		// �אڊm�����X�g
};


// --------------------------------- ��Q�� ---------------------------------

class CProdTree : public CNatureProduces
{
public:
	CProdTree() : CNatureProduces("data\\PREFAB\\bench.pref", {10.0f, 10.0f}, 5.0f) {}
private:

};

#endif // !_NATURE_PRODUCES_H_
