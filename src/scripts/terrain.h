//=============================================================
//
// 地形 [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "component.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "terrain_type.h"

// 地形
class Terrain
{
public:
	void Init();
	void Uninit();
	void Update();
	void Generate();
	void SetSeed(const unsigned int& seed) { m_seed = seed; }
	unsigned int GetSeed() { return m_seed; }
	float GetMinHeight() { return m_minHeight; }
	float GetMaxHeight() { return m_maxHeight; }
	void LoadTerrainFile(const std::string path);

	// 指定の高度の色を取得する
	D3DXCOLOR GetHeightColor(const float& height);
	// 指定の位置の色を取得する
	D3DXCOLOR GetVertexColor(const int& x, const int& y);
	// 頂点の高さを取得する
	float GetVertexHeight(const int& x, const int& y);

	static const int TERRAIN_SIZE = 150;
	static const float TERRAIN_SCALE;
	static const float TERRAIN_DISTANCE;
	static const float TERRAIN_DISTANCE_HALF;
private:
	void UninitTerrain();
	void GenerateGem();

	// 辺
	enum Area
	{
		NONE,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	Area m_startArea;

	unsigned int m_seed;										// シード値
	TerrainNoise* m_terrainNoise;							// 地形ノイズデータ
	GameObject* m_field;									// メッシュフィールド
	GameObject* m_limitField;								// 制限フィールド
	float* m_terrainData;										// 高度データ
	btHeightfieldTerrainShape* m_terrainShape;	// 地形コリジョンシェイプ
	int m_gemNum;												// ジェムの数

	float m_maxHeight;	// 最高高度
	float m_minHeight;	// 最低高度

	bool m_lakeEnabled;	// 湖の有効状態
	float m_lakeHeight;	// 湖の高さ

	int m_terrainID;			// 地形ID

	// 頂点の高さを設定する
	void SetVertexHeight(const int& x, const int& y, const float& height);

	// 地形を生成する
	void GenerateTerrain();
	// 山道を生成する
	void GenerateRoad();

	std::vector<int[TERRAIN_SIZE][TERRAIN_SIZE]> m_routeData;

	// 高度カラーを追加する
	void AddHeightColor(const float& height, const D3DXCOLOR& color);
	// 傾斜カラーを追加する
	void AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate = 1.0f);

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
