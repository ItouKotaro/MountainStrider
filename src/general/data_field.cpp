//=============================================================
//
// データフィールド [data_field.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "data_field.h"

// 静的メンバ変数の初期化
std::vector<CDataField*> CDataField::m_pDataFields;

//=============================================================
// [CDataField] デストラクタIntデータの取得
//=============================================================
CDataField::~CDataField()
{
	if (m_pDatas.size() > 0)
	{ // データが存在するとき
		// データを削除する
		for (int i = 0; i < (int)m_pDatas.size(); i++)
		{
			delete m_pDatas[i];
			m_pDatas[i] = nullptr;
		}
	}

	// 空にする
	m_pDatas.clear();

	// データフィールドリストから削除する
	for (int i = 0; i < (int)m_pDataFields.size(); i++)
	{
		if (m_pDataFields[i] == this)
		{ // 一致するとき
			m_pDataFields.erase(m_pDataFields.begin() + i);
			return;
		}
	}
}

//=============================================================
// [CDataField] Intデータの取得
//=============================================================
int CDataField::GetInt(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (m_pDatas[i]->GetName() == name &&
			m_pDatas[i]->GetType() == CDF::TYPE::INT)
		{
			CDF_Int* data = (CDF_Int*)m_pDatas[i];
			return  data->GetValue();
		}
	}

	return 0;
}

//=============================================================
// [CDataField] Floatデータの取得
//=============================================================
float CDataField::GetFloat(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (m_pDatas[i]->GetName() == name &&
			m_pDatas[i]->GetType() == CDF::TYPE::FLOAT)
		{
			CDF_Float* data = (CDF_Float*)m_pDatas[i];
			return  data->GetValue();
		}
	}
	return 0.0f;
}

//=============================================================
// [CDataField] Boolデータの取得
//=============================================================
bool CDataField::GetBool(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (m_pDatas[i]->GetName() == name &&
			m_pDatas[i]->GetType() == CDF::TYPE::BOOL)
		{
			CDF_Bool* data = (CDF_Bool*)m_pDatas[i];
			return  data->GetValue();
		}
	}
	return false;
}

//=============================================================
// [CDataField] Stringデータの取得
//=============================================================
std::string CDataField::GetString(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (m_pDatas[i]->GetName() == name &&
			m_pDatas[i]->GetType() == CDF::TYPE::STRING)
		{
			CDF_String* data = (CDF_String*)m_pDatas[i];
			return  data->GetValue();
		}
	}
	return "";
}

//=============================================================
// [CDataField] Vector3データの取得
//=============================================================
D3DXVECTOR3 CDataField::GetVector3(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (m_pDatas[i]->GetName() == name &&
			m_pDatas[i]->GetType() == CDF::TYPE::VECTOR3)
		{
			CDF_Vector3* data = (CDF_Vector3*)m_pDatas[i];
			return  data->GetValue();
		}
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================
// [CDataField] Vector2データの取得
//=============================================================
D3DXVECTOR2 CDataField::GetVector2(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (m_pDatas[i]->GetName() == name &&
			m_pDatas[i]->GetType() == CDF::TYPE::VECTOR2)
		{
			CDF_Vector2* data = (CDF_Vector2*)m_pDatas[i];
			return  data->GetValue();
		}
	}
	return D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================
// [CDataField] データの追加
//=============================================================
void CDataField::AddData(CDF* pData)
{
	// 名前が重複していないか
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (pData->GetName() == m_pDatas[i]->GetName())
		{ // 重複している場合
			return;
		}
	}

	// データを追加する
	m_pDatas.push_back(pData);
}

//=============================================================
// [CDataField] データの取得
//=============================================================
CDF* CDataField::GetData(const std::string& name)
{
	for (int i = 0; i < (int)m_pDatas.size(); i++)
	{
		if (name == m_pDatas[i]->GetName())
		{ // 見つかった場合
			return m_pDatas[i];
		}
	}
	return nullptr;
}

//=============================================================
// [CDataField] データフィールドが存在するか
//=============================================================
bool CDataField::IsExist(GameObject* gameObject)
{
	for (int i = 0; i < (int)m_pDataFields.size(); i++)
	{
		if (m_pDataFields[i]->m_pAttachObject == gameObject)
		{ // 存在するとき
			return true;
		}
	}
	return false;
}

//=============================================================
// [CDataField] データフィールドの取得
//=============================================================
CDataField* CDataField::GetDataField(GameObject* gameObject)
{
	// 探す
	for (int i = 0; i < (int)m_pDataFields.size(); i++)
	{
		if (m_pDataFields[i]->m_pAttachObject == gameObject)
		{
			return m_pDataFields[i];
		}
	}

	// 存在しない場合、作成する
	CDataField* pDataField = new CDataField();
	pDataField->m_pAttachObject = gameObject;
	m_pDataFields.push_back(pDataField);

	return pDataField;
}

//=============================================================
// [CDataField] データを破棄する
//=============================================================
void CDataField::Destroy(GameObject* gameObject)
{
	if (IsExist(gameObject))
	{ // 存在するとき
		delete GetDataField(gameObject);
	}
}
