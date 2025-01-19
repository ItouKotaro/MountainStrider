//=============================================================
//
// 道 [road.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ROAD_H_
#define _ROAD_H_

#include "component.h"
#include "component/3d/meshfield.h"

// 道のフィールド
class RoadMesh : public CMeshField
{
public:
	void Draw() override;
};

// 道
class Road : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Draw() override;

	void AddIdx() { m_pointData.resize(m_pointData.size() + 1); }
	void AddPoint(const int& idx, const int& x, const int& y);
	bool IsRoutedByIdx(const int& idx, const int& x, const int& y);
	bool IsRouted(const int& x, const int& y);
	int IsIndexRouted(const int& x, const int& y);
	void Generate();
	void OutputText();
	void SetTexture(const std::string& path) { m_texturePath = path; }
private:
	void CreateRoad(const D3DXVECTOR2& pos, const float& size, const float& angle);
	void CreateJumpStand(const D3DXVECTOR2& pos, const float& angle);

	D3DXVECTOR2 m_currentPos;	// 現在の位置
	int m_currentIdx;						// 現在の地点
	std::string m_texturePath;

	// ポリゴン管理
	std::vector<SingleComponent<RoadMesh>*> m_roadObj;

	// ポイント情報
	struct PointData
	{
		int x;
		int y;
	};
	std::vector<std::vector<PointData>> m_pointData;
};

#endif // !_ROAD_H_
