//=============================================================
//
// 3D物理 [physics.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PHYSICS_3D_H_
#define _PHYSICS_3D_H_

#include "general/main.h"
#include "gameobject.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

class CLine;

// 物理デバッグの描画
class CPhysicsDebugDraw : public btIDebugDraw
{
public:
	CPhysicsDebugDraw();
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
	void draw3dText(const btVector3& location, const char* textString) {}
	void reportErrorWarning(const char* warningString) {}
	void clearLines();
	void setDebugMode(int debugMode) { m_debugMode = debugMode; }
	int getDebugMode() const { return m_debugMode; }
private:
	int m_debugMode;
	GameObject* m_pLineObject;
	std::vector<CLine*> m_pLines;
};

// アクションインターフェイス
class CActionInterface : public btActionInterface
{
public:
	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
	void debugDraw(btIDebugDraw* debugDrawer) {}
};

// 物理クラスの定義
class CPhysics
{
public:
	CPhysics();
	~CPhysics();
	void Init();				// 初期化
	void Uninit();			// 終了
	void Update();		// 更新
	void SetUpdatePhysics(const bool& enabled) { m_isUpdatePhysics = enabled; }

	static CPhysics* GetInstance() {
		static CPhysics physics;
		return &physics;
	}

	static btDiscreteDynamicsWorld& GetDynamicsWorld() { return *m_dynamicsWorld; }							// 物理ワールドの取得
	static btAlignedObjectArray<btCollisionShape*>& GetCollisionShape() { return m_collisionShapes; }		// 形状リストの取得
	static CPhysicsDebugDraw* GetPhysicsDebugDraw() { return m_debugDraw; }
private:
	static btDiscreteDynamicsWorld* m_dynamicsWorld;								// 物理ワールド
	static btAlignedObjectArray<btCollisionShape*> m_collisionShapes;		// 形状リスト
	static CPhysicsDebugDraw* m_debugDraw;
	bool m_isUpdatePhysics;

	// 設定
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;
	btGhostPairCallback* m_ghostPairCollback;
	CActionInterface* m_actionInterface;
};


// コリジョンクラス（コリジョンが必要なゲームオブジェクトをアタッチ先に設定する）
class CCollision
{
public:
	CCollision(GameObject* gameObject);				// コンストラクタ
	virtual ~CCollision();										// デストラクタ
	void Init();														// 初期化処理
	void Uninit();													// 終了処理
	void UpdateCollision();									// コリジョンを更新する
	void SaveTransform();									// トランスフォーム値を保存する

	const D3DXVECTOR3& GetBeforePos() { return m_beforePos; }					// 前回の位置の取得
	const D3DXQUATERNION& GetBeforeRot() { return m_beforeRot; }			// 前回の回転の取得

	GameObject* GetAttachObject() { return m_pAttachObject; }					// アタッチされているゲームオブジェクトの取得
	btGhostObject* GetGhostObject() { return m_pGhostObject; }					// ゴーストオブジェクトの取得
	btRigidBody* GetRigidBody() { return m_pRigidBody; }							// リジッドボディの取得
	void IsTrigger(const bool& isTrigger);														// トリガー状態の設定
	bool GetIsTrigger() { return m_bTrigger; }												// トリガー状態の取得
	void IsKinematic(const bool& isKinematic);												// キネマティクの設定
	bool GetIsKinematic() { return m_bKinematic; }										// キネマティクの取得
	void SetMass(const float& fMass);															// 質量の設定
	float GetMass() { return m_fMass; }														// 質量の取得
	void SetFriction(const float& fFriction);														// 摩擦の設定
	float GetFriction() { return m_fFriction; }													// 摩擦の取得
	std::vector<GameObject*>& GetOverlappingGameObjects() {					// 重なっているオブジェクト一覧の取得
		return m_overlappingObjects; 
	}
	btCompoundShape* GetCompoundShape() { return m_compShape; }

	float GetWalkSpeed();																				// 歩行速度の取得（XとZの移動速度）
	void SetFilter(const int& nGroup, const int& nMask = -1);							// フィルターの設定
	void UseFilter(const bool& bUse);															// フィルターを有効にするか

	static void Create(GameObject* gameObject);											// 生成
	static bool isCollisionExist(GameObject* gameObject);								// ゲームオブジェクトにコリジョンがあるか
	static GameObject* GetGameObjectFromCollisionObj(const btCollisionObject* collision);		// コリジョンオブジェクトからゲームオブジェクトを取得する
	static CCollision* GetCollision(GameObject* gameObject);							// コリジョンの取得
	static void RemoveCollision(GameObject* gameObject);							// コリジョンの削除
	static std::vector<CCollision*>& GetCollisions() { return m_collisions; }		// すべてのコリジョンを取得
private:
	void UpdateShapes();												// 形状の更新

	GameObject* const m_pAttachObject;						// アタッチするゲームオブジェクト
	btGhostObject* m_pGhostObject;							// ゴーストオブジェクト（当たり判定）
	btRigidBody* m_pRigidBody;									// リジッドボディ
	btCompoundShape* m_compShape;						// 形状
	float m_fMass;														// 質量
	float m_fFriction;													// 摩擦
	bool m_bTrigger;													// トリガー
	bool m_bKinematic;												// キネマティック
	int m_nGroup;														// グループ
	int m_nMask;															// マスク
	bool m_bUseFilter;													// フィルターを使用するか
	std::vector<GameObject*> m_overlappingObjects;	// 重なっているオブジェクト
	static std::vector<CCollision*> m_collisions;			// コリジョンの保存
	

	// 前回の値
	D3DXVECTOR3 m_beforePos;			// 位置
	D3DXQUATERNION m_beforeRot;	// 回転
};

#endif // !_PHYSICS_3D_H_
