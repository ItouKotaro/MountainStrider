//=============================================================
//
// �� [road.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ROAD_H_
#define _ROAD_H_

#include "component.h"
#include "component/3d/meshfield.h"

// ��
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

	D3DXVECTOR2 m_currentPos;	// ���݂̈ʒu
	int m_currentIdx;						// ���݂̒n�_
	std::string m_texturePath;

	// �|���S���Ǘ�
	std::vector<SingleComponent<CMeshField>*> m_roadObj;

	// �|�C���g���
	struct PointData
	{
		int x;
		int y;
	};
	std::vector<std::vector<PointData>> m_pointData;
};

#endif // !_ROAD_H_
