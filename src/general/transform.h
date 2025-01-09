//=============================================================
//
// トランスフォーム [transform.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

class GameObject;

// トランスフォームクラスの定義
class Transform
{
public:
	Transform(D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 scale = { 1.0f, 1.0f, 1.0f }) {
		m_position = pos;
		this->SetRot(rot);
		m_scale = scale;
		m_size = { 1.0f, 1.0f };
		m_pParent = nullptr;
	}

	void Translate(float x, float y, float z) { m_position += {x, y, z}; }
	void Translate(D3DXVECTOR3 pos) { Translate(pos.x, pos.y, pos.z); }
	void Rotate(float x, float y, float z) {
		D3DXVECTOR3 axis;
		D3DXQUATERNION q;

		// X
		axis = { 1.0f, 0.0f, 0.0f };
		D3DXQuaternionRotationAxis(&q, &axis, x);
		D3DXQuaternionMultiply(&m_rotation, &m_rotation, &q);

		// Y
		axis = { 0.0f, 1.0f, 0.0f };
		D3DXQuaternionRotationAxis(&q, &axis, y);
		D3DXQuaternionMultiply(&m_rotation, &m_rotation, &q);

		// Z
		axis = { 0.0f, 0.0f, 1.0f };
		D3DXQuaternionRotationAxis(&q, &axis, z);
		D3DXQuaternionMultiply(&m_rotation, &m_rotation, &q);

		//D3DXQuaternionRotationYawPitchRoll(&q, y, x, z);
		//D3DXQuaternionMultiply(&m_rotation, &m_rotation, &q);
	}
	void Rotate(D3DXVECTOR3 rot) {
		Rotate(rot.x, rot.y, rot.z);
	}

	D3DXVECTOR3 GetPos() { return m_position; }
	float GetPosX() { return m_position.x; }
	float GetPosY() { return m_position.y; }
	float GetPosZ() { return m_position.z; }
	D3DXQUATERNION GetQuaternion() { return m_rotation; }
	D3DXVECTOR3 GetRot();
	float GetRotX() { return GetRot().x; }
	float GetRotY() { return GetRot().y; }
	float GetRotZ() { return GetRot().z; }
	D3DXVECTOR3 GetScale() { return m_scale; }
	float GetScaleX() { return m_scale.x; }
	float GetScaleY() { return m_scale.y; }
	float GetScaleZ() { return m_scale.z; }
	D3DXVECTOR2 GetSize() { return m_size; }
	float GetSizeX() { return m_size.x; }
	float GetSizeY() { return m_size.y; }
	Transform* GetParent() { return m_pParent; }

	D3DXVECTOR3 GetWPos();
	float GetWPosX() { return GetWPos().x; }
	float GetWPosY() { return GetWPos().y; }
	float GetWPosZ() { return GetWPos().z; }
	D3DXQUATERNION GetWQuaternion();
	D3DXVECTOR3 GetWRot();
	float GetWRotX() { return GetWRot().x; }
	float GetWRotY() { return GetWRot().y; }
	float GetWRotZ() { return GetWRot().z; }
	D3DXVECTOR3 GetWScale();
	float GetWScaleX() { return GetWScale().x; }
	float GetWScaleY() { return GetWScale().y; }
	float GetWScaleZ() { return GetWScale().z; }
	D3DXMATRIX& GetMatrix();
	D3DXMATRIX GetTranslationMatrix() {
		D3DXMATRIX mtx;
		D3DXMatrixIdentity(&mtx);
		D3DXMatrixTranslation(&mtx, GetWPosX(), GetWPosY(), GetWPosZ());
		return mtx;
	}
	D3DXMATRIX GetRotationMatrix() {
		D3DXMATRIX mtx;
		D3DXMatrixIdentity(&mtx);
		D3DXMatrixRotationYawPitchRoll(&mtx, GetWRotY(), GetWRotX(), GetWRotZ());
		return mtx;
	}

	void SetPos(float x, float y, float z);
	void SetPos(D3DXVECTOR3 position) { SetPos(position.x, position.y, position.z); }
	void SetPos(float x, float y) { SetPos(x, y, m_position.z);  }
	void SetPos(D3DXVECTOR2 position) { SetPos(position.x, position.y, m_position.z); }
	void SetQuaternion(D3DXQUATERNION quaternion) { m_rotation = quaternion; }
	void MultiplyQuaternion(D3DXQUATERNION quaternion) { D3DXQuaternionMultiply(&m_rotation, &m_rotation, &quaternion); }
	void SetRot(float x, float y, float z);
	void SetRot(D3DXVECTOR3 rotation) { SetRot(rotation.x, rotation.y, rotation.z); }
	void SetRot(float fAngle) { SetRot(GetRot().x, GetRot().y, fAngle); }
	void LookAt(D3DXVECTOR3 target);
	void SetScale(float x, float y, float z) { m_scale = { x, y, z }; }
	void SetScale(D3DXVECTOR3 scale) { SetScale(scale.x, scale.y, scale.z); }
	void SetScale(float ratio) { SetScale(ratio, ratio, ratio); }
	void SetSize(float x, float y) { m_size = { x, y }; }
	void SetSize(int x, int y) { m_size = { static_cast<float>(x), static_cast<float>(y) }; }
	void SetSize(D3DXVECTOR2 size) { SetSize(size.x, size.y); }
	void SetParent(Transform* pParent);
	static D3DXVECTOR3 NormalizeRotation(const D3DXVECTOR3& rot);						// 回転の正規化

	// 演算子
	bool operator ==(Transform& transform);
private:
	D3DXVECTOR3 m_position;		// 位置
	D3DXQUATERNION m_rotation;	// 回転
	D3DXVECTOR3 m_scale;			// スケール
	D3DXVECTOR2 m_size;			// サイズ（2D）
	D3DXMATRIX m_mtx;				// マトリックス
	Transform* m_pParent;			// 親
};

#endif // !_TRANSFORM_H_
