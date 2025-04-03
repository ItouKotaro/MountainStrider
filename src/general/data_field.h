//=============================================================
//
// データフィールド [data_field.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DATA_FIELD_H_
#define _DATA_FIELD_H_

#include "gameobject.h"


// データフィールドのデータ
class CDF
{
public:
	enum TYPE
	{
		INT,
		FLOAT,
		BOOL,
		STRING,
		VECTOR3,
		VECTOR2,
	};

	CDF(const TYPE& type) { m_type = type; }								// コンストラクタ（型設定）
	TYPE GetType() { return m_type; }											// 種類の取得
	void SetName(const std::string name) { m_name = name; }	// 名前の設定
	std::string GetName() { return m_name; }								// 名前の取得
private:
	std::string m_name;	// 名前
	TYPE m_type;			// 種類
};

// Int型データ
class CDF_Int : public CDF
{
public:
	CDF_Int() : CDF(TYPE::INT), m_nData(0) {}
	void SetValue(const int& data) { m_nData = data; }
	int GetValue() { return m_nData; }
private:
	int m_nData;
};

// Float型データ
class CDF_Float : public CDF
{
public:
	CDF_Float() : CDF(TYPE::FLOAT), m_fData(0.0f) {}
	void SetValue(const float& data) { m_fData = data; }
	float GetValue() { return m_fData; }
private:
	float m_fData;
};

// Bool型データ
class CDF_Bool : public CDF
{
public:
	CDF_Bool() : CDF(TYPE::BOOL), m_bData(false) {}
	void SetValue(const bool& data) { m_bData = data; }
	bool GetValue() { return m_bData; }
private:
	bool m_bData;
};

// String型データ
class CDF_String : public CDF
{
public:
	CDF_String() : CDF(TYPE::STRING), m_sData("") {}
	void SetValue(const std::string& data) { m_sData = data; }
	std::string GetValue() { return m_sData; }
private:
	std::string m_sData;
};

// Vector3型データ
class CDF_Vector3 : public CDF
{
public:
	CDF_Vector3() : CDF(TYPE::VECTOR3), m_vec3Data(D3DXVECTOR3(0.0f, 0.0f, 0.0f)) {}
	void SetValue(const D3DXVECTOR3& data) { m_vec3Data = data; }
	D3DXVECTOR3 GetValue() { return m_vec3Data; }
private:
	D3DXVECTOR3 m_vec3Data;
};

// Vector2型データ
class CDF_Vector2 : public CDF
{
public:
	CDF_Vector2() : CDF(TYPE::VECTOR2), m_vec2Data(D3DXVECTOR2(0.0f, 0.0f)) {}
	void SetValue(const D3DXVECTOR2& data) { m_vec2Data = data; }
	D3DXVECTOR2 GetValue() { return m_vec2Data; }
private:
	D3DXVECTOR2 m_vec2Data;
};

// データフィールド
class CDataField
{
public:
	~CDataField();

	// 各型のデータ取得
	int GetInt(const std::string& name);
	float GetFloat(const std::string& name);
	bool GetBool(const std::string& name);
	std::string GetString(const std::string& name);
	D3DXVECTOR3 GetVector3(const std::string& name);
	D3DXVECTOR2 GetVector2(const std::string& name);

	// 操作
	void AddData(CDF* pData);
	CDF* GetData(const std::string& name);

	static void Destroy(GameObject* gameObject);
	static bool IsExist(GameObject* gameObject);
	static CDataField* GetDataField(GameObject* gameObject);
private:
	CDataField(){}
	GameObject* m_pAttachObject;
	std::vector<CDF*> m_pDatas;
	static std::vector<CDataField*> m_pDataFields;
};

#endif // !_DATA_FIELD_H_
