//=============================================================
//
// 装飾 [decoration.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DECORATION_H_
#define _DECORATION_H_

#include "scripts/terrain.h"

// 装飾管理
class DecorationManager
{
public:
	void Init(Terrain* terrain);
	void Uninit();
	void Update(const D3DXVECTOR3& pos);

	//@brief デコレーションの種類を追加
	void AddDecorationType(const std::string& path, const int& chance = 1, const float& offsetY = 0.0f);

	//@brief デコレーションを生成する（単体）
	void GenerateDecoration();

	static const int MAX_CHUNK = 20;
	static const float CHUNK_DIVISION;
	static const int DESTROY_LIMIT;
private:
	// デコレーションの種類
	struct DecorationType
	{
		std::string path;		// プレハブのパス
		int chance;				// 確率
		float offsetY;			// Yのオフセット

		// 条件
		float maxSlant;
		float minSlant;
	};

	// デコレーションの設置データ
	struct DecorationData
	{
		DecorationType* type;
		Transform transform;
	};

	// デコレーションのゲームオブジェクト
	struct DecorationObject
	{
		DecorationData* decoDeta;
		DecorationType* decoType;
		GameObject* gameObject;
		int destroyCounter;
	};

	//@brief デコレーションを設置する
	void AddDecorationData(DecorationType* decoType, Transform transform);

	//@brief デコレーションの種類を決める
	DecorationType* RandomDecoType();

	//@brief 現在位置からチャンクを取得する
	void GetChunk(int* x, int* y, const D3DXVECTOR3& pos);

	//@brief チャンクをロードする
	void LoadChunk(const int& x, const int& y);

	//@brief チャンクをアンロードする
	void UnloadChunk(const int& x, const int& y);

	//@brief 設置データのオブジェクトを有効化する
	void ActiveData(DecorationData* decoData);

	//@brief 設置データのオブジェクトを無効化する
	void PassiveData(DecorationData* decoData);

	//@brief 使われていないオブジェクトの破棄カウンター更新
	void UpdateDestroyObjects();

	std::vector<DecorationType*> m_decoType;											// 種類
	std::vector<DecorationData*> m_decoData[MAX_CHUNK][MAX_CHUNK];	// 設置データ（チャンク管理）
	std::vector<DecorationObject*> m_decoObjects;										// オブジェクト
	Terrain* m_terrain;

	GameObject* m_debugText;

	// 前回のチャンク
	int m_oldChunkX, m_oldChunkY;
};

#endif // !_DECORATION_H_
