//=============================================================
//
// コンポーネント [component.h]
// Author: Ito Kotaro
// 
// ゲームオブジェクトに追加できる要素を構成するものです
//
//=============================================================
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "gameobject.h"

// コンポーネントクラス
class Component
{
public:
	Component() { 
		enabled = true;
		gameObject = nullptr;
		transform = nullptr;
		m_pComponents.push_back(this);
	}
	virtual ~Component() {
		for (int i = 0; i < (int)m_pComponents.size(); i++)
		{
			if (m_pComponents.at(i) == this)
			{
				m_pComponents.erase(m_pComponents.begin() + i);
				break;
			}
		}
	}

	/*
	@brief 初期化処理
	@details コンポーネントが追加された際に呼ばれるイベント
	*/
	virtual void Init() {}

	/*
	@brief 終了処理
	@details コンポーネントが破棄された際に呼ばれるイベント [強制]
	*/
	virtual void Uninit() {}

	/*
	@brief 更新処理
	@details 毎フレーム呼ばれるイベント
	*/
	virtual void Update() {}

	/*
	@brief オブジェクト描画前処理
	@details オブジェクト描画前に呼ばれるイベント（メッシュ描画より先）
	*/
	virtual void BeforeDraw() {}

	/*
	@brief オブジェクト描画処理
	@details オブジェクト描画の際に呼ばれるイベント
	*/
	virtual void Draw() {}

	/*
	@brief UI描画処理
	@details UI描画の際に呼ばれるイベント
	*/
	virtual void DrawUI() {}

	/*
	@brief 他のコリジョンと重なっているときに呼ばれるイベント
	*/
	virtual void OnTriggerStay(GameObject* other) {}

	/*
	@brief 他のコリジョンが入ってきたときのイベント
	*/
	virtual void OnTriggerEnter(GameObject* other) {}

	/*
	@brief 他のコリジョンが出たときのイベント
	*/
	virtual void OnTriggerExit(GameObject* other) {}

	/*
	@brief 他のオブジェクトが消されたときに呼ばれるイベント [強制]
	*/
	virtual void OnDestroyOtherObject(GameObject* other) {}

	/*
	@brief 他のコンポーネントが消されたときに呼ばれるイベント [強制]
	*/
	virtual void OnDestroyOtherComponent(Component* other) {}

	/*
	@brief 全コンポーネントを取得する
	@param[in] bOnlyActive : アクティブ状態のみ
	*/
	static std::vector<Component*> GetComponents(bool bOnlyActive = false) {
		std::vector<Component*> result;
		for (int i = 0; i < (int)m_pComponents.size(); i++)
		{
			if (!bOnlyActive || (bOnlyActive && m_pComponents.at(i)->enabled && m_pComponents.at(i)->gameObject->GetActive()))
				result.push_back(m_pComponents.at(i));
		}
		return result;
	}

	/*
	@brief 全コンポーネントから複数の特定コンポーネントを取得する
	@param[in] includeChild : 子クラスを含めるか
	@param[in] bOnlyActive : アクティブ状態のみ
	*/
	template<class T> static std::vector<T*> GetComponents(const bool& includeChild = false, const bool& bOnlyActive = false) {
		std::vector<T*> result;
		for (int i = 0; i < (int)m_pComponents.size(); i++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(m_pComponents.at(i)))
				{
					if (!bOnlyActive || (bOnlyActive && m_pComponents.at(i)->enabled && m_pComponents.at(i)->gameObject->GetActive()))
						result.push_back((T*)m_pComponents.at(i));
				}
			}
			else
			{ // 子を含まないとき
				if (typeid(T) == typeid(*m_pComponents.at(i)))
				{
					if (!bOnlyActive || (bOnlyActive && m_pComponents.at(i)->enabled && m_pComponents.at(i)->gameObject->GetActive()))
						result.push_back((T*)m_pComponents.at(i));
				}
			}
		}
		return result;
	}

	/*
	@brief コンポーネントが存在するか
	@param[in] pComponent : コンポーネントのポインタ
	*/
	static bool IsExist(Component* pComponent);

	/*
	@brief 指定クラスのコンポーネントか
	@param[in] pComponent : コンポーネントのポインタ
	*/
	template<class T> static bool IsClassType(Component* pComponent)
	{
		if (typeid(*pComponent) == typeid(T))
		{ // 同じ
			return true;
		}
		return false;
	}

	/*
	@brief コンポーネントのソート
	*/
	static void Sort();

	// @brief 有効状態か
	bool enabled;

	// @brief アタッチ先のゲームオブジェクト
	GameObject* gameObject;

	// @brief アタッチ先のトランスフォーム
	Transform* transform;
private:
	static std::vector<Component*> m_pComponents;
};

#endif // !_COMPONENT_H_
