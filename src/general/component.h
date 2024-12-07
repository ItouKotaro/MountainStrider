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
	@details オブジェクト描画前に呼ばれるイベント
	*/
	virtual void BeforeDraw() {}

	/*
	@brief オブジェクト描画後処理
	@details オブジェクト描画後に呼ばれるイベント
	*/
	virtual void AfterDraw() {}

	/*
	@brief オブジェクト描画処理
	@details オブジェクト描画の際に呼ばれるイベント
	*/
	virtual void Draw() {}

	/*
	@brief UI描画前処理
	@details UI描画前に呼ばれるイベント
	*/
	virtual void BeforeDrawUI() {}

	/*
	@brief UI描画後処理
	@details UI描画後に呼ばれるイベント
	*/
	virtual void AfterDrawUI() {}

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
		for (auto itr = m_pComponents.begin(); itr != m_pComponents.end(); itr++)
		{
			if (!bOnlyActive || (bOnlyActive && (*itr)->enabled && (*itr)->gameObject->GetActive()))
				result.push_back(*itr);
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
		for (auto itr = m_pComponents.begin(); itr != m_pComponents.end(); itr++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(*itr))
				{
					if (!bOnlyActive || (bOnlyActive && (*itr)->enabled && (*itr)->gameObject->GetActive()))
						result.push_back((T*)*itr);
				}
			}
			else
			{ // 子を含まないとき
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
	@brief ゲームオブジェクトにアタッチする
	@param[in] attachObj : アタッチ先のゲームオブジェクト
	*/
	void AttachGameObject(GameObject* attachObj);

	/*
	@brief コンポーネントのソート
	*/
	static void Sort();

	// @brief 有効状態か
	bool enabled;

	// @brief アタッチ先のトランスフォーム
	Transform* transform;

	// @brief アタッチ先のゲームオブジェクト
	GameObject* gameObject;
private:
	static std::vector<Component*> m_pComponents;
	bool m_attached;	// アタッチ済みか
};

#endif // !_COMPONENT_H_
