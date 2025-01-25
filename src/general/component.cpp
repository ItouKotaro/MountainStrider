//=============================================================
//
// コンポーネント [component.cpp]
// Author: Ito Kotaro
// 
// ゲームオブジェクトに追加できる要素を構成するものです
//
//=============================================================
#include "component.h"

//=============================================================
// [Component] ゲームオブジェクトにアタッチする
//=============================================================
void Component::AttachGameObject(GameObject* attachObj)
{
	if (!m_attached)
	{ // アタッチされていないとき
		gameObject = attachObj;
		transform = gameObject->transform;
		m_attached = true;
	}
}

//=============================================================
// [Component] 存在するか
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
			{	// 存在しているとき
				return true;
			}
		}
	}
	return false;
}

//=============================================================
// [Component] パスの開始
//=============================================================
void Component::BeginPass()
{
	if (m_shader != nullptr)
	{
		m_shader->BeginPass(m_pass);
	}
}

//=============================================================
// [Component] パスの終了
//=============================================================
void Component::EndPass()
{
	if (m_shader != nullptr)
	{
		m_shader->EndPass();
	}

	// シェーダー情報をリセットする
	m_shader = nullptr;
	m_pass = 0;
}

//=============================================================
// [Component] シェーダーに情報を渡す
//=============================================================
void Component::SetParam(Shader::ParamData data)
{
	if (m_shader != nullptr)
	{
		data.component = this;
		m_shader->SetParam(data);
	}
}