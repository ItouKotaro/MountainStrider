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
// [CDataManager] オーディオの参照（存在しない場合は作成）
//=============================================================
CDataAudio* CDataManager::RefAudio(const std::string& sPath)
{
	// データが存在するかを検索する
	for (unsigned int i = 0; i < m_apData.size(); i++)
	{
		if (m_apData[i]->GetPath() == sPath &&
			m_apData[i]->GetFormat() == CDataBase::FORMAT::AUDIO)
		{ // 見つかったとき
			CDataAudio* pAudioData = (CDataAudio*)m_apData[i];		// オーディオクラスにダウンキャスト
			return pAudioData;
		}
	}

	// データが存在しない場合 ---------------------------------------------------------------------------------------------

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// データクラスを生成
	CDataAudio* pNewData = new CDataAudio();
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



//=============================================================
// [CDataAudio] コンストラクタ
//=============================================================
CDataAudio::CDataAudio() : CDataBase(AUDIO)
{
	// バッファを生成
	alGenBuffers(1, &m_buffer);
}

//=============================================================
// [CDataAudio] クリア
//=============================================================
void CDataAudio::Clear()
{
	// バッファの破棄
	alDeleteBuffers(1, &m_buffer);
}

//=============================================================
// [CDataAudio] WAVEファイルを読み込む
//=============================================================
HRESULT CDataAudio::Load(const std::string& sPath)
{
	m_fp.open(sPath.c_str(), std::ios::binary);

	// "RIFF" の読み込み
	unsigned int riff;
	m_fp.read((char*)&riff, 4);

	// データサイズを取得
	m_fp.read((char*)&m_DataSize, 4);

	// WAVEの読み込み
	unsigned int wave;
	m_fp.read((char*)&wave, 4);

	// PCM 情報とデータの先頭の取得
	for (int i = 0; i < 2; ++i)
	{
		unsigned int res, size;
		m_fp.read((char*)&res, 4);
		m_fp.read((char*)&size, 4);
		if (0x20746d66u == res)
		{
			// PCM 情報の取得
			unsigned short res16;
			m_fp.read((char*)&res16, 2);
			if (1 != res16)
			{// 非対応フォーマット
				return E_FAIL;
			}

			// モノラル(1), ステレオ(2)
			m_fp.read((char*)&m_ChannelQuantity, 2);
			if (2 < m_ChannelQuantity)
			{
				return E_FAIL;
			}

			// サンプリングレート
			m_fp.read((char*)&m_SamplingRate, 4);

			// 1秒あたりのバイト数(byte/sec)
			m_fp.read((char*)&m_Bps, 4);

			// ブロックサイズ(byte/sample)
			m_fp.read((char*)&m_BlockSize, 2);

			// サンプルあたりのビット数(bit/sample)
			m_fp.read((char*)&m_BitPerSample, 2);
		}
		else if (0x61746164u == res)
		{
			// データの開始位置を保存
			m_DataHead = m_fp.tellg();
			m_DataSize = size;

			// データを読み飛ばす
			m_fp.seekg(size, std::ios::cur);
		}
	}

	// データの開始位置までシーク
	m_fp.seekg(m_DataHead);
	m_LoadedSize = 0;
	m_DataSize -= m_DataSize % 100;
	m_Length = m_DataSize / m_Bps;

	std::vector<char> wavData(m_DataSize);
	int read = Read(&wavData[0], m_DataSize);
	ALenum format;
	if (1 == m_ChannelQuantity)
	{ // モノラル
		format = (8 == m_BitPerSample) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	}
	else
	{ // ステレオ
		format = (8 == m_BitPerSample) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	}

	// OpenAL のバッファにデータの書き込み
	alBufferData(m_buffer, format, &wavData[0], read, m_SamplingRate);

	wavData.clear();
	m_fp.close();

	return true;
}

//=============================================================
// [CDataAudio] 読み込む
//=============================================================
int CDataAudio::Read(void* out, size_t size)
{
	if (!out)
	{
		return 0;
	}

	// データサイズの調整
	if (m_LoadedSize + size > m_DataSize) {
		size = m_DataSize - m_LoadedSize;
	}

	// データを読み出し書き込む
	m_fp.read((char*)out, size);
	m_LoadedSize += size;
	if (m_LoadedSize == m_DataSize) {
		m_LoadedSize = 0;
		m_fp.seekg(m_DataHead);
	}
	return size;
}