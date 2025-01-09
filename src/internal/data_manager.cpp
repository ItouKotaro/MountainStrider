//=============================================================
//
// データ管理 [data_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#include "data_manager.h"
#include "manager.h"
using namespace std;

//=============================================================
// [CDataManager] コンストラクタ
//=============================================================
CDataManager::CDataManager()
{
	
}

//=============================================================
// [CDataManager] デストラクタ
//=============================================================
CDataManager::~CDataManager()
{

}

//=============================================================
// [CDataManager] 初期化
//=============================================================
void CDataManager::Init()
{

}

//=============================================================
// [CDataManager] 終了
//=============================================================
void CDataManager::Uninit()
{
	// すべてのデータを破棄する
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i] != nullptr)
		{
			// 格納データを破棄する
			m_apData[i]->Clear();

			// データを破棄する
			delete m_apData[i];
			m_apData[i] = nullptr;
		}
	}

	// データリストを空にする
	m_apData.clear();
}

//=============================================================
// [CDataManager] テクスチャの参照（存在しない場合は作成）
//=============================================================
CDataTexture* CDataManager::RefTexture(const std::string& sPath)
{
	// データが存在するかを検索する
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i]->GetPath() == sPath &&
			m_apData[i]->GetFormat() == CDataBase::FORMAT::TEXTURE)
		{ // 見つかったとき
			CDataTexture* pTextureData = (CDataTexture*)m_apData[i];	// テクスチャクラスにダウンキャスト
			return pTextureData;
		}
	}

	// データが存在しない場合 ---------------------------------------------------------------------------------------------

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// データクラスを生成
	CDataTexture* pNewData = new CDataTexture();
	pNewData->SetPath(sPath);

	// ロード
	if (FAILED(pNewData->Load(pNewData->GetPath())))
	{ // 失敗
		delete pNewData;
		pNewData = nullptr;
		return nullptr;
	}

	m_apData.push_back(pNewData);		// データを登録する

	return pNewData;
}

//=============================================================
// [CDataManager] メッシュの参照（存在しない場合は作成）
//=============================================================
CDataMesh* CDataManager::RefMesh(const std::string& sPath)
{
	// データが存在するかを検索する
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i]->GetPath() == sPath &&
			m_apData[i]->GetFormat() == CDataBase::FORMAT::MESH)
		{ // 見つかったとき
			CDataMesh* pMeshData = (CDataMesh*)m_apData[i];		// メッシュクラスにダウンキャスト
			return pMeshData;
		}
	}

	// データが存在しない場合 ---------------------------------------------------------------------------------------------

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// データクラスを生成
	CDataMesh* pNewData = new CDataMesh();
	pNewData->SetPath(sPath);

	// ロード
	if (FAILED(pNewData->Load(pNewData->GetPath())))
	{ // 失敗
		delete pNewData;
		pNewData = nullptr;
		return nullptr;
	}

	m_apData.push_back(pNewData);		// データを登録する

	return pNewData;
}

//=============================================================
// [CDataManager] データの削除
//=============================================================
void CDataManager::RemoveData(const std::string& path)
{
	// すべてのデータを破棄する
	for (auto itr = m_apData.begin(); itr != m_apData.end(); itr++)
	{
		if ((*itr)->GetPath() == path)
		{
			(*itr)->Clear();
			delete* itr;
			*itr = nullptr;
			m_apData.erase(itr);
			return;
		}
	}
}


//=============================================================
// [CDataBase] コンストラクタ
//=============================================================
CDataBase::CDataBase(const FORMAT& format) : m_format(format)
{
}

//=============================================================
// [CDataBase] フォーマットの取得
//=============================================================
CDataBase::FORMAT CDataBase::GetFormat()
{
	return m_format;	// フォーマットを返す
}

//=============================================================
// [CDataBase] パスの設定
//=============================================================
void CDataBase::SetPath(const std::string& sPath)
{
	m_sPath = sPath;
}

//=============================================================
// [CDataBase] パスの取得
//=============================================================
string CDataBase::GetPath()
{
	return m_sPath;
}





//=============================================================
// [CDataTexture] コンストラクタ
//=============================================================
CDataTexture::CDataTexture() : CDataBase(TEXTURE)
{
	// 変数の初期化
	m_texture = nullptr;
}

//=============================================================
// [CDataTexture] クリア
//=============================================================
void CDataTexture::Clear()
{
	// テクスチャの破棄
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

//=============================================================
// [CDataTexture] ロード
//=============================================================
HRESULT CDataTexture::Load(const std::string& sPath)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// テクスチャの作成
	if (FAILED(D3DXCreateTextureFromFile(pDevice, sPath.c_str(), &m_texture)))
	{ // 失敗
		return E_FAIL;
	}

	// 成功
	return S_OK;
}




//=============================================================
// [CDataMesh] コンストラクタ
//=============================================================
CDataMesh::CDataMesh() : CDataBase(MESH)
{
	// 変数の初期化
	m_pMesh = nullptr;
	m_pShadowMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
}

//=============================================================
// [CDataMesh] クリア
//=============================================================
void CDataMesh::Clear()
{
	// メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// 影用メッシュの破棄
	if (m_pShadowMesh != nullptr)
	{
		m_pShadowMesh->Release();
		m_pShadowMesh = nullptr;
	}

	// マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//=============================================================
// [CDataMesh] ロード
//=============================================================
HRESULT CDataMesh::Load(const std::string& sPath)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// メッシュの生成
	if (FAILED(D3DXLoadMeshFromX(
		sPath.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		nullptr,
		&m_pBuffMat,
		nullptr,
		&m_dwNumMat,
		&m_pMesh
	)))
	{ // 失敗
		return E_FAIL;
	}

	// 影用メッシュの生成
	if (CShadow::USE_SHADOW)
	{
		if (FAILED(CShadow::GenerateShadowMesh(pDevice, m_pMesh, &m_pShadowMesh)))
		{ // 失敗
			return E_FAIL;
		}
	}

	// 成功
	return S_OK;
}