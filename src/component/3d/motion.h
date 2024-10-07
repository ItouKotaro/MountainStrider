//=============================================================
//
// モーション [motion.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "transform.h"
#include "component.h"

// モーション内容クラスの定義
class CMotionContents
{
public:
	CMotionContents(int nPartsNum);			// コンストラクタ

	// キー構造体
	typedef struct
	{
		int nKeyFrame;				// キーの位置
		Transform transform;		// トランスフォーム
	}Key;

	// キーの追加
	void AddKey(int nPartsIdx, int nKey,
		D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f }, 
		D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f }, 
		D3DXVECTOR3 scale = { 1.0f, 1.0f, 1.0f });
	void RemoveKey(int nPartsIdx, int nKey);				// キーの削除
	void SortKey(int nPartsIdx);								// ソートする

	// 取得
	std::string GetName() { return m_name; }								// 名前を取得
	int GetMaxFrame() { return m_nMaxFrame; }							// 最大フレーム数の取得
	bool GetLoop() { return m_bLoop; }										// ループの取得
	Transform GetTransform(int nPartsIdx, int nKey);						// キー位置のトランスフォーム値を取得
	std::vector<std::vector<Key>>& GetKey() { return m_partsKey; }

	// 設定
	void SetName(std::string ContentsName) {	// 名前変更
		m_name = ContentsName;
	}
	void SetMaxFrame(int nMaxFrame) {									// 最大フレーム数の設定
		if (nMaxFrame >= 0)
		{
			m_nMaxFrame = nMaxFrame;
		}
	}
	void SetLoop(bool bLoop) { m_bLoop = bLoop; }					// ループの設定

private:
	std::string m_name;															// モーション名
	int m_nMaxFrame;																// 最大フレーム
	bool m_bLoop;																	// ループ
	std::vector<std::vector<Key>>	m_partsKey;						// パーツごとのキー
};


// モーションレイヤークラスの定義
class CMotionLayer
{
public:
	CMotionLayer(int* pBlendFrame);										// コンストラクタ
	void Update();																	// 更新

	void AddParts(int nPartsIdx);												// パーツを追加する
	void RemoveParts(int nPartsIdx);											// パーツを削除する
	void SetMotion(CMotionContents* pContents);

	// 取得
	char* GetName() { return &m_name[0]; }							// 名前を取得
	std::vector<int> GetPartsList() { return m_partsIdx; }			// パーツリストを取得
	int GetKeyFrame() { return m_nKeyFrame; }						// キーフレームを取得
	void SetKeyFrame(int nKeyFrame) { m_nKeyFrame = nKeyFrame; }
	CMotionContents* GetPlayContents() {									// 再生中のコンテンツを取得
		return m_pPlayContents; 
	}
	bool GetEnded() { return m_bEnded; }								// 終了しているか
	
	// 設定
	void SetName(std::string LayerName) {								// 名前変更
		m_name = LayerName;
	}

private:
	std::string m_name;															// レイヤー名
	std::vector<int> m_partsIdx;												// 含まれるパーツインデックス
	CMotionContents* m_pPlayContents;									// 再生中のモーション
	int m_nKeyFrame;																// キーフレーム
	int m_nBlendCounter;															// ブレンドカウンター
	int* m_pBlendFrame;															// ブレンドフレームの取得用ポインタ

	bool m_bEnded;																	// モーションが終了しているか

	// ブレンドのための前回のモーション
	CMotionContents* m_pBeforeContents;								// 前回のコンテンツ
	int m_nBeforeKeyFrame;													// 前回のキーフレーム
};

// モーションクラスの定義
class CMotionManager : public Component
{
public:
	CMotionManager(int nPartsNum);				// コンストラクタ
	~CMotionManager();									// デストラクタ

	void Update() override;								// 更新
	void OnDestroyOtherObject(GameObject* other) override;								// 消失イベント

	CMotionLayer* AddLayer(std::string	 LayerName);											// レイヤーの追加
	void RemoveLayer(std::string	LayerName);														// レイヤーの削除
	CMotionLayer* FindLayer(std::string LayerName);											// レイヤーの検索

	CMotionContents* AddContents(std::string ContentsName);								// コンテンツの追加
	void RemoveContents(std::string ContentsName);											// コンテンツの削除
	CMotionContents* FindContents(std::string ContentsName);								// コンテンツの検索

	void Play(std::string ContentsName, std::string LayerName = "Base");				// 再生
	void Stop(std::string LayerName = "Base");														// 停止
	void Stop(CMotionLayer* pLayer);																	// 停止
	void StopAll();																								// すべて停止

	void Resize(int nPartsNum);
	void SetParts(int nPartsIdx, GameObject* pParts);											// パーツの設定
	void SetOffset(const int& nPartsIdx, Transform offset);										// オフセットの設定
	std::vector<GameObject*>& GetParts() { return m_partsObject; }					// パーツリストの取得
	std::vector<CMotionContents*>& GetContents() { return m_contents; }			// コンテンツの取得
	Transform GetTransform(int nPartsIdx);															// トランスフォーム値を取得

private:
	std::vector<CMotionLayer*> m_layer;								// レイヤー
	std::vector<CMotionLayer*> m_playingLayer;					// 再生中のレイヤー（後半にある方が優先される）
	std::vector<CMotionContents*> m_contents;					// モーション内容
	std::vector<GameObject*> m_partsObject;						// パーツのトランスフォームのポインタ
	std::vector<Transform> m_offset;									// オフセット情報
	std::vector<Transform> m_transform;							// トランスフォーム情報（結果）
	int m_nBlendFrame;														// ブレンドフレーム数
	int m_nPartsNum;															// パーツ数

	// モーションの状況データ
	struct MotionStateData
	{
		CMotionContents* pContents;
		int nKeyFrame;
		int nBlendCounter;
	};
	std::vector<MotionStateData> m_motionData;
	std::vector<MotionStateData> m_blendData;
};


#endif // !_MOTION_H_
