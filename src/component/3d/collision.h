//=============================================================
//
// コリジョンコンポーネント [collision.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <component.h>
#include "internal/physics.h"

// リジッドボディ
class CRigidBody : public Component
{
public:
	void Init() override;					// 初期化
	void Update() override;				// 更新

	btRigidBody* GetRigidBody();		// リジッドボディの取得
	CCollision* GetCollision();			// コリジョンの取得
	void EnableAlwayActive();
private:
};

// ゴーストオブジェクト
class CGhostObject : public Component
{
public:
	void Init() override;							// 初期化
	btGhostObject* GetGhostObject();		// ゴーストオブジェクトの取得
	CCollision* GetCollision();					// コリジョンの取得
};

// 点球結合
class CPoint2PointConstraint : public Component
{
public:
	CPoint2PointConstraint();
	void Uninit() override;
	void SetConstraint(btRigidBody* rb1, btRigidBody* rb2, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB);
	btPoint2PointConstraint* GetPoint2Point() { return m_p2p; }
private:
	btPoint2PointConstraint* m_p2p;
};

// ヒンジ結合
class CHingeConstraint : public Component
{
public:
	CHingeConstraint();
	void Uninit() override;
	void SetConstraint(btRigidBody* rb, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& axisInA);
	void SetConstraint(btRigidBody* rb1, btRigidBody* rb2, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB, const D3DXVECTOR3& axisInA, const D3DXVECTOR3& axisInB);
	btHingeConstraint* GetHinge() { return m_hinge; }
private:
	btHingeConstraint* m_hinge;
};

// 2軸ヒンジ結合
class CHinge2Constraint : public Component
{
public:
	CHinge2Constraint();
	void Uninit() override;
	void SetConstraint(btRigidBody* rb1, btRigidBody* rb2, D3DXVECTOR3 anchor, D3DXVECTOR3 parentAxis, D3DXVECTOR3 childAxis);
	btHinge2Constraint* GetHinge2() { return m_hinge2; }
private:
	btHinge2Constraint* m_hinge2;
};

// コライダー基底クラス
class CCollider : public Component
{
public:
	virtual void AddShapes(btCompoundShape* pShapes) = 0;						// 形状追加
	void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }			// オフセット設定
	D3DXVECTOR3 GetOffset() { return m_offset; }										// オフセット取得
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }							// 回転設定
	D3DXVECTOR3 GetRot() { return m_rot; }												// 回転取得
private:
	D3DXVECTOR3 m_offset;		// 中心
	D3DXVECTOR3 m_rot;			// 回転
};


// ボックスコライダー
class CBoxCollider : public CCollider
{
public:
	CBoxCollider(D3DXVECTOR3 size = {1.0f, 1.0f, 1.0f}, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });		// コンストラクタ
	void Init() override;																						// 初期化
	void AddShapes(btCompoundShape* pShapes) override;									// 形状追加

	void SetSize(const D3DXVECTOR3& size) { m_size = size; }								// サイズ設定
	D3DXVECTOR3 GetSize() { return m_size; }														// サイズ取得
private:
	D3DXVECTOR3 m_size;		// サイズ
};

// 球コライダー
class CSphereCollider : public CCollider
{
public:
	CSphereCollider(float fRadius = 1.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
	void Init() override;																					// 初期化
	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
	float GetRadius() { return m_fRadius; }														// 半径取得
private:
	float m_fRadius;		// 半径
};

// カプセルコライダー
class CCapsuleCollider : public CCollider
{
public:
	CCapsuleCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
	void Init() override;																					// 初期化
	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
	float GetRadius() { return m_fRadius; }														// 半径取得
	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// 高さ設定
	float GetHeight() { return m_fHeight; }														// 高さ取得
private:
	float m_fRadius;		// 半径
	float m_fHeight;		// 高さ
};

// シリンダー(円柱) コライダー
class CCylinderCollider : public CCollider
{
public:
	CCylinderCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
	void Init() override;																					// 初期化
	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
	float GetRadius() { return m_fRadius; }														// 半径取得
	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// 高さ設定
	float GetHeight() { return m_fHeight; }														// 高さ取得
private:
	float m_fRadius;		// 半径
	float m_fHeight;		// 高さ
};

// コーン(円錐) コライダー
class CConeCollider : public CCollider
{
public:
	CConeCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// コンストラクタ
	void Init() override;																					// 初期化
	void AddShapes(btCompoundShape* pShapes) override;								// 形状追加

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// 半径設定
	float GetRadius() { return m_fRadius; }														// 半径取得
	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// 高さ設定
	float GetHeight() { return m_fHeight; }														// 高さ取得
private:
	float m_fRadius;		// 半径
	float m_fHeight;		// 高さ
};

#endif // !_COLLISION_H_
