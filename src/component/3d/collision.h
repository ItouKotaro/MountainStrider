//=============================================================
//
// �R���W�����R���|�[�l���g [collision.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <component.h>
#include "internal/physics.h"

// ���W�b�h�{�f�B
class CRigidBody : public Component
{
public:
	void Init() override;					// ������
	void Update() override;				// �X�V

	btRigidBody* GetRigidBody();		// ���W�b�h�{�f�B�̎擾
	CCollision* GetCollision();			// �R���W�����̎擾
	void SetAlwayActive(bool bActive) { m_bAlwayActive = bActive; }
	bool GetAlwayActive() { return m_bAlwayActive; }
private:
	bool m_bAlwayActive;						// ��ɃA�N�e�B�u
};

// �S�[�X�g�I�u�W�F�N�g
class CGhostObject : public Component
{
public:
	void Init() override;							// ������
	btGhostObject* GetGhostObject();		// �S�[�X�g�I�u�W�F�N�g�̎擾
	CCollision* GetCollision();					// �R���W�����̎擾
};

// �R���C�_�[���N���X
class CCollider : public Component
{
public:
	virtual void AddShapes(btCompoundShape* pShapes) = 0;						// �`��ǉ�
	void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }			// �I�t�Z�b�g�ݒ�
	D3DXVECTOR3 GetOffset() { return m_offset; }										// �I�t�Z�b�g�擾
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }							// ��]�ݒ�
	D3DXVECTOR3 GetRot() { return m_rot; }												// ��]�擾
private:
	D3DXVECTOR3 m_offset;		// ���S
	D3DXVECTOR3 m_rot;			// ��]
};


// �{�b�N�X�R���C�_�[
class CBoxCollider : public CCollider
{
public:
	CBoxCollider(D3DXVECTOR3 size = {1.0f, 1.0f, 1.0f}, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });		// �R���X�g���N�^
	void Init() override;																						// ������
	void AddShapes(btCompoundShape* pShapes) override;									// �`��ǉ�

	void SetSize(const D3DXVECTOR3& size) { m_size = size; }								// �T�C�Y�ݒ�
	D3DXVECTOR3 GetSize() { return m_size; }														// �T�C�Y�擾
private:
	D3DXVECTOR3 m_size;		// �T�C�Y
};

// ���R���C�_�[
class CSphereCollider : public CCollider
{
public:
	CSphereCollider(float fRadius = 1.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// �R���X�g���N�^
	void Init() override;																					// ������
	void AddShapes(btCompoundShape* pShapes) override;								// �`��ǉ�

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// ���a�ݒ�
	float GetRadius() { return m_fRadius; }														// ���a�擾
private:
	float m_fRadius;		// ���a
};

// �J�v�Z���R���C�_�[
class CCapsuleCollider : public CCollider
{
public:
	CCapsuleCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// �R���X�g���N�^
	void Init() override;																					// ������
	void AddShapes(btCompoundShape* pShapes) override;								// �`��ǉ�

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// ���a�ݒ�
	float GetRadius() { return m_fRadius; }														// ���a�擾
	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// �����ݒ�
	float GetHeight() { return m_fHeight; }														// �����擾
private:
	float m_fRadius;		// ���a
	float m_fHeight;		// ����
};

// �V�����_�[(�~��) �R���C�_�[
class CCylinderCollider : public CCollider
{
public:
	CCylinderCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// �R���X�g���N�^
	void Init() override;																					// ������
	void AddShapes(btCompoundShape* pShapes) override;								// �`��ǉ�

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// ���a�ݒ�
	float GetRadius() { return m_fRadius; }														// ���a�擾
	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// �����ݒ�
	float GetHeight() { return m_fHeight; }														// �����擾
private:
	float m_fRadius;		// ���a
	float m_fHeight;		// ����
};

// �R�[��(�~��) �R���C�_�[
class CConeCollider : public CCollider
{
public:
	CConeCollider(float fRadius = 1.0f, float fHeight = 2.0f, D3DXVECTOR3 offset = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });	// �R���X�g���N�^
	void Init() override;																					// ������
	void AddShapes(btCompoundShape* pShapes) override;								// �`��ǉ�

	void SetRadius(const float& fRadius) { m_fRadius = fRadius; }						// ���a�ݒ�
	float GetRadius() { return m_fRadius; }														// ���a�擾
	void SetHeight(const float& fHeight) { m_fHeight = fHeight; }						// �����ݒ�
	float GetHeight() { return m_fHeight; }														// �����擾
private:
	float m_fRadius;		// ���a
	float m_fHeight;		// ����
};

#endif // !_COLLISION_H_
