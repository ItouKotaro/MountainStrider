//=============================================================
//
// �R���|�[�l���g [component.cpp]
// Author: Ito Kotaro
// 
// �Q�[���I�u�W�F�N�g�ɒǉ��ł���v�f���\��������̂ł�
//
//=============================================================
#include "component.h"

//=============================================================
// [Component] �Q�[���I�u�W�F�N�g�ɃA�^�b�`����
//=============================================================
void Component::AttachGameObject(GameObject* attachObj)
{
	if (!m_attached)
	{ // �A�^�b�`����Ă��Ȃ��Ƃ�
		gameObject = attachObj;
		transform = gameObject->transform;
		m_attached = true;
	}
}

//=============================================================
// [Component] ���݂��邩
//=============================================================
bool Component::IsExist(Component* pComponent)
{
	auto gameObjects = GameObject::GetAllGameObjects();
	for (auto objItr = gameObjects.begin(); objItr != gameObjects.end(); objItr++)
	{
		auto components = (*objItr)->GetComponents();
		for (auto itr = components.begin(); itr != components.end(); itr++)
		{
			if (*itr == pComponent)
			{	// ���݂��Ă���Ƃ�
				return true;
			}
		}
	}
	return false;
}

//=============================================================
// [Component] �p�X�̊J�n
//=============================================================
void Component::BeginPass()
{
	if (m_shader != nullptr)
	{
		m_shader->BeginPass(m_pass);
	}
}

//=============================================================
// [Component] �p�X�̏I��
//=============================================================
void Component::EndPass()
{
	if (m_shader != nullptr)
	{
		m_shader->EndPass();
	}

	// �V�F�[�_�[�������Z�b�g����
	m_shader = nullptr;
	m_pass = 0;
}

//=============================================================
// [Component] �V�F�[�_�[�ɏ���n��
//=============================================================
void Component::SetParam(Shader::ParamData data)
{
	if (m_shader != nullptr)
	{
		data.component = this;
		m_shader->SetParam(data);
	}
}