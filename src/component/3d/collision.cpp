//=============================================================
//
// コリジョンコンポーネント [collision.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "collision.h"
#include "internal/physics.h"
#include "manager.h"


//=============================================================
// [CRigidBody] 初期化
//=============================================================
void CRigidBody::Init()
{
	// コリジョン
	CCollision::Create(gameObject);												// 作成
	CCollision* pCollision = CCollision::GetCollision(gameObject);	// 取得

	// コリジョンの更新
	if (pCollision != nullptr)
	{
		pCollision->UpdateCollision();
	}
}

//=============================================================
// [CRigidBody] 更新
//=============================================================
void CRigidBody::Update()
{
}

//=============================================================
// [CRigidBody] リジッドボディの取得
//=============================================================
btRigidBody* CRigidBody::GetRigidBody()
{
	return CCollision::GetCollision(gameObject)->GetRigidBody();
}

//=============================================================
// [CRigidBody] コリジョンの取得
//=============================================================
CCollision* CRigidBody::GetCollision()
{
	return CCollision::GetCollision(gameObject);
}

//=============================================================
// [CRigidBody] 常にアクティブ
//=============================================================
void CRigidBody::EnableAlwayActive()
{
	GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
}

//=============================================================
// [CGhostObject] 初期化
//=============================================================
void CGhostObject::Init()
{
	// コリジョン
	CCollision::Create(gameObject);												// 作成
	CCollision* pCollision = CCollision::GetCollision(gameObject);	// 取得

	// コリジョンの更新
	if (pCollision != nullptr)
	{
		pCollision->UpdateCollision();
	}
}

//=============================================================
// [CGhostObject] ゴーストオブジェクトの取得
//=============================================================
btGhostObject* CGhostObject::GetGhostObject()
{
	return CCollision::GetCollision(gameObject)->GetGhostObject();
}

//=============================================================
// [CGhostObject] コリジョンの取得
//=============================================================
CCollision* CGhostObject::GetCollision()
{
	return CCollision::GetCollision(gameObject);
}


//=============================================================
// [CPoint2PointConstraint] コンストラクタ
//=============================================================
CPoint2PointConstraint::CPoint2PointConstraint() : 
	m_p2p(nullptr)
{
}

//=============================================================
// [CPoint2PointConstraint] 終了
//=============================================================
void CPoint2PointConstraint::Uninit()
{
	if (m_p2p != nullptr)
	{
		// 物理ワールドから削除する
		CPhysics::GetInstance()->GetDynamicsWorld().removeConstraint(m_p2p);

		// 破棄する
		delete m_p2p;
		m_p2p = nullptr;
	}
}

//=============================================================
// [CPoint2PointConstraint] 拘束設定
//=============================================================
void CPoint2PointConstraint::SetConstraint(btRigidBody* rb1, btRigidBody* rb2, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB)
{
	// 破棄する
	CPoint2PointConstraint::Uninit();

	// 作成する
	m_p2p = new btPoint2PointConstraint(
		*rb1, 
		*rb2, 
		btVector3(pivotInA.x, pivotInA.y, pivotInA.z), 
		btVector3(pivotInB.x, pivotInB.y, pivotInB.z)
	);

	// 設定する
	CPhysics::GetInstance()->GetDynamicsWorld().addConstraint(m_p2p);
}


//=============================================================
// [CHingeConstraint] コンストラクタ
//=============================================================
CHingeConstraint::CHingeConstraint() :
	m_hinge(nullptr)
{
}

//=============================================================
// [CHingeConstraint] 終了
//=============================================================
void CHingeConstraint::Uninit()
{
	if (m_hinge != nullptr)
	{
		// 物理ワールドから削除する
		CPhysics::GetInstance()->GetDynamicsWorld().removeConstraint(m_hinge);

		// 破棄する
		delete m_hinge;
		m_hinge = nullptr;
	}
}

//=============================================================
// [CHingeConstraint] 拘束設定
//=============================================================
void CHingeConstraint::SetConstraint(btRigidBody* rb, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& axisInA)
{
	// 破棄する
	CHingeConstraint::Uninit();

	// 作成する
	m_hinge = new btHingeConstraint(
		*rb,
		btVector3(pivotInA.x, pivotInA.y, pivotInA.z),
		btVector3(axisInA.x, axisInA.y, axisInA.z)
	);

	// 設定する
	CPhysics::GetInstance()->GetDynamicsWorld().addConstraint(m_hinge);
}

//=============================================================
// [CHingeConstraint] 拘束設定
//=============================================================
void CHingeConstraint::SetConstraint(btRigidBody* rb1, btRigidBody* rb2, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB, const D3DXVECTOR3& axisInA, const D3DXVECTOR3& axisInB)
{
	// 破棄する
	CHingeConstraint::Uninit();

	// 作成する
	m_hinge = new btHingeConstraint(
		*rb1, *rb2,
		btVector3(pivotInA.x, pivotInA.y, pivotInA.z), btVector3(pivotInB.x, pivotInB.y, pivotInB.z),
		btVector3(axisInA.x, axisInA.y, axisInA.z), btVector3(axisInB.x, axisInB.y, axisInB.z)
	);

	// 設定する
	CPhysics::GetInstance()->GetDynamicsWorld().addConstraint(m_hinge);
}


//=============================================================
// [CHinge2Constraint] コンストラクタ
//=============================================================
CHinge2Constraint::CHinge2Constraint() :
	m_hinge2(nullptr)
{
}

//=============================================================
// [CHinge2Constraint] 終了
//=============================================================
void CHinge2Constraint::Uninit()
{
	if (m_hinge2 != nullptr)
	{
		// 物理ワールドから削除する
		CPhysics::GetInstance()->GetDynamicsWorld().removeConstraint(m_hinge2);

		// 破棄する
		delete m_hinge2;
		m_hinge2 = nullptr;
	}
}

//=============================================================
// [CHinge2Constraint] 拘束設定
//=============================================================
void CHinge2Constraint::SetConstraint(btRigidBody* rb1, btRigidBody* rb2, D3DXVECTOR3 anchor, D3DXVECTOR3 parentAxis, D3DXVECTOR3 childAxis)
{
	// 破棄する
	CHinge2Constraint::Uninit();

	// 作成する
	btVector3 btAnchor = btVector3(anchor.x, anchor.y, anchor.z);
	btVector3 btParentAxis = btVector3(parentAxis.x, parentAxis.y, parentAxis.z);
	btVector3 btChildAxis = btVector3(childAxis.x, childAxis.y, childAxis.z);
	m_hinge2 = new btHinge2Constraint(
		*rb1, *rb2,
		btAnchor,
		btParentAxis,
		btChildAxis
	);

	// 設定する
	CPhysics::GetInstance()->GetDynamicsWorld().addConstraint(m_hinge2, true);
}

//=============================================================
// [CBoxCollider] コンストラクタ
//=============================================================
CBoxCollider::CBoxCollider(D3DXVECTOR3 size, D3DXVECTOR3 offset, D3DXVECTOR3 rot)
{
	SetSize(size);
	SetOffset(offset);
	SetRot(rot);
}

//=============================================================
// [CBoxCollider] 初期化
//=============================================================
void CBoxCollider::Init()
{
	// コリジョンの作成（すでにある場合はスルー）
	CCollision::Create(gameObject);

	// コリジョンの取得
	CCollision* pCollision = CCollision::GetCollision(gameObject);

	// コリジョンの更新
	pCollision->UpdateCollision();
}

//=============================================================
// [CBoxCollider] シェイプに追加する
//=============================================================
void CBoxCollider::AddShapes(btCompoundShape* pShapes)
{
	// ローカル姿勢
	btTransform localTrans = btTransform::getIdentity();
	localTrans.setOrigin(btVector3(GetOffset().x, GetOffset().y, GetOffset().z));

	// 回転
	btQuaternion quaternion;
	quaternion.setEuler(GetRot().y, GetRot().x, GetRot().z);
	localTrans.setRotation(quaternion);

	// シェイプを追加する
	btBoxShape* box_shape = new btBoxShape(btVector3(m_size.x, m_size.y, m_size.z));
	pShapes->addChildShape(localTrans, box_shape);

	// リストに追加する
	CPhysics::GetInstance()->GetCollisionShape().push_back(box_shape);
}


//=============================================================
// [CSphereCollider] コンストラクタ
//=============================================================
CSphereCollider::CSphereCollider(float fRadius, D3DXVECTOR3 offset, D3DXVECTOR3 rot)
{
	SetRadius(fRadius);
	SetOffset(offset);
	SetRot(rot);
}

//=============================================================
// [CSphereCollider] 初期化
//=============================================================
void CSphereCollider::Init()
{
	// コリジョンの作成（すでにある場合はスルー）
	CCollision::Create(gameObject);

	// コリジョンの取得
	CCollision* pCollision = CCollision::GetCollision(gameObject);

	// コリジョンの更新
	pCollision->UpdateCollision();
}

//=============================================================
// [CSphereCollider] シェイプに追加する
//=============================================================
void CSphereCollider::AddShapes(btCompoundShape* pShapes)
{
	// ローカル姿勢
	btTransform localTrans = btTransform::getIdentity();
	localTrans.setOrigin(btVector3(GetOffset().x, GetOffset().y, GetOffset().z));

	// 回転
	btQuaternion quaternion;
	quaternion.setEuler(GetRot().y, GetRot().x, GetRot().z);
	localTrans.setRotation(quaternion);

	// シェイプを追加する
	btSphereShape* sphere_shape = new btSphereShape(m_fRadius);
	pShapes->addChildShape(localTrans, sphere_shape);

	// リストに追加する
	CPhysics::GetInstance()->GetCollisionShape().push_back(sphere_shape);
}


//=============================================================
// [CCapsuleCollider] コンストラクタ
//=============================================================
CCapsuleCollider::CCapsuleCollider(float fRadius, float fHeight, D3DXVECTOR3 offset, D3DXVECTOR3 rot)
{
	SetRadius(fRadius);
	SetHeight(fHeight);
	SetOffset(offset);
	SetRot(rot);
}

//=============================================================
// [CCapsuleCollider] 初期化
//=============================================================
void CCapsuleCollider::Init()
{
	// コリジョンの作成（すでにある場合はスルー）
	CCollision::Create(gameObject);

	// コリジョンの取得
	CCollision* pCollision = CCollision::GetCollision(gameObject);

	// コリジョンの更新
	pCollision->UpdateCollision();
}

//=============================================================
// [CCapsuleCollider] シェイプに追加する
//=============================================================
void CCapsuleCollider::AddShapes(btCompoundShape* pShapes)
{
	// ローカル姿勢
	btTransform localTrans = btTransform::getIdentity();
	localTrans.setOrigin(btVector3(GetOffset().x, GetOffset().y, GetOffset().z));

	// 回転
	btQuaternion quaternion;
	quaternion.setEuler(GetRot().y, GetRot().x, GetRot().z);
	localTrans.setRotation(quaternion);

	// シェイプを追加する
	btCapsuleShape* capsule_shape = new btCapsuleShape(m_fRadius, m_fHeight);
	pShapes->addChildShape(localTrans, capsule_shape);

	// リストに追加する
	CPhysics::GetInstance()->GetCollisionShape().push_back(capsule_shape);
}


//=============================================================
// [CCylinderCollider] コンストラクタ
//=============================================================
CCylinderCollider::CCylinderCollider(float fRadius, float fHeight, D3DXVECTOR3 offset, D3DXVECTOR3 rot)
{
	SetRadius(fRadius);
	SetHeight(fHeight);
	SetOffset(offset);
	SetRot(rot);
}

//=============================================================
// [CCylinderCollider] 初期化
//=============================================================
void CCylinderCollider::Init()
{
	// コリジョンの作成（すでにある場合はスルー）
	CCollision::Create(gameObject);

	// コリジョンの取得
	CCollision* pCollision = CCollision::GetCollision(gameObject);

	// コリジョンの更新
	pCollision->UpdateCollision();
}

//=============================================================
// [CCylinderCollider] シェイプに追加する
//=============================================================
void CCylinderCollider::AddShapes(btCompoundShape* pShapes)
{
	// ローカル姿勢
	btTransform localTrans = btTransform::getIdentity();
	localTrans.setOrigin(btVector3(GetOffset().x, GetOffset().y, GetOffset().z));

	// 回転
	btQuaternion quaternion;
	quaternion.setEuler(GetRot().y, GetRot().x, GetRot().z);
	localTrans.setRotation(quaternion);

	// シェイプを追加する
	btCylinderShape* cylinder_shape = new btCylinderShape(btVector3(m_fRadius, m_fHeight, m_fRadius));
	pShapes->addChildShape(localTrans, cylinder_shape);

	// リストに追加する
	CPhysics::GetInstance()->GetCollisionShape().push_back(cylinder_shape);
}


//=============================================================
// [CConeCollider] コンストラクタ
//=============================================================
CConeCollider::CConeCollider(float fRadius, float fHeight, D3DXVECTOR3 offset, D3DXVECTOR3 rot)
{
	SetRadius(fRadius);
	SetHeight(fHeight);
	SetOffset(offset);
	SetRot(rot);
}

//=============================================================
// [CConeCollider] 初期化
//=============================================================
void CConeCollider::Init()
{
	// コリジョンの作成（すでにある場合はスルー）
	CCollision::Create(gameObject);

	// コリジョンの取得
	CCollision* pCollision = CCollision::GetCollision(gameObject);

	// コリジョンの更新
	pCollision->UpdateCollision();
}
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
//=============================================================
// [CConeCollider] シェイプに追加する
//=============================================================
void CConeCollider::AddShapes(btCompoundShape* pShapes)
{
	// ローカル姿勢
	btTransform localTrans = btTransform::getIdentity();
	localTrans.setOrigin(btVector3(GetOffset().x, GetOffset().y, GetOffset().z));

	// 回転
	btQuaternion quaternion;
	quaternion.setEuler(GetRot().y, GetRot().x, GetRot().z);
	localTrans.setRotation(quaternion);

	// シェイプを追加する
	btConeShape* cone_shape = new btConeShape(m_fRadius, m_fHeight);
	pShapes->addChildShape(localTrans, cone_shape);

	// リストに追加する
	CPhysics::GetInstance()->GetCollisionShape().push_back(cone_shape);
}