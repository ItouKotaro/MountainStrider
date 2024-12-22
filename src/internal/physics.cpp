//=============================================================
//
// 3D物理 [physics.cpp]
// Author: Ito Kotaro
// 
// 説明:
// 物理エンジンを使用した処理
//
//=============================================================
#include "physics.h"
#include "manager.h"
#include "component/3d/collision.h"
#include "component/3d/line.h"
#include "benlib.h"

// 静的メンバの初期化
std::vector<CCollision*> CCollision::m_collisions;
btDiscreteDynamicsWorld* CPhysics::m_dynamicsWorld = nullptr;
btAlignedObjectArray<btCollisionShape*> CPhysics::m_collisionShapes;
CPhysicsDebugDraw* CPhysics::m_debugDraw = nullptr;

// 設定
constexpr bool COLLISION_WIREFRAME = false;

//=============================================================
// [CPhysicsDebugDraw] コンストラクタ
//=============================================================
CPhysicsDebugDraw::CPhysicsDebugDraw()
{
	m_pLineObject = new GameObject();		// ライン描画用のゲームオブジェクトを作成
	m_pLineObject->SetName("PhysicsDebugLines");
	m_pLineObject->SetTag(TAG_NODESTROY);
}

//=============================================================
// [CPhysicsDebugDraw] ラインの描画
//=============================================================
void CPhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
#if _DEBUG
	// ラインのコンポーネントを追加する
	for (unsigned int i = 0; i < m_pLines.size(); i++)
	{
		if (!m_pLines[i]->enabled)
		{
			m_pLines[i]->SetLine({ from.getX(), from.getY(), from.getZ() }, { to.getX(), to.getY(), to.getZ() }, { color.getX(), color.getY(), color.getZ(), 1.0f });
			m_pLines[i]->enabled = true;
			return;
		}
	}

	// ラインがすべて埋まっているとき（追加する）
	CLine* pLine = m_pLineObject->AddComponent<CLine>();
	pLine->SetLine({ from.getX(), from.getY(), from.getZ() }, { to.getX(), to.getY(), to.getZ() }, { color.getX(), color.getY(), color.getZ(), 1.0f });
	pLine->enabled = true;
	m_pLines.push_back(pLine);
#endif
}

//=============================================================
// [CPhysicsDebugDraw] ラインのクリア
//=============================================================
void CPhysicsDebugDraw::clearLines()
{
#if _DEBUG
	for (unsigned int i = 0; i < m_pLines.size(); i++)
	{
		m_pLines[i]->enabled = false;
	}
#endif
}

//=============================================================
// [CPhysics] コンストラクタ
//=============================================================
CPhysics::CPhysics() : m_isUpdatePhysics(true)
{

}

//=============================================================
// [CPhysics] デストラクタ
//=============================================================
CPhysics::~CPhysics()
{

}

//=============================================================
// [CPhysics] 初期化
//=============================================================
void CPhysics::Init()
{
	// コリジョンの設定
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	// ブロードフェーズ法の設定
	m_overlappingPairCache = new btDbvtBroadphase();

	// 拘束のソルバ設定
	m_solver = new btSequentialImpulseConstraintSolver;

	// Bulletワールドを作成
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

	// ゴースト
	m_ghostPairCollback = new btGhostPairCallback();
	m_dynamicsWorld->getPairCache()->setInternalGhostPairCallback(m_ghostPairCollback);

	// デバッグ
	m_debugDraw = new CPhysicsDebugDraw();
	m_debugDraw->setDebugMode(COLLISION_WIREFRAME ? btIDebugDraw::DBG_DrawWireframe : btIDebugDraw::DBG_NoDebug);
	m_dynamicsWorld->setDebugDrawer(m_debugDraw);

	// アクションインターフェイス
	m_actionInterface = new CActionInterface();
	m_dynamicsWorld->addAction(m_actionInterface);

	// 重力を設定
	m_dynamicsWorld->setGravity(btVector3(0, -90, 0));
}

//=============================================================
// [CPhysics] 終了
//=============================================================
void CPhysics::Uninit()
{
	// 形状を削除する
	for (int i = 0; i < m_collisionShapes.size(); i++)
	{
		btCollisionShape* shape = m_collisionShapes[i];
		m_collisionShapes[i] = nullptr;
		delete shape;
	}

	// 拘束を削除する
	for (int i = 0; i < m_dynamicsWorld->getNumConstraints(); i++)
	{
		btTypedConstraint* pConstraint = m_dynamicsWorld->getConstraint(i);
		m_dynamicsWorld->removeConstraint(pConstraint);
		delete pConstraint;
	}

	// アクションインターフェイスの破棄
	if (m_actionInterface != nullptr)
	{
		m_dynamicsWorld->removeAction(m_actionInterface);		// ワールドから削除する
		delete m_actionInterface;
		m_actionInterface = nullptr;
	}

	// 物理ワールドの破棄
	if (m_dynamicsWorld != nullptr)
	{
		delete m_dynamicsWorld;
		m_dynamicsWorld = nullptr;
	}

	// ソルバの破棄
	if (m_solver != nullptr)
	{
		delete m_solver;
		m_solver = nullptr;
	}

	// オーバーラッピングペアキャッシュの破棄
	if (m_overlappingPairCache != nullptr)
	{
		delete m_overlappingPairCache;
		m_overlappingPairCache = nullptr;
	}

	// ディスパッチャーの破棄
	if (m_dispatcher != nullptr)
	{
		delete m_dispatcher;
		m_dispatcher = nullptr;
	}

	// コリジョン設定の破棄
	if (m_collisionConfiguration != nullptr)
	{
		delete m_collisionConfiguration;
		m_collisionConfiguration = nullptr;
	}

	// ゴーストコールバックの破棄
	if (m_ghostPairCollback != nullptr)
	{
		delete m_ghostPairCollback;
		m_ghostPairCollback = nullptr;
	}

	// デバッグ描画の破棄
	if (m_debugDraw != nullptr)
	{
		delete m_debugDraw;
		m_debugDraw = nullptr;
	}

	m_collisionShapes.clear();
}

//=============================================================
// [CPhysics] 更新
//=============================================================
void CPhysics::Update()
{
	// 物理ワールドを進める
	if (m_isUpdatePhysics)
	{
		m_dynamicsWorld->stepSimulation(static_cast<btScalar>(1. / 60.), 1);
		m_dynamicsWorld->debugDrawWorld();
	}

	// コリジョンリストを取得する
	std::vector<CCollision*> pCollisions = CCollision::GetCollisions();

	for (unsigned int i = 0; i < pCollisions.size(); i++)
	{
		// リジッドボディを取得する
		btRigidBody* pRigidBody = pCollisions[i]->GetRigidBody();
		btGhostObject* pGhostObject = pCollisions[i]->GetGhostObject();
		btTransform resultTrans;		// 結果トランスフォーム

		// 前回の回転に変更が加えられていないか
		if (pCollisions[i]->GetBeforeRot() != pCollisions[i]->GetAttachObject()->transform->GetWQuaternion())
		{
			// 変更後の回転の取得
			D3DXQUATERNION wRot = pCollisions[i]->GetAttachObject()->transform->GetWQuaternion();

			// 新たに設定する回転を作成する
			btQuaternion quaternion(wRot.x, wRot.y, wRot.z, wRot.w);

			// 回転を適用する
			if (pRigidBody != nullptr)
			{ // リジッドボディーの場合
				resultTrans = pRigidBody->getWorldTransform();
				resultTrans.setRotation(quaternion);
				pRigidBody->setWorldTransform(resultTrans);

			}
			else
			{ // リジッドボディー以外の場合
				resultTrans = pGhostObject->getWorldTransform();
				resultTrans.setRotation(quaternion);
				pGhostObject->setWorldTransform(resultTrans);
			}
		}

		// 前回の位置に変更が加えられていないか
		if (pCollisions[i]->GetBeforePos() != pCollisions[i]->GetAttachObject()->transform->GetWPos())
		{
			// 変更後の位置の取得
			D3DXVECTOR3 wPos = pCollisions[i]->GetAttachObject()->transform->GetWPos();

			// 位置を適用する
			if (pRigidBody != nullptr)
			{ // リジッドボディーの場合
				if (pCollisions[i]->GetIsKinematic())
				{ // キネマティックオブジェクトのとき
					btTransform newTrans;
					pRigidBody->getMotionState()->getWorldTransform(newTrans);
					newTrans.getOrigin() = btVector3(wPos.x, wPos.y, wPos.z);
					pRigidBody->getMotionState()->setWorldTransform(newTrans);
				}
				else
				{ // キネマティックオブジェクト以外のとき
					D3DXVECTOR3 moveTrans = wPos - pCollisions[i]->GetBeforePos();
					pRigidBody->translate(btVector3(moveTrans.x, moveTrans.y, moveTrans.z));
				}
			}
			else
			{ // リジッドボディー以外の場合
				resultTrans = pGhostObject->getWorldTransform();
				resultTrans.setOrigin(btVector3(wPos.x, wPos.y, wPos.z));
				pGhostObject->setWorldTransform(resultTrans);
			}
		}

		// リジッドボディーの処理
		if (pRigidBody != nullptr)
		{
			// リジッドボディのトランスフォーム情報を取得する
			pRigidBody->getMotionState()->getWorldTransform(resultTrans);

			// アタッチされているゲームオブジェクトのトランスフォームを取得
			Transform* pTransform = pCollisions[i]->GetAttachObject()->transform;

			// 位置の反映
			pTransform->SetPos({ resultTrans.getOrigin().getX(), resultTrans.getOrigin().getY(), resultTrans.getOrigin().getZ() });

			// 向きの反映
			pTransform->SetQuaternion(D3DXQUATERNION(resultTrans.getRotation().getX(), resultTrans.getRotation().getY(), resultTrans.getRotation().getZ(), resultTrans.getRotation().getW()));
		}
	}

	// 前回のトランスフォーム値として保存する
	for (unsigned int i = 0; i < pCollisions.size(); i++)
	{
		pCollisions[i]->SaveTransform();
	}
}





//=============================================================
// [CCollision] コンストラクタ
//=============================================================
CCollision::CCollision(GameObject* gameObject) : m_pAttachObject(gameObject)
{
	m_pGhostObject = nullptr;
	m_pRigidBody = nullptr;
	m_fMass = 1.0f;
	m_fFriction = 1.0f;
	m_bTrigger = false;
	m_bKinematic = false;
	m_nGroup = 0;
	m_nMask = 0;
	m_bUseFilter = false;
	m_beforePos = gameObject->transform->GetWPos();
	m_beforeRot = gameObject->transform->GetWQuaternion();
}

//=============================================================
// [CCollision] デストラクタ
//=============================================================
CCollision::~CCollision()
{

}

//=============================================================
// [CCollision] 初期化
//=============================================================
void CCollision::Init()
{
	UpdateCollision();
}

//=============================================================
// [CCollision] 終了
//=============================================================
void CCollision::Uninit()
{
	// 形状の破棄
	if (m_compShape != nullptr)
	{
		for (int i = 0; i < m_compShape->getNumChildShapes(); i++)
		{
			m_compShape->removeChildShape(m_compShape->getChildShape(0));
		}

		delete m_compShape;
		m_compShape = nullptr;
	}

	// ゴーストオブジェクトの破棄
	if (m_pGhostObject != nullptr)
	{
		CPhysics::GetInstance()->GetDynamicsWorld().removeCollisionObject(m_pGhostObject);
		delete m_pGhostObject;
		m_pGhostObject = nullptr;
	}

	// リジッドボディの破棄
	if (m_pRigidBody != nullptr)
	{
		// モーションステートの破棄
		if (m_pRigidBody->getMotionState() != nullptr)
		{
			delete m_pRigidBody->getMotionState();
		}

		CPhysics::GetInstance()->GetDynamicsWorld().removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
}

//=============================================================
// [CCollision] （前回の値として）トランスフォーム値の保存
//=============================================================
void CCollision::SaveTransform()
{
	m_beforePos = m_pAttachObject->transform->GetWPos();
	m_beforeRot = m_pAttachObject->transform->GetWQuaternion();
}

//=============================================================
// [CCollision] トリガー状態の設定
//=============================================================
void CCollision::IsTrigger(const bool& isTrigger)
{
	m_bTrigger = isTrigger;		// 設定変更

	// コリジョンの更新
	UpdateCollision();
}

//=============================================================
// [CCollision] キネマティックの設定
//=============================================================
void CCollision::IsKinematic(const bool& isKinematic)
{
	m_bKinematic = isKinematic;		// 設定変更

	// コリジョンの更新
	UpdateCollision();
}

//=============================================================
// [CCollision] 質量の設定
//=============================================================
void CCollision::SetMass(const float& fMass)
{
	m_fMass = fMass;		// 設定変更

	// コリジョンの更新
	UpdateCollision();
}

//=============================================================
// [CCollision] 摩擦の設定
//=============================================================
void CCollision::SetFriction(const float& fFriction)
{
	m_fFriction = fFriction;		// 設定変更

	// コリジョンの更新
	UpdateCollision();
}

//=============================================================
// [CCollision] 歩行速度の取得
//=============================================================
float CCollision::GetWalkSpeed()
{
	if (m_pRigidBody != nullptr)
	{ // リジッドボディのとき
		return fabsf(m_pRigidBody->getLinearVelocity().getX()) + fabsf(m_pRigidBody->getLinearVelocity().getZ());
	}
	return 0.0f;
}

//=============================================================
// [CCollision] コリジョンの更新
//=============================================================
void CCollision::UpdateCollision()
{
	// 物理ワールドを取得する
	btDiscreteDynamicsWorld& pDynamicsWorld = CPhysics::GetInstance()->GetDynamicsWorld();

	// リジッドボディとゴーストオブジェクトを破棄する
	if (m_pRigidBody != nullptr)
	{
		// モーションステートの破棄
		if (m_pRigidBody->getMotionState() != nullptr)
		{
			delete m_pRigidBody->getMotionState();
		}

		pDynamicsWorld.removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
	if (m_pGhostObject != nullptr)
	{
		pDynamicsWorld.removeCollisionObject(m_pGhostObject);
		delete m_pGhostObject;
		m_pGhostObject = nullptr;
	}

	// 形状を更新する
	UpdateShapes();


	// リジッドボディコンポーネントを取得する
	CRigidBody* pCmpRigidBody = m_pAttachObject->GetComponent<CRigidBody>();

	if (pCmpRigidBody != nullptr)
	{ // リジッドボディコンポーネントが存在するとき
		// 剛体の形状
		btCollisionShape* shapes = m_compShape;
		btScalar mass = m_fMass;

		// 移動するオブジェクトか
		bool bIsDynamic = (mass != 0.0f);

		// 位置を設定
		D3DXVECTOR3 worldPos = m_pAttachObject->transform->GetWPos();
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(worldPos.x, worldPos.y, worldPos.z));

		// 回転を設定
		D3DXVECTOR3 worldRot = m_pAttachObject->transform->GetWRot();
		btQuaternion quaternion;
		quaternion.setEuler(worldRot.y, worldRot.x, worldRot.z);
		startTransform.setRotation(quaternion);

		// 慣性モーメント
		btVector3 inertia(0, 0, 0);
		if (bIsDynamic)
		{
			shapes->calculateLocalInertia(mass, inertia);
		}
		
		// 剛体操作
		btMotionState* pMotionState = new btDefaultMotionState(startTransform);

		// 剛体作成
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, pMotionState, shapes, inertia);
		m_pRigidBody = new btRigidBody(rb_info);

		// 摩擦設定
		m_pRigidBody->setFriction(m_fFriction);

		// キネマティックか
		if (m_bKinematic)
		{
			m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		}

		// 物理ワールドに追加
		if (!m_bUseFilter)
		{ // フィルターを使用しない
			pDynamicsWorld.addRigidBody(m_pRigidBody);
		}
		else
		{ // フィルターを使用する
			pDynamicsWorld.addRigidBody(m_pRigidBody, m_nGroup, m_nMask);
		}
	}
	else
	{	// ゴーストオブジェクトのとき
		// ゴーストオブジェクトを作成する
		m_pGhostObject = new btGhostObject;

		// 形状を設定する
		m_pGhostObject->setCollisionShape(m_compShape);

		// 摩擦設定
		m_pGhostObject->setFriction(m_fFriction);

		// 他のオブジェクトに干渉するか
		if (m_bTrigger)
		{
			m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		// 位置を設定
		D3DXVECTOR3 worldPos = m_pAttachObject->transform->GetWPos();
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(worldPos.x, worldPos.y, worldPos.z));

		// 回転を設定
		D3DXVECTOR3 worldRot = m_pAttachObject->transform->GetWRot();
		btQuaternion quaternion;
		quaternion.setEuler(worldRot.y, worldRot.x, worldRot.z);
		startTransform.setRotation(quaternion);

		m_pGhostObject->setWorldTransform(startTransform);

		// 物理ワールドに追加
		if (!m_bUseFilter)
		{ // フィルターを使用しない
			pDynamicsWorld.addCollisionObject(m_pGhostObject);
		}
		else
		{ // フィルターを使用する
			pDynamicsWorld.addCollisionObject(m_pGhostObject, m_nGroup, m_nMask);
		}
	}
}

//=============================================================
// [CCollision] 形状の更新
//=============================================================
void CCollision::UpdateShapes()
{
	// 現在の形状を破棄する
	if (m_compShape != nullptr)
	{
		for (int i = m_compShape->getNumChildShapes() - 1; i >= 0; i--)
		{
			m_compShape->removeChildShape(m_compShape->getChildShape(i));
		}
		delete m_compShape;
		m_compShape = nullptr;
	}

	// シェイプを作成する
	m_compShape = new btCompoundShape;

	// 形状を追加
	std::vector<CCollider*> pColliders = m_pAttachObject->GetComponents<CCollider>(true);
	for (unsigned int i = 0; i < pColliders.size(); i++)
	{
		pColliders[i]->AddShapes(m_compShape);
	}

	// スケールを設定する
	//D3DXVECTOR3 scale = m_pAttachObject->transform->GetScale();
	//m_compShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
}

//=============================================================
// [CCollision] 生成
//=============================================================
void CCollision::Create(GameObject* gameObject)
{
	// すでにコリジョンがないかを調べる
	if (isCollisionExist(gameObject))
	{ // 存在するとき
		return;
	}

	// コリジョンを生成する
	CCollision* pCollision = new CCollision(gameObject);
	pCollision->Init();

	// リストに追加する
	m_collisions.push_back(pCollision);

	// グループIDを設定する
	pCollision->m_nGroup = static_cast<int>(m_collisions.size());
}

//=============================================================
// [CCollision] フィルターを設定する
//=============================================================
void CCollision::SetFilter(const int& nGroup, const int& nMask)
{
	m_nGroup = nGroup;
	m_nMask = nMask;
}

//=============================================================
// [CCollision] フィルターを使用するか
//=============================================================
void CCollision::UseFilter(const bool& bUse)
{
	m_bUseFilter = bUse;
}

//=============================================================
// [CCollision] ゲームオブジェクトのコリジョンが存在するかを調べる
//=============================================================
bool CCollision::isCollisionExist(GameObject* gameObject)
{
	for (unsigned int i = 0; i < m_collisions.size(); i++)
	{
		if (m_collisions[i]->m_pAttachObject == gameObject)
		{
			return true;	// 見つかった
		}
	}
	return false;
}

//=============================================================
// [CCollision] コリジョンオブジェクトからゲームオブジェクトを取得する
//=============================================================
GameObject* CCollision::GetGameObjectFromCollisionObj(const btCollisionObject* collision)
{
	// すべてのコリジョンオブジェクトの取得
	std::vector<CCollision*>& pCollisions = CCollision::GetCollisions();

	for (unsigned int i = 0; i < pCollisions.size(); i++)
	{
		if (pCollisions[i] != nullptr)
		{ // nullptrチェック
			if (pCollisions[i]->GetGhostObject() == collision ||
				pCollisions[i]->GetRigidBody() == collision)
			{ // 見つかったとき
				return pCollisions[i]->GetAttachObject();		// アタッチされているゲームオブジェクトを返す
			}
		}
	}

	return nullptr;	// 見つからなかった
}

//=============================================================
// [CCollision] コリジョンを取得する
//=============================================================
CCollision* CCollision::GetCollision(GameObject* gameObject)
{
	for (unsigned int i = 0; i < m_collisions.size(); i++)
	{
		if (m_collisions[i]->m_pAttachObject == gameObject)
		{
			return m_collisions[i];		// コリジョンを返す
		}
	}
	return nullptr;
}

//=============================================================
// [CCollision] コリジョンを削除する
//=============================================================
void CCollision::RemoveCollision(GameObject* gameObject)
{
	for (unsigned int i = 0; i < m_collisions.size(); i++)
	{
		if (m_collisions[i]->m_pAttachObject == gameObject)
		{
			// 破棄する
			m_collisions[i]->Uninit();
			delete m_collisions[i];
			m_collisions[i] = nullptr;

			// リストから削除
			m_collisions.erase(m_collisions.begin() + i);
			return;
		}
	}
}

//=============================================================
// [CActionInterface] アクションの更新
//=============================================================
void CActionInterface::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
	// すべてのコリジョンオブジェクトの取得
	std::vector<CCollision*>& pCollisions = CCollision::GetCollisions();

	for (unsigned int nCntObj = 0; nCntObj < pCollisions.size(); nCntObj++)
	{
		if (pCollisions[nCntObj]->GetGhostObject() != nullptr && pCollisions[nCntObj]->GetIsTrigger())
		{ // ゴーストオブジェクトかつトリガーのとき
			btGhostObject* pObj = pCollisions[nCntObj]->GetGhostObject();		// 対象オブジェクトのポインタ
			std::vector<Component*>& pComponents = pCollisions[nCntObj]->GetAttachObject()->GetComponents();		// 対象オブジェクトにアタッチされているコンポーネントの取得
			std::vector<GameObject*>& pOverlappingVector = pCollisions[nCntObj]->GetOverlappingGameObjects();		// 前回までに入っていたオブジェクト一覧を取得する

			// 重なっているオブジェクトの数を取得する
			int nNumOverlapping = pObj->getNumOverlappingObjects();

			// Exitイベント用処理
			try {
				for (unsigned int i = 0; i < pOverlappingVector.size(); i++)
				{
					bool bCurrentExist = false;	// 今存在するか
					for (int n = 0; n < nNumOverlapping; n++)
					{
						GameObject* pCurrentGameObj = CCollision::GetGameObjectFromCollisionObj(pObj->getOverlappingObject(n));		// 現在のゲームオブジェクト
						if (pOverlappingVector[i] == pCurrentGameObj)
						{	// 今回も存在するとき
							bCurrentExist = true;
							break;
						}

						// オブジェクト数の更新
						nNumOverlapping = pObj->getNumOverlappingObjects();
					}

					if (!bCurrentExist)
					{ // 前回はあって、今回は存在しないとき
						for (unsigned int nCntComp = 0; nCntComp < pComponents.size(); nCntComp++)
						{
							pComponents[nCntComp]->OnTriggerExit(pOverlappingVector[i]);	// 重なっているときのイベント

							// イベント内で破棄されたとき
							if (pComponents.size() <= 0)
							{
								return;	// 処理を終了する
							}
						}
					}
				}
			}
			catch (char a) 
			{
				std::cout << a << "Exit Event Error" << std::endl;
			}

			try {
				// 重なっているオブジェクト分、繰り返す
				for (int i = 0; i < nNumOverlapping; i++)
				{
					btCollisionObject* pCurrentOverlappingObj = pObj->getOverlappingObject(i);													// 現在、重なっているオブジェクト
					GameObject* pCurrentOverlappingGameObj = CCollision::GetGameObjectFromCollisionObj(pCurrentOverlappingObj);	// 現在、重なっているオブジェクトのゲームオブジェクトの取得

					if (pCurrentOverlappingGameObj != nullptr)
					{
						// 入出イベント
						bool bBeforeExist = false;	// 前回も存在していたか
						for (unsigned int nCntOverlapping = 0; nCntOverlapping < pOverlappingVector.size(); nCntOverlapping++)
						{
							if (pOverlappingVector[nCntOverlapping] == pCurrentOverlappingGameObj)
							{ // 前回も存在しているとき
								bBeforeExist = true;
								break;
							}
						}

						if (!bBeforeExist)
						{ // 前回存在していないとき
							// OnEnter
							for (unsigned int nCntComp = 0; nCntComp < pComponents.size(); nCntComp++)
							{
								pComponents[nCntComp]->OnTriggerEnter(pCurrentOverlappingGameObj);	// 出たときのイベント
							}
						}


						for (unsigned int nCntComp = 0; nCntComp < pComponents.size(); nCntComp++)
						{
							pComponents[nCntComp]->OnTriggerStay(pCurrentOverlappingGameObj);	// 重なっているときのイベント
						}
					}

					// オブジェクト数の更新
					nNumOverlapping = pObj->getNumOverlappingObjects();
				}
			}
			catch (char a) 
			{
				std::cout << a << "Stay and Enter Events Error" << std::endl;
			}

			try {
				// コリジョンに入っているオブジェクトを更新する
				nNumOverlapping = pObj->getNumOverlappingObjects();
				if (!pOverlappingVector.empty())
				{
					pOverlappingVector.clear();
				}
				for (int i = 0; i < nNumOverlapping; i++)
				{
					btCollisionObject* pOverlappingObj = pObj->getOverlappingObject(i);													// 重なっているオブジェクト
					GameObject* pOverlappingGameObj = CCollision::GetGameObjectFromCollisionObj(pOverlappingObj);	// 重なっているオブジェクトのゲームオブジェクトの取得

					if (pOverlappingGameObj != nullptr)
					{ // nullptrチェック
						pOverlappingVector.push_back(pOverlappingGameObj);	// ゲームオブジェクトを追加
					}

					// オブジェクト数の更新
					nNumOverlapping = pObj->getNumOverlappingObjects();
				}
			}
			catch (char a) 
			{
				std::cout << a << "Update Overlapping Objects Error" << std::endl;
			}
		}
	}
}