//=============================================================
//
// データ管理 [data_manager.h]
// Author: Ito Kotaro
// 
// テクスチャやメッシュなどのデータ参照元として管理します
//
//=============================================================
#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <fstream>

// データ基底クラス
class CDataBase
{
public:
	typedef enum
	{
		TEXTURE = 0,		// テクスチャ
		MESH,					// メッシュ
	}FORMAT;

	CDataBase(const FORMAT& format);
	virtual void Clear() = 0;							// 格納データ削除

	FORMAT GetFormat();
	void SetPath(const std::string& sPath);	// パスの設定
	std::string GetPath();								// パスの取得
private:
	std::string m_sPath;		// パス
	FORMAT m_format;		// フォーマット
};

// テクスチャデータクラス
class CDataTexture : public CDataBase
{
public:
	CDataTexture();
	void Clear() override;
	HRESULT Load(const std::string& sPath);
	LPDIRECT3DTEXTURE9 GetTexture() { return m_texture; }			// テクスチャの取得
	
private:
	LPDIRECT3DTEXTURE9 m_texture;												// テクスチャデータ
};

// メッシュデータクラス
class CDataMesh : public CDataBase
{
public:
	CDataMesh();
	void Clear() override;
	HRESULT Load(const std::string& sPath);
	LPD3DXMESH GetMesh() { return m_pMesh; }								// メッシュの取得
	LPD3DXMESH GetShadowMesh() { return m_pShadowMesh; }		// 影用メッシュの取得
	LPD3DXBUFFER GetBuffMat() { return m_pBuffMat; }					// マテリアルの取得
	DWORD GetNumMat() { return m_dwNumMat; }							// マテリアル数の取得
private:
	LPD3DXMESH m_pMesh;															// メッシュデータ
	LPD3DXMESH m_pShadowMesh;													// 影用メッシュデータ
	LPD3DXBUFFER m_pBuffMat;														// マテリアルデータ
	DWORD m_dwNumMat;																// マテリアル数
};

// データ管理クラス
class CDataManager
{
public:
	CDataManager();
	~CDataManager();
	void Init();
	void Uninit();

	static CDataManager* GetInstance()
	{
		static CDataManager dataManager;
		return &dataManager;
	}

	CDataTexture* RefTexture(const std::string& sPath);
	CDataMesh* RefMesh(const std::string& sPath);
	void RemoveData(const std::string& path);
private:
	std::vector<CDataBase*> m_apData;		// 全データ格納
};

#endif // !_DATA_MANAGER_H_
