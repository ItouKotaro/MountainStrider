//=============================================================
//
// コンポーネント [component.cpp]
// Author: Ito Kotaro
// 
// ゲームオブジェクトに追加できる要素を構成するものです
//
//=============================================================
#include "component.h"
std::vector<Component*> Component::m_pComponents;

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
		m_pComponents.push_back(this);
	}
}

//=============================================================
// [Component] 存在するか
//=============================================================
bool Component::IsExist(Component* pComponent)
{
	for (auto itr = m_pComponents.begin(); itr != m_pComponents.end(); itr++)
	{
		if (*itr == pComponent)
		{	// 存在しているとき
			return true;
		}
	}
	return false;
}

//=============================================================
// [Component] ソート
//=============================================================
void Component::Sort()
{
	// アタッチされているゲームオブジェクトの優先度で比較し、ソートする
	std::sort(m_pComponents.begin(), m_pComponents.end(), [](Component* com1, Component* com2) {
		return com1->gameObject->GetPriority() < com2->gameObject->GetPriority();
		});
}