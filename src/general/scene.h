//=============================================================
//
// シーン管理 [scene.h]
// Author: Ito Kotaro
// 
// 説明: シーンの管理を行います
//
//=============================================================
#ifndef _SCENE_H_
#define _SCENE_H_

// シーンの基底クラス
class CScene
{
public:
	virtual ~CScene() {}			// デストラクタ
	virtual void Init() {}			// 初期化
	virtual void Uninit() {}		// 終了
	virtual void Update() {}		// 更新
	virtual void Draw() {}			// 描画
	virtual void LastUninit() {}	// 最終終了
};

// シーンマネージャー
class CSceneManager
{
public:
	CSceneManager();		// コンストラクタ

	/*
	@brief シーン情報
	@param pScene : シーンのポインタ
	@param sName : シーン名
	*/
	typedef struct
	{
		CScene* pScene;		// シーンのポインタ
		std::string sName;		// シーン名
	}SceneData;

	void Init();				// 初期化
	void Uninit();			// 終了
	void Update();		// 更新
	void Draw();			// 描画

	/*
	@brief シーンの切り替え（設定）
	@param [in] sName : シーン名
	@param [in] bIsDestroy : 破棄するか
	@return シーン情報
	*/
	HRESULT SetScene(std::string sName, bool bIsDestroy = true);

	/*
	@brief 現在のシーンの再読み込み
	*/
	void ReloadScene();

	/*
	@brief 前シーンの終了、次シーンの初期化（安全性を守るため、実行しないでください）
	*/
	void ChangingScene();

	/*
	@brief シーンの登録 例: RegistScene<CAScene>("a");
	@param [template] : シーンクラス
	@param [in] sName : シーン名
	*/
	template<class T> void RegistScene(std::string sName)
	{
		// 同じポインタが存在しないかをチェック
		for (unsigned int i = 0; i < m_apScene.size(); i++)
		{
			if (m_apScene[i]->sName == sName)
			{ // 同じ名前が見つかったとき
				return;	// 失敗
			}
		}

		// シーンデータを作成する
		SceneData* pData = new SceneData();
		pData->pScene = new T();		// ポインタ
		pData->sName = sName;		// 名前

		// 登録する
		m_apScene.push_back(pData);
	}

	/*
	@brief シーンの取得
	@param [in] sName : シーン名
	@return シーン情報
	*/
	SceneData* GetScene(std::string sName);

	/*
	@brief 現在のシーンの取得
	@return シーン情報
	*/
	SceneData* GetCurrentScene() { return m_pCurrentScene; }

	/*
	@brief インスタンスの取得
	@return インスタンス
	*/
	static CSceneManager* GetInstance()
	{
		static CSceneManager instance;
		return &instance;
	}

	static const std::string START_SCENE_NAME;		// スタートシーンの名前
private:
	SceneData* m_pCurrentScene;					// 現在のシーン
	SceneData* m_pNextScene;						// 次のシーン（予定）
	bool m_bNoDestroy;									// オブジェクトを破棄するか
	bool m_bReload;										// リロードトリガー
	std::vector<SceneData*> m_apScene;		// シーン管理
};

#endif // !_SCENE_H_
