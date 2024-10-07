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
// [Component] 存在するか
//=============================================================
bool Component::IsExist(Component* pComponent)
{
	for (unsigned int i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i] == pComponent)
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
	std::sort(m_pComponents.begin(), m_pComponents.end(), [](Component* com1, Component* com2) {return com1->gameObject->GetPriority() < com2->gameObject->GetPriority(); });
}