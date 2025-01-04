//=============================================================
//
// �O�� [trajectory.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

#include "component.h"

class CTrajectory : public Component
{
public:
	CTrajectory() : m_bShow(false), m_pVtxBuffTrajectory(nullptr), m_pTextureTrajectory(nullptr), m_pIdxBuffTrajectory(nullptr){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void AddTrajectory(const D3DXVECTOR3& root, const D3DXVECTOR3& tip);
	void SetShow(const bool& bEnabled);
	bool GetShow() { return m_bShow; }
	void ResetPos(const D3DXVECTOR3& pos);

	// �O���̍��W�ۑ��p�\����
	struct TrajectoryData
	{
		D3DXVECTOR3 rootPos;		// �����̍��W
		D3DXVECTOR3 tipPos;		// ��[�̍��W
	};

	static const int MAX_FRAME = 40;
	static const float FADE;
	static const char* TEX_PATH;
private:
	bool m_bShow;																	// �\�����邩
	TrajectoryData m_trajectoryData[MAX_FRAME];
	D3DXMATRIX m_mtxWorldTrajectory;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffTrajectory;
	LPDIRECT3DTEXTURE9 m_pTextureTrajectory;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffTrajectory;
};

#endif // !_TRAJECTORY_H_
