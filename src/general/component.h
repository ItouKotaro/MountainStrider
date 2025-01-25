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
#include "shader.h"

// コンポーネントクラス
class Component
{
public:
	Component() { 
		enabled = true;
		m_attached = false;
		gameObject = nullptr;
		transform = nullptr;
		m_shader = nullptr;
		m_pass = 0;
	}
	virtual ~Component() {}

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

		auto gameObjects = GameObject::GetAllGameObjects();
		for (auto objItr = gameObjects.begin(); objItr != gameObjects.end(); objItr++)
		{
			auto components = (*objItr)->GetComponents();
			for (auto itr = components.begin(); itr != components.end(); itr++)
			{
				if (!bOnlyActive || (bOnlyActive && (*itr)->enabled && (*itr)->gameObject->GetActive()))
					result.push_back(*itr);
			}
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

		auto components = Component::GetComponents();
		for (auto itr = components.begin(); itr != components.end(); itr++)
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
	@brief シェーダーの命令
	@param[in] shader : シェーダー
	@param[in] pass : パス
	*/
	void SetShader(Shader* shader, UINT pass) 
	{
		m_shader = shader;
		m_pass = pass;
	}

	// @brief シェーダー情報
	struct ShaderInfo
	{
		Shader* shader;
		UINT pass;
	};

	/*
	@brief シェーダーの命令
	@param[in] shaderInfo : シェーダー情報（GetShaderInfoで取得可能）
	*/
	void SetShader(ShaderInfo shaderInfo)
	{
		m_shader = shaderInfo.shader;
		m_pass = shaderInfo.pass;
	}

	/*
	@brief ゲームオブジェクトにアタッチする
	@param[in] attachObj : アタッチ先のゲームオブジェクト
	*/
	void AttachGameObject(GameObject* attachObj);

	// @brief 有効状態か
	bool enabled;

	// @brief アタッチ先のゲームオブジェクト
	GameObject* gameObject;

	// @brief アタッチ先のトランスフォーム
	Transform* transform;

protected:
	// @brief シェーダーの取得
	ShaderInfo GetShaderInfo()
	{
		ShaderInfo info;
		info.shader = m_shader;
		info.pass = m_pass;
		return info;
	}

	// @brief パスの開始
	void BeginPass();

	// @brief パスの終了
	void EndPass();

	// @brief シェーダーに情報を渡す
	void SetParam(Shader::ParamData data);

	// @brief シェーダーが有効か
	bool IsEnabledShader() { return m_shader != nullptr; }

private:
	// @brief シェーダー類
	Shader* m_shader;
	UINT m_pass;

	// @brief アタッチ済みか
	bool m_attached;
};


// 単一で動作するコンポーネント型を提供します
// 各イベントは自前で呼び出す必要があります
template<class T> class SingleComponent : public T
{
public:
	// 単一コンポーネント
	template<typename ...Args> SingleComponent(Args... args) : T(args...)
	{
		InitTransform();
	}
	~SingleComponent()
	{
		delete this->transform;
		this->transform = nullptr;
	}

	// 親を設定する
	void SetParent(GameObject* parent) { this->transform->SetParent(parent->transform); }
	void SetParent(Transform* parent) { this->transform->SetParent(parent); }
private:
	void InitTransform() { this->transform = new Transform(); }
};

#endif // !_COMPONENT_H_
