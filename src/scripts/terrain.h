//=============================================================
//
// 地形 [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "nature_produces.h"
#include "component.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// 生成物管理（必要最低限のゲームオブジェクトで管理する）
// 詳細: 同じ生成物で使われていない場合はゲームオブジェクトを使い回す
class ProducesManager
{
public:
	// 生成物データを追加する
	void AddProduce(const Transform& transform, CNatureProduces* pNatureProduce);
	// 初期化
	void Init();
	// 破棄
	void Uninit();
	// 更新
	void Update(const D3DXVECTOR3& pos);

	// 指定位置の周囲のオブジェクトを取得する
	float GetNearProducesRate(const std::string& name , const D3DXVECTOR3& pos, const float& range);

	// 指定位置から指定の範囲に生成物が存在するか
	bool FindProducesByDistance(const D3DXVECTOR3& pos, const float& distance);
private:
	void UpdateGameObjects(const D3DXVECTOR3& pos);

	// ゲームオブジェクトデータ
	struct ManagedGameObject
	{
		GameObject* gameObject;
		CNatureProduces* natureProduce;
	};

	// 生成物管理データ（配置情報）
	struct ManagedProduce
	{
		Transform transform;
		CNatureProduces* natureProduce;
		ManagedGameObject* managedGameObject;	// 配置ゲームオブジェクト
	};

	std::vector<ManagedProduce*> m_managedProduces;				// 管理されている生成配置物
	std::vector<ManagedGameObject*> m_managedGameObjects;	// 管理されているゲームオブジェクト

	GameObject* m_pVehicle;		// バイク格納

	// デバッグ用
	GameObject* m_pNumObj;	// オブジェクトの総数テキスト
};

// 地形
class Terrain
{
public:
	void Init();
	void Uninit();
	void Update(const D3DXVECTOR3& pos);
	void Generate();
	void SetSeed(const unsigned int& seed) { m_seed = seed; }
	unsigned int GetSeed() { return m_seed; }
	float GetMinHeight() { return m_minHeight; }
	float GetMaxHeight() { return m_maxHeight; }
	void LoadTerrainFile(const std::string path);

	ProducesManager* GetProducesManager() { return m_producesManager; }

	static const int TERRAIN_SIZE = 150;
	static const float TERRAIN_SCALE;
private:
	void UninitTerrain();

	unsigned int m_seed;										// シード値
	GameObject* m_pField;									// メッシュフィールド
	float* m_terrainData;										// 高度データ
	btHeightfieldTerrainShape* m_terrainShape;	// 地形コリジョンシェイプ

	float m_maxHeight;	// 最高高度
	float m_minHeight;	// 最低高度

	// 頂点の高さを取得する
	float GetVertexHeight(const int& x, const int& y);

	// 地形を生成する
	void GenerateTerrain();
	// 山道を生成する
	void GenerateRoad();
	// 生成物を生成する
	void GenerateProduces();
	// 生成物を登録する
	void RegisterProduces(CNatureProduces* pNatureProduce);

	std::vector<CNatureProduces*> m_registerNatureProduces;		// 生成物リスト（登録）
	ProducesManager* m_producesManager;									// 生成物管理
	bool m_routeData[TERRAIN_SIZE][TERRAIN_SIZE];
	

	// 高度カラーを追加する
	void AddHeightColor(const float& height, const D3DXCOLOR& color);
	// 傾斜カラーを追加する
	void AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate = 1.0f);
	// 指定の高度の色を取得する
	D3DXCOLOR GetHeightColor(const float& height);
	// 指定の位置の色を取得する
	D3DXCOLOR GetVertexColor(const int& x, const int& y);

	// 高度カラー
	struct HeightColor
	{
		float height;				// 高度
		D3DXCOLOR color;		// 色
	};
	std::vector<HeightColor> m_heightColor;

	// 角度カラー
	struct SlantColor
	{
		float minHeight;			// 高度範囲（最小）
		float maxHeight;		// 高度範囲（最大）
		D3DXCOLOR color;		// 色
		float rate;					// 適用割合
	};
	std::vector<SlantColor> m_slantColor;
};

#endif // !_TERRAIN_H_
