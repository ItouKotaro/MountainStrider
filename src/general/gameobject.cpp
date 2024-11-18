//=============================================================
//
// ゲームオブジェクト [gameobject.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "gameobject.h"
#include "component.h"
#include "internal/physics.h"
#include "component/3d/motion.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "component/3d/light.h"
#include "scene.h"
#include <fstream>
using namespace std;


// 静的メンバ変数の初期化
std::vector<GameObject*> GameObject::m_gameObjects;
const int GameObject::DEFAULT_PRIORITY = 6;

//=============================================================
// ゲームオブジェクトの更新処理
//=============================================================
void GameObject::UpdateAll()
{
	// 更新処理を行う
	for (int nCntObj = 0; nCntObj < (int)m_gameObjects.size(); nCntObj++)
	{
		if (m_gameObjects[nCntObj]->GetActive())
		{ // アクティブのとき

			// コンポーネントの更新処理を行う
			for (int i = 0; i < (int)m_gameObjects[nCntObj]->m_components.size(); i++)
			{
				if (m_gameObjects[nCntObj]->m_components[i]->enabled)
					m_gameObjects[nCntObj]->m_components[i]->Update();
			}
		}
	}
}

//=============================================================
// ゲームオブジェクトの描画前処理（メッシュより先に実行される）
//=============================================================
void GameObject::BeforeDrawAll()
{
	// オブジェクト描画前処理を行う
	for (int nCntObj = 0; nCntObj < (int)m_gameObjects.size(); nCntObj++)
	{
		if (m_gameObjects[nCntObj]->GetActive() && m_gameObjects[nCntObj]->GetVisible())
		{ // アクティブのとき

			// コンポーネントのオブジェクト描画前処理を行う
			for (int i = 0; i < (int)m_gameObjects[nCntObj]->m_components.size(); i++)
			{
				if (m_gameObjects[nCntObj]->m_components[i]->enabled)
					m_gameObjects[nCntObj]->m_components[i]->BeforeDraw();
			}
		}
	}
}

//=============================================================
// ゲームオブジェクトの描画処理
//=============================================================
void GameObject::DrawAll()
{
	// オブジェクト描画処理を行う
	for (int nCntObj = 0; nCntObj < (int)m_gameObjects.size(); nCntObj++)
	{
		if (m_gameObjects[nCntObj]->GetActive() && m_gameObjects[nCntObj]->GetVisible())
		{ // アクティブのとき

			// コンポーネントのオブジェクト描画処理を行う
			for (int i = 0; i < (int)m_gameObjects[nCntObj]->m_components.size(); i++)
			{
				if (m_gameObjects[nCntObj]->m_components[i]->enabled)
					m_gameObjects[nCntObj]->m_components[i]->Draw();
			}
		}
	}
}

//=============================================================
// ゲームオブジェクトのUI描画処理
//=============================================================
void GameObject::DrawUIAll()
{
	// UI描画処理を行う
	for (int nCntObj = 0; nCntObj < (int)m_gameObjects.size(); nCntObj++)
	{
		if (m_gameObjects[nCntObj]->GetActive() && m_gameObjects[nCntObj]->GetVisible())
		{ // アクティブのとき

			// コンポーネントのUI描画処理を行う
			for (int i = 0; i < (int)m_gameObjects[nCntObj]->m_components.size(); i++)
			{
				if (m_gameObjects[nCntObj]->m_components[i]->enabled)
					m_gameObjects[nCntObj]->m_components[i]->DrawUI();
			}
		}
	}
}

//=============================================================
// [GameObject] ゲームオブジェクトの検索
//=============================================================
GameObject* GameObject::Find(const std::string& name)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->GetName() == name)
		{	// 見つかったとき
			return m_gameObjects[i];
		}
	}
	return nullptr;
}

//=============================================================
// [GameObject] ゲームオブジェクトの検索
//=============================================================
GameObject* GameObject::Find(const std::string& name, const std::string& tag)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->GetName() == name && m_gameObjects[i]->GetTag() == tag)
		{	// 見つかったとき
			return m_gameObjects[i];
		}
	}
	return nullptr;
}

//=============================================================
// [GameObject] ゲームオブジェクトの子を取得する
//=============================================================
std::vector<GameObject*> GameObject::GetChildren(const bool& includeTreeChildren)
{
	std::vector<GameObject*> pChildren;
	for (int i = 0; i < (int)m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i]->transform->GetParent() == this->transform)
		{ // 子のとき
			pChildren.push_back(m_gameObjects[i]);

			// 子の子を取得する
			std::vector<GameObject*> pChildrenInChildren = m_gameObjects[i]->GetChildren(true);
			for (int n = 0; n < (int)pChildrenInChildren.size(); n++)
			{
				pChildren.push_back(pChildrenInChildren[n]);
			}
		}
	}
	return pChildren;
}

//=============================================================
// [GameObject] 名前から子のゲームオブジェクトを取得する
//=============================================================
GameObject* GameObject::FindNameChildren(const std::string& sName)
{
	// 子を取得する
	std::vector<GameObject*> pChildren = GetChildren();

	// 探す
	for (int i = 0; i < (int)pChildren.size(); i++)
	{
		if (pChildren[i]->GetName() == sName)
		{ // 名前が一致したとき
			return pChildren[i];
		}
	}

	// 名前が一致しなかったとき
	return nullptr;
}

//=============================================================
// [GameObject] タグから子のゲームオブジェクトを取得する
//=============================================================
GameObject* GameObject::FindTagChildren(const std::string& sTag)
{
	// 子を取得する
	std::vector<GameObject*> pChildren = GetChildren();

	// 探す
	for (int i = 0; i < (int)pChildren.size(); i++)
	{
		if (pChildren[i]->GetTag() == sTag)
		{ // タグが一致したとき
			return pChildren[i];
		}
	}

	// タグが一致しなかったとき
	return nullptr;
}

//=============================================================
// [GameObject] 削除
//=============================================================
void GameObject::Destroy(const bool& includeChild)
{
	if (GameObject::IsExist(this))
	{
		m_bDeathFlag = true;
	}

	// 子を含める場合
	if (includeChild)
	{
		std::vector<GameObject*> pChildren = this->GetChildren();
		for (unsigned int i = 0; i < pChildren.size(); i++)
		{
			pChildren[i]->Destroy(true);
		}
	}
}

//=============================================================
// [GameObject] すべて削除
//=============================================================
void GameObject::DestroyAll(bool bIncludeNot)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (bIncludeNot)
		{ // すべてのオブジェクトを破棄する
			m_gameObjects[i]->Destroy();
		}
		else
		{ // NotDestroyのタグが付いている場合はスルーする
			if (m_gameObjects[i]->GetTag() != TAG_NODESTROY)
			{ // タグがつけられていない場合
				m_gameObjects[i]->Destroy();
			}
		}
	}
}

//=============================================================
// [GameObject] 死亡フラグのついたオブジェクトを削除
//=============================================================
void GameObject::DestroyDeathFlag()
{
	for (int i = static_cast<int>(m_gameObjects.size()) - 1; i >= 0; i--)
	{
		if (m_gameObjects[i]->m_bDeathFlag)
		{ // 死亡フラグがついているとき
			delete m_gameObjects[i];
		}
	}
}

//=============================================================
// [GameObject] コンポーネントの削除
//=============================================================
void GameObject::Destroy(Component* component)
{
	if (component == nullptr)
		return;

	for (int i = 0; i < (int)m_components.size(); i++)
	{
		if (m_components[i] == component)
		{
			// 終了処理
			m_components[i]->Uninit();
			m_components.erase(m_components.begin() + i);
			
			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
			return;
		}
	}
}

//=============================================================
// [GameObject] コンストラクタ
//=============================================================
GameObject::GameObject(const std::string& name, const std::string& tag)
{
	m_active = true;
	m_visible = true;
	SetName(name);
	SetTag(tag);
	transform = new Transform();
	m_bDeathFlag = false;

	// ゲームオブジェクトに登録する
	m_gameObjects.push_back(this);

	// 優先度を設定する
	SetPriority(DEFAULT_PRIORITY);
}

//=============================================================
// [GameObject] デストラクタ
//=============================================================
GameObject::~GameObject()
{
	// ゲームオブジェクト消失イベント
	for (int nCntObj = 0; nCntObj < (int)m_gameObjects.size(); nCntObj++)
	{
		if (m_gameObjects[nCntObj] == this) { continue; }

		// コンポーネントのイベント処理を行う
		for (int i = 0; i < (int)m_gameObjects[nCntObj]->m_components.size(); i++)
		{
			if (m_gameObjects[nCntObj]->m_components[i] != nullptr)
			{
				m_gameObjects[nCntObj]->m_components[i]->OnDestroyOtherObject(this);
			}
		}
	}

	// 含まれるコンポーネントを削除する
	for (int i = 0; i < (int)m_components.size(); i++)
	{
		if (m_components[i] != nullptr)
		{
			// コンポーネント消失イベント
			for (int nCntObj = 0; nCntObj < (int)m_gameObjects.size(); nCntObj++)
			{
				if (m_gameObjects[nCntObj] == this) { continue; }
				// コンポーネントのイベント処理を行う
				for (int n = 0; n < (int)m_gameObjects[nCntObj]->m_components.size(); n++)
				{
					if (m_gameObjects[nCntObj]->m_components[n] != nullptr)
					{
						m_gameObjects[nCntObj]->m_components[n]->OnDestroyOtherComponent(m_components[i]);
					}
				}
			}

			m_components[i]->Uninit();
			delete m_components[i];
			m_components[i] = nullptr;
		}
	}

	// このオブジェクトを親として設定しているオブジェクトの解除
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i] == this) { continue; }

		if (m_gameObjects[i]->transform->GetParent() == transform)
		{
			m_gameObjects[i]->transform->SetParent(nullptr);		// 親状態の解除
		}
	}

	// トランスフォームの破棄
	if (transform)
	{
		delete transform;
		transform = nullptr;
	}

	// データフィールドの削除
	CDataField::Destroy(this);

	// コリジョンを削除する
	CCollision::RemoveCollision(this);

	// ゲームオブジェクトの登録を解除する
	for (int i = 0; i < (int)m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i] == this)
		{
			m_gameObjects.erase(m_gameObjects.begin() + i);
			break;
		}
	}
}

//=============================================================
// [GameObject] 優先度の設定
//=============================================================
void GameObject::SetPriority(int nPriority)
{
	m_nPriority = nPriority;

	// ゲームオブジェクトを優先度順にソートする
	std::sort(m_gameObjects.begin(), m_gameObjects.end(), [](GameObject* obj1, GameObject* obj2) {return obj1->GetPriority() < obj2->GetPriority(); });
	
	// コンポーネントをソートする
	Component::Sort();
}

//=============================================================
// [GameObject] ゲームオブジェクトが存在するか
//=============================================================
bool GameObject::IsExist(GameObject* pGameObject)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i] == pGameObject)
		{	// 存在しているとき
			return true;
		}
	}
	return false;
}

//=============================================================
// [GameObject] プレハブの読み込み
//=============================================================
GameObject* GameObject::LoadPrefab(const std::string& sPath, Transform transform)
{
	// jsonファイルを読み込む
	ifstream ifs(sPath.c_str());

	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		return nullptr;
	}
	
	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jPrefab = json::parse(sInputData);

	// 親プレハブオブジェクトの作成
	GameObject* pPrefabObject = new GameObject();
	pPrefabObject->transform->SetPos(transform.GetPos());
	pPrefabObject->transform->SetRot(transform.GetRot());
	pPrefabObject->transform->SetScale(transform.GetScale());

	if (jPrefab.contains("prefab"))
	{ // プレハブが含まれているとき（必須）
		pPrefabObject->SetName(jPrefab["prefab"]["name"]);
		pPrefabObject->SetTag(jPrefab["prefab"]["tag"]);
		pPrefabObject->transform->Translate(jPrefab["prefab"]["transform"]["position"][0], jPrefab["prefab"]["transform"]["position"][1], jPrefab["prefab"]["transform"]["position"][2]);
		pPrefabObject->transform->Rotate(jPrefab["prefab"]["transform"]["rotation"][0], jPrefab["prefab"]["transform"]["rotation"][1], jPrefab["prefab"]["transform"]["rotation"][2]);
		//pPrefabObject->transform->SetScale(jPrefab["prefab"]["transform"]["scale"][0], jPrefab["prefab"]["transform"]["scale"][1], jPrefab["prefab"]["transform"]["scale"][2]);

		// コリジョンのとき
		if (jPrefab["prefab"].contains("collision"))
		{ // コリジョン情報が含まれているとき
			CCollision::Create(pPrefabObject);

			// リジッドボディか
			if (jPrefab["prefab"]["rigidbody"])
			{ // リジッドボディが有効の時
				pPrefabObject->AddComponent<CRigidBody>();
			}

			// トリガーか
			if (jPrefab["prefab"].contains("trigger"))
			{
				CCollision::GetCollision(pPrefabObject)->IsTrigger(jPrefab["prefab"]["trigger"]);
			}

			// コリジョン情報
			for (unsigned int i = 0; i < jPrefab["prefab"]["collision"].size(); i++)
			{
				// コリジョンのタイプを取得する
				string sCollisionType = jPrefab["prefab"]["collision"][i]["type"];

				// タイプごとに処理する
				CCollider* pCol = nullptr;
				if (sCollisionType == "box")
				{ // ボックスのとき
					pCol = pPrefabObject->AddComponent<CBoxCollider>();
					CBoxCollider* pBoxCol = (CBoxCollider*)pCol;

					// サイズ
					pBoxCol->SetSize({
						jPrefab["prefab"]["collision"][i]["size"][0],
						jPrefab["prefab"]["collision"][i]["size"][1],
						jPrefab["prefab"]["collision"][i]["size"][2]
						});
					pBoxCol->SetSize(pBoxCol->GetSize() /2);
				}
				else if (sCollisionType == "sphere")
				{ // 球のとき
					pCol = pPrefabObject->AddComponent<CSphereCollider>();
					CSphereCollider* pSphereCol = (CSphereCollider*)pCol;

					// 半径
					pSphereCol->SetRadius(jPrefab["prefab"]["collision"][i]["radius"]);
				}
				else if (sCollisionType == "capsule")
				{ // カプセルのとき
					pCol = pPrefabObject->AddComponent<CCapsuleCollider>();
					CCapsuleCollider* pCapsuleCol = (CCapsuleCollider*)pCol;

					// 半径
					pCapsuleCol->SetRadius(jPrefab["prefab"]["collision"][i]["radius"]);

					// 高さ
					pCapsuleCol->SetHeight(jPrefab["prefab"]["collision"][i]["height"]);
				}
				else if (sCollisionType == "cylinder")
				{ // 円柱のとき
					pCol = pPrefabObject->AddComponent<CCylinderCollider>();
					CCylinderCollider* pCylinderCol = (CCylinderCollider*)pCol;

					// 半径
					pCylinderCol->SetRadius(jPrefab["prefab"]["collision"][i]["radius"]);

					// 高さ
					pCylinderCol->SetHeight(jPrefab["prefab"]["collision"][i]["height"]);
				}

				// ---------------------------------------------------
				// 全般設定
				
				// オフセット
				if (jPrefab["prefab"]["collision"][i].contains("offset"))
				{
					pCol->SetOffset({
						jPrefab["prefab"]["collision"][i]["offset"][0],
						jPrefab["prefab"]["collision"][i]["offset"][1],
						jPrefab["prefab"]["collision"][i]["offset"][2]
						});
				}

				// 回転
				if (jPrefab["prefab"]["collision"][i].contains("rotation"))
				{
					pCol->SetRot({
						jPrefab["prefab"]["collision"][i]["rotation"][0],
						jPrefab["prefab"]["collision"][i]["rotation"][1],
						jPrefab["prefab"]["collision"][i]["rotation"][2]
						});
				}
			}
		}
	}
	else
	{ // 含まれていない場合は失敗
		pPrefabObject->Destroy();
		return nullptr;
	}

	// オブジェクト情報の作成・読み込み
	vector<GameObject*> vecObjects;		// オブジェクト保持用
	if (jPrefab.contains("object"))
	{ // オブジェクトが含まれているとき
		for (unsigned int i = 0; i < jPrefab["object"].size(); i++)
		{
			GameObject* pObject = new GameObject();

			// 情報の取得
			pObject->SetName(jPrefab["object"][to_string(i)]["name"]);
			pObject->SetTag(jPrefab["object"][to_string(i)]["tag"]);
			pObject->transform->SetPos(jPrefab["object"][to_string(i)]["transform"]["position"][0],
				jPrefab["object"][to_string(i)]["transform"]["position"][1],
				jPrefab["object"][to_string(i)]["transform"]["position"][2]);
			pObject->transform->SetRot(jPrefab["object"][to_string(i)]["transform"]["rotation"][0],
				jPrefab["object"][to_string(i)]["transform"]["rotation"][1],
				jPrefab["object"][to_string(i)]["transform"]["rotation"][2]);
			pObject->transform->SetScale(jPrefab["object"][to_string(i)]["transform"]["scale"][0],
				jPrefab["object"][to_string(i)]["transform"]["scale"][1],
				jPrefab["object"][to_string(i)]["transform"]["scale"][2]);

			if (jPrefab["object"][to_string(i)].contains("model"))
			{ // モデル情報が含まれているとき（任意）
				// 影描画
				bool bDrawShadow = true;
				if (jPrefab["object"][to_string(i)].contains("shadow"))
				{
					bDrawShadow = jPrefab["object"][to_string(i)]["shadow"];
				}

				if (jPrefab["object"][to_string(i)]["model"] != "none")
				{
					CMesh* pMesh = pObject->AddComponent<CMesh>();
					pMesh->LoadMeshX(jPrefab["object"][to_string(i)]["model"], bDrawShadow);
				}
			}

			// 特殊オブジェクトのとき
			if (jPrefab["object"][to_string(i)].contains("light"))
			{ // ライト情報が含まれているとき
				CLight* pLight = pObject->AddComponent<CLight>();
				pLight->SetIntensity(jPrefab["object"][to_string(i)]["light"]["intensity"]);
				pLight->SetColor(D3DXCOLOR(
					jPrefab["object"][to_string(i)]["light"]["color"][0],
					jPrefab["object"][to_string(i)]["light"]["color"][1],
					jPrefab["object"][to_string(i)]["light"]["color"][2],
					jPrefab["object"][to_string(i)]["light"]["color"][3]));
			}

			// コリジョンのとき
			if (jPrefab["object"][to_string(i)].contains("collision"))
			{ // オブジェクトにコリジョン情報が含まれているか
				CCollision::Create(pObject);
				auto jCol = jPrefab["object"][to_string(i)]["collision"]; // 省略

				// トリガーか
				if (jPrefab["object"][to_string(i)].contains("trigger"))
				{
					CCollision::GetCollision(pObject)->IsTrigger(jPrefab["object"][to_string(i)]["trigger"]);
				}

				// 摩擦情報
				if (jPrefab["object"][to_string(i)].contains("friction"))
				{
					CCollision::GetCollision(pObject)->SetFriction(jPrefab["object"][to_string(i)]["friction"]);
				}

				// コリジョン情報
				for (unsigned int nCntCol = 0; nCntCol < jCol.size(); nCntCol++)
				{
					// コリジョンのタイプを取得する
					string sCollisionType = jPrefab["object"][to_string(i)]["collision"][nCntCol]["type"];

					// タイプごとに処理する
					CCollider* pCol = nullptr;
					if (sCollisionType == "box")
					{ // ボックスのとき
						pCol = pObject->AddComponent<CBoxCollider>();
						CBoxCollider* pBoxCol = (CBoxCollider*)pCol;

						// サイズ
						pBoxCol->SetSize({
							jCol[nCntCol]["size"][0],
							jCol[nCntCol]["size"][1],
							jCol[nCntCol]["size"][2]
							});
						pBoxCol->SetSize(pBoxCol->GetSize() / 2);
					}
					else if (sCollisionType == "sphere")
					{ // 球のとき
						pCol = pObject->AddComponent<CSphereCollider>();
						CSphereCollider* pSphereCol = (CSphereCollider*)pCol;

						// 半径
						pSphereCol->SetRadius(jCol[nCntCol]["radius"]);
					}
					else if (sCollisionType == "capsule")
					{ // カプセルのとき
						pCol = pObject->AddComponent<CCapsuleCollider>();
						CCapsuleCollider* pCapsuleCol = (CCapsuleCollider*)pCol;

						// 半径
						pCapsuleCol->SetRadius(jCol[nCntCol]["radius"]);

						// 高さ
						pCapsuleCol->SetHeight(jCol[nCntCol]["height"]);
					}
					else if (sCollisionType == "cylinder")
					{ // 円柱のとき
						pCol = pObject->AddComponent<CCylinderCollider>();
						CCylinderCollider* pCylinderCol = (CCylinderCollider*)pCol;

						// 半径
						pCylinderCol->SetRadius(jCol[nCntCol]["radius"]);

						// 高さ
						pCylinderCol->SetHeight(jCol[nCntCol]["height"]);
					}

					// ---------------------------------------------------
					// 全般設定

					// オフセット
					if (jCol[nCntCol].contains("offset"))
					{
						pCol->SetOffset({
							jCol[nCntCol]["offset"][0],
							jCol[nCntCol]["offset"][1],
							jCol[nCntCol]["offset"][2]
							});
					}

					// 回転
					if (jCol[nCntCol].contains("rotation"))
					{
						pCol->SetRot({
							jCol[nCntCol]["rotation"][0],
							jCol[nCntCol]["rotation"][1],
							jCol[nCntCol]["rotation"][2]
							});
					}
				}
			}

			// オブジェクト情報を保持する
			vecObjects.push_back(pObject);
		}
	}

	// 親子関係の読み込み
	if (jPrefab.contains("parent"))
	{ // 親子情報が含まれているとき
		for (unsigned int i = 0; i < jPrefab["parent"].size(); i++)
		{
			// 親の設定
			if (0 <= jPrefab["parent"][to_string(i)] &&
				jPrefab["parent"][to_string(i)] < vecObjects.size())
			{ // 指定のオブジェクトで設定されているとき
				vecObjects[i]->SetParent(vecObjects[jPrefab["parent"][to_string(i)]]);
			}
			else
			{ // 親プレハブのとき
				vecObjects[i]->SetParent(pPrefabObject);
			}
		}
	}

	// コリジョン情報を更新する
	for (unsigned int i = 0; i < vecObjects.size(); i++)
	{
		if (CCollision::GetCollision(vecObjects[i]) != nullptr)
		{
			CCollision::GetCollision(vecObjects[i])->UpdateCollision();
		}
	}

	// 質量と摩擦の設定
	if (jPrefab["prefab"].contains("collision"))
	{// コリジョン情報が含まれているとき
		// 質量
		if (jPrefab["prefab"].contains("mass"))
		{
			CCollision::GetCollision(pPrefabObject)->SetMass(jPrefab["prefab"]["mass"]);
		}

		// 摩擦
		if (jPrefab["prefab"].contains("friction"))
		{
			CCollision::GetCollision(pPrefabObject)->SetFriction(jPrefab["prefab"]["friction"]);
		}
	}

	// モーション情報の読み込み
	if (jPrefab.contains("motion"))
	{ // モーション情報が含まれているとき
		// コンポーネントを追加
		CMotionManager* pMotionManager = pPrefabObject->AddComponent<CMotionManager>((int)vecObjects.size());

		// パーツにアタッチする
		for (unsigned int i = 0; i < vecObjects.size(); i++)
		{
			pMotionManager->SetParts(i, vecObjects[i]);
			pMotionManager->SetOffset(i, *vecObjects[i]->transform);
		}

		for (auto motion = jPrefab["motion"].items().begin(); motion != jPrefab["motion"].items().end(); motion++)
		{
			// モーションコンテンツの作成
			string sContentName = motion.key();					// モーション名を取得
			CMotionContents* pContent = pMotionManager->AddContents(sContentName);		// コンテンツを作成

			// パーツごとにキーの追加
			for (auto motionParts = jPrefab["motion"][sContentName].items().begin(); motionParts != jPrefab["motion"][sContentName].items().end(); motionParts++)
			{
				if (motionParts.key() == "loop")
				{ // 繰り返し設定
					pContent->SetLoop(jPrefab["motion"][sContentName]["loop"]);
				}
				else if (motionParts.key() == "max_frame")
				{ // 最大フレーム設定
					pContent->SetMaxFrame(jPrefab["motion"][sContentName]["max_frame"]);
				}
				else
				{ // キーの追加
					int nPartsID = atoi(motionParts.key().c_str());		// パーツIDの取得

					for (auto motionKey = jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"].items().begin(); motionKey != jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"].items().end(); motionKey++)
					{
						int nKey = atoi(motionKey.key().c_str());			// キーの取得

						// キーの追加
						pContent->AddKey(nPartsID, nKey,
							{ 
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["position"][0],
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["position"][1],
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["position"][2] 
							},
							{
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["rotation"][0],
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["rotation"][1],
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["rotation"][2]
							},
							{
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["scale"][0],
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["scale"][1],
							jPrefab["motion"][sContentName][to_string(nPartsID)]["keys"][to_string(nKey)]["scale"][2]
							}
						);
					}
				}
			}
		}

		// 初動モーション
		if (jPrefab.contains("default-motion"))
		{ // 設定されているとき
			pMotionManager->Play(jPrefab["default-motion"]);
		}
	}

	// データフィールドの読み込み
	if (jPrefab.contains("data-field"))
	{ // データフィールド情報が含まれているとき
		for (int i = 0; i < (int)jPrefab["data-field"].size(); i++)
		{
			// データフィールドの取得
			CDataField* pDataField = CDataField::GetDataField(pPrefabObject);

			// 名前と型
			string sNameDF = jPrefab["data-field"][i]["name"];
			string sTypeDF = jPrefab["data-field"][i]["type"];

			// 各型に合った処理
			if (sTypeDF == "int")
			{
				CDF_Int* pIntDF = new CDF_Int();
				pIntDF->SetName(sNameDF);
				pIntDF->SetValue(jPrefab["data-field"][i]["value"]);
				pDataField->AddData(pIntDF);
			}
			if (sTypeDF == "float")
			{
				CDF_Float* pFloatDF = new CDF_Float();
				pFloatDF->SetName(sNameDF);
				pFloatDF->SetValue(jPrefab["data-field"][i]["value"]);
				pDataField->AddData(pFloatDF);
			}
			if (sTypeDF == "bool")
			{
				CDF_Bool* pBoolDF = new CDF_Bool();
				pBoolDF->SetName(sNameDF);
				pBoolDF->SetValue(jPrefab["data-field"][i]["value"]);
				pDataField->AddData(pBoolDF);
			}
			if (sTypeDF == "string")
			{
				CDF_String* pStringDF = new CDF_String();
				pStringDF->SetName(sNameDF);
				pStringDF->SetValue(jPrefab["data-field"][i]["value"]);
				pDataField->AddData(pStringDF);
			}
			if (sTypeDF == "vector3")
			{
				CDF_Vector3* pVector3DF = new CDF_Vector3();
				pVector3DF->SetName(sNameDF);
				pVector3DF->SetValue({ jPrefab["data-field"][i]["value"][0], jPrefab["data-field"][i]["value"][1], jPrefab["data-field"][i]["value"][2] });
				pDataField->AddData(pVector3DF);
			}
			if (sTypeDF == "vector2")
			{
				CDF_Vector2* pVector2DF = new CDF_Vector2();
				pVector2DF->SetName(sNameDF);
				pVector2DF->SetValue({ jPrefab["data-field"][i]["value"][0], jPrefab["data-field"][i]["value"][1]});
				pDataField->AddData(pVector2DF);
			}
		}
	}

	// ファイルを閉じる
	ifs.close();

	return pPrefabObject;
}