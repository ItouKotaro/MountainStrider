//=============================================================
//
// �R���|�[�l���g [component.h]
// Author: Ito Kotaro
// 
// �Q�[���I�u�W�F�N�g�ɒǉ��ł���v�f���\��������̂ł�
//
//=============================================================
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "gameobject.h"

// �R���|�[�l���g�N���X
class Component
{
public:
	Component() {
		enabled = true;
		m_attached = false;
		gameObject = nullptr;
		transform = nullptr;
	}
	virtual ~Component() {
		if (this->gameObject != nullptr)
		{
			for (auto itr = m_pComponents.begin(); itr != m_pComponents.end(); itr++)
			{
				if (*itr == this)
				{
					SAFE_ERASE(m_pComponents, itr)
						break;
				}
			}
		}
	}

	/*
	@brief ����������
	@details �R���|�[�l���g���ǉ����ꂽ�ۂɌĂ΂��C�x���g
	*/
	virtual void Init() {}

	/*
	@brief �I������
	@details �R���|�[�l���g���j�����ꂽ�ۂɌĂ΂��C�x���g [����]
	*/
	virtual void Uninit() {}

	/*
	@brief �X�V����
	@details ���t���[���Ă΂��C�x���g
	*/
	virtual void Update() {}

	/*
	@brief �I�u�W�F�N�g�`��O����
	@details �I�u�W�F�N�g�`��O�ɌĂ΂��C�x���g
	*/
	virtual void BeforeDraw() {}

	/*
	@brief �I�u�W�F�N�g�`��㏈��
	@details �I�u�W�F�N�g�`���ɌĂ΂��C�x���g
	*/
	virtual void AfterDraw() {}

	/*
	@brief �I�u�W�F�N�g�`�揈��
	@details �I�u�W�F�N�g�`��̍ۂɌĂ΂��C�x���g
	*/
	virtual void Draw() {}

	/*
	@brief UI�`��O����
	@details UI�`��O�ɌĂ΂��C�x���g
	*/
	virtual void BeforeDrawUI() {}

	/*
	@brief UI�`��㏈��
	@details UI�`���ɌĂ΂��C�x���g
	*/
	virtual void AfterDrawUI() {}

	/*
	@brief UI�`�揈��
	@details UI�`��̍ۂɌĂ΂��C�x���g
	*/
	virtual void DrawUI() {}

	/*
	@brief ���̃R���W�����Əd�Ȃ��Ă���Ƃ��ɌĂ΂��C�x���g
	*/
	virtual void OnTriggerStay(GameObject* other) {}

	/*
	@brief ���̃R���W�����������Ă����Ƃ��̃C�x���g
	*/
	virtual void OnTriggerEnter(GameObject* other) {}

	/*
	@brief ���̃R���W�������o���Ƃ��̃C�x���g
	*/
	virtual void OnTriggerExit(GameObject* other) {}

	/*
	@brief ���̃I�u�W�F�N�g�������ꂽ�Ƃ��ɌĂ΂��C�x���g [����]
	*/
	virtual void OnDestroyOtherObject(GameObject* other) {}

	/*
	@brief ���̃R���|�[�l���g�������ꂽ�Ƃ��ɌĂ΂��C�x���g [����]
	*/
	virtual void OnDestroyOtherComponent(Component* other) {}

	/*
	@brief �S�R���|�[�l���g���擾����
	@param[in] bOnlyActive : �A�N�e�B�u��Ԃ̂�
	*/
	static std::vector<Component*> GetComponents(bool bOnlyActive = false) {
		std::vector<Component*> result;
		for (auto itr = m_pComponents.begin(); itr != m_pComponents.end(); itr++)
		{
			if (!bOnlyActive || (bOnlyActive && (*itr)->enabled && (*itr)->gameObject->GetActive()))
				result.push_back(*itr);
		}
		return result;
	}

	/*
	@brief �S�R���|�[�l���g���畡���̓���R���|�[�l���g���擾����
	@param[in] includeChild : �q�N���X���܂߂邩
	@param[in] bOnlyActive : �A�N�e�B�u��Ԃ̂�
	*/
	template<class T> static std::vector<T*> GetComponents(const bool& includeChild = false, const bool& bOnlyActive = false) {
		std::vector<T*> result;
		for (auto itr = m_pComponents.begin(); itr != m_pComponents.end(); itr++)
		{
			if (includeChild)
			{ // �q���܂ނƂ�
				if (T* pComp = dynamic_cast<T*>(*itr))
				{
					if (!bOnlyActive || (bOnlyActive && (*itr)->enabled && (*itr)->gameObject->GetActive()))
						result.push_back((T*)*itr);
				}
			}
			else
			{ // �q���܂܂Ȃ��Ƃ�
				if (typeid(T) == typeid(**itr))
				{
					if (!bOnlyActive || (bOnlyActive && (*itr)->enabled && (*itr)->gameObject->GetActive()))
						result.push_back((T*)*itr);
				}
			}
		}
		return result;
	}

	/*
	@brief �R���|�[�l���g�����݂��邩
	@param[in] pComponent : �R���|�[�l���g�̃|�C���^
	*/
	static bool IsExist(Component* pComponent);

	/*
	@brief �w��N���X�̃R���|�[�l���g��
	@param[in] pComponent : �R���|�[�l���g�̃|�C���^
	*/
	template<class T> static bool IsClassType(Component* pComponent)
	{
		if (typeid(*pComponent) == typeid(T))
		{ // ����
			return true;
		}
		return false;
	}

	/*
	@brief �Q�[���I�u�W�F�N�g�ɃA�^�b�`����
	@param[in] attachObj : �A�^�b�`��̃Q�[���I�u�W�F�N�g
	*/
	void AttachGameObject(GameObject* attachObj);

	/*
	@brief �R���|�[�l���g�̃\�[�g
	*/
	static void Sort();

	// @brief �L����Ԃ�
	bool enabled;

	// @brief �A�^�b�`��̃g�����X�t�H�[��
	Transform* transform;

	// @brief �A�^�b�`��̃Q�[���I�u�W�F�N�g
	GameObject* gameObject;
private:
	static std::vector<Component*> m_pComponents;
	bool m_attached;	// �A�^�b�`�ς݂�
};

#endif // !_COMPONENT_H_
