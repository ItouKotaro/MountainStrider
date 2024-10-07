//=============================================================
//
// �R���|�[�l���g [component.cpp]
// Author: Ito Kotaro
// 
// �Q�[���I�u�W�F�N�g�ɒǉ��ł���v�f���\��������̂ł�
//
//=============================================================
#include "component.h"
std::vector<Component*> Component::m_pComponents;

//=============================================================
// [Component] ���݂��邩
//=============================================================
bool Component::IsExist(Component* pComponent)
{
	for (unsigned int i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i] == pComponent)
		{	// ���݂��Ă���Ƃ�
			return true;
		}
	}
	return false;
}

//=============================================================
// [Component] �\�[�g
//=============================================================
void Component::Sort()
{
	// �A�^�b�`����Ă���Q�[���I�u�W�F�N�g�̗D��x�Ŕ�r���A�\�[�g����
	std::sort(m_pComponents.begin(), m_pComponents.end(), [](Component* com1, Component* com2) {return com1->gameObject->GetPriority() < com2->gameObject->GetPriority(); });
}