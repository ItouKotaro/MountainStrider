//=============================================================
//
// ゲームオブジェクト [gameobject.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "transform.h"
#include "data_field.h"

// マクロ定義
#define TAG_NODESTROY	"NoDestroy"	// シーンを変更しても削除しない

class Component;

// ゲームオブジェクトクラスの定義
class GameObject
{
public:
	/*
	@brief ゲームオブジェクトを作成する
	*/
	GameObject(const std::string& name = "", const std::string& tag = "");
	~GameObject();

	/*
	@brief ゲームオブジェクトの名前を取得する
	@return 名前
	*/
	std::string& GetName() { return m_name; }

	/*
	@brief ゲームオブジェクトのタグ名を取得する
	@return タグ名
	*/
	std::string& GetTag() { return m_tag; }

	/*
	@brief ゲームオブジェクトの優先度を取得する
	@return 優先度
	*/
	int GetPriority() { return m_nPriority; }

	/*
	@brief ゲームオブジェクトのアクティブ状態を取得する
	@retval アクティブ=true, 非アクティブ=false
	*/
	bool GetActive() { return m_active; }

	/*
	@brief ゲームオブジェクトの表示状態を取得する
	@retval 表示=true, 非表示=false
	*/
	bool GetVisible() { return m_visible; }

	/*
	@brief ゲームオブジェクトの名前を設定する
	@param[in] name : 名前
	*/
	void SetName(std::string name) { m_name = name; }

	/*
	@brief ゲームオブジェクトのタグを設定する
	@param[in] tag : タグ名
	*/
	void SetTag(std::string tag) { m_tag = tag; }

	/*
	@brief ゲームオブジェクトの優先度を設定する
	@param[in] nPriority : 優先度
	*/
	void SetPriority(int nPriority);

	/*
	@brief ゲームオブジェクトのアクティブ状態を設定する
	@param[in] bActive : アクティブ状態にするか
	*/
	void SetActive(bool bActive) { m_active = bActive; }

	/*
	@brief ゲームオブジェクトの表示状態を設定する
	@param[in] bActive : 表示状態にするか
	*/
	void SetVisible(bool bVisible) { m_visible = bVisible; }

	/*
	@brief ゲームオブジェクトの親を設定する
	@param[in] gameObject : 親のゲームオブジェクト
	*/
	void SetParent(GameObject* gameObject) { transform->SetParent(gameObject->transform); }

	/*
	@brief ゲームオブジェクトの親を取得する
	@return GameObject : 親のゲームオブジェクト
	*/
	GameObject* GetParent() {
		for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
		{
			if ((*itr)->transform == this->transform->GetParent())
			{ // 親と一致するとき
				return *itr;
			}
		}
		return nullptr;
	}

	static void UpdateAll();
	static void DrawAll();
	static void DrawUIAll();

	/*
	@brief ゲームオブジェクトを検索する
	@param[in] name : ゲームオブジェクト名
	@return GameObjectのポインタ
	*/
	static GameObject* Find(const std::string& name);

	/*
	@brief ゲームオブジェクトを検索する
	@param[in] name : ゲームオブジェクト名
	@param[in] tag : タグ
	@return GameObjectのポインタ
	*/
	static GameObject* Find(const std::string& name, const std::string& tag);

	/*
	@brief 子のゲームオブジェクトを取得する
	@param[in] includeTreeChildren : 子の子以上を含めるか
	@return 子のゲームオブジェクト配列
	*/
	std::vector<GameObject*> GetChildren(const bool& includeTreeChildren = false);

	/*
	@brief 名前から子のゲームオブジェクトを取得する
	@return 子のゲームオブジェクト
	*/
	GameObject* FindNameChildren(const std::string& sName);

	/*
	@brief タグから子のゲームオブジェクトを取得する
	@return 子のゲームオブジェクト
	*/
	GameObject* FindTagChildren(const std::string& sTag);

	/*
	@brief コンポーネントを追加する
	*/
	template<class T, typename ...Args > inline T* AddComponent(Args... args) {
		// コンポーネントを作成する
		T* component = new T(args...);

		// ゲームオブジェクトにアタッチする
		component->AttachGameObject(this);

		// コンポーネントを追加する
		m_components.push_back(component);

		// コンポーネントの初期化処理を実行する
		component->Init();

		// 作成したコンポーネントを返す
		return component;
	}

	/*
	@brief コンポーネントを取得する
	@param[in] includeChild : 子クラスを含めるか
	*/
	template<class T> inline T* GetComponent(const bool& includeChild = false) {
		for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(*itr))
					return (T*)*itr;
			}
			else
			{ // 子を含まないとき
				if (typeid(T) == typeid(**itr))
					return (T*)*itr;
			}
		}
		return nullptr;
	}

	/*
	@brief 複数のコンポーネントを取得する
	@param[in] includeChild : 子クラスを含めるか
	*/
	template<class T> inline std::vector<T*> GetComponents(const bool& includeChild = false) {
		std::vector<T*> result;
		for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
		{
			if (includeChild)
			{ // 子を含むとき
				if (T* pComp = dynamic_cast<T*>(*itr))
					result.push_back((T*)*itr);
			}
			else
			{ // 子を含まないとき
				if (typeid(T) == typeid(**itr))
					result.push_back((T*)*itr);
			}
		}
		return result;
	}

	/*
	@brief すべてのコンポーネントを取得する
	*/
	std::vector<Component*>& GetComponents() {
		return m_components;
	}

	/*
	@brief すべてのゲームオブジェクトを取得する
	*/
	static std::vector<GameObject*> GetAllGameObjects() {
		return m_gameObjects;
	}

	/*
	@brief オブジェクト数を取得する
	*/
	static int GetNumObjects() { return (int)m_gameObjects.size(); }

	/*
	@brief ゲームオブジェクトを破棄する
	*/
	void Destroy(const bool& includeChild = false);

	/*
	@brief コンポーネントを破棄する
	@param[in] component : コンポーネントのポインタ
	*/
	void Destroy(Component* component);

	/*
	@brief すべてのコンポーネントを破棄する
	@param[in] bIncludeNot : 削除対象にNoDestroyを含むか
	*/
	static void DestroyAll(bool bIncludeNot = false);

	/*
	@brief 死亡フラグのついたオブジェクトを破棄する
	*/
	static void DestroyDeathFlag();

	/*
	@brief ゲームオブジェクトが存在するか
	@param[in] pGameObject : ゲームオブジェクトのポインタ
	*/
	static bool IsExist(GameObject* pGameObject);

	/*
	@brief プレハブの読み込み
	@param[in] sPath : プレハブのパス
	*/
	static GameObject* LoadPrefab(const std::string& sPath, Transform transform = Transform());

	// @brief トランスフォーム情報
	Transform* transform;

	static const int DEFAULT_PRIORITY;		// デフォルトの優先度
private:
	bool m_visible;														// 表示
	bool m_active;														// アクティブ
	std::string m_name;												// 名前
	std::string m_tag;													// タグ
	int m_nPriority;														// 優先度
	bool m_bDeathFlag;												// 死亡フラグ
	std::vector<Component*> m_components;				// コンポーネント
	static std::vector<GameObject*> m_gameObjects;	// すべてのゲームオブジェクトのポインタを管理
};

#endif // !_GAMEOBJECT_H_