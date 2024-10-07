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
	if (m_bAlwayActive)
	{
		GetRigidBody()->activate(true);
	}
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