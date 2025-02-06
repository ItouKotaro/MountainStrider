//=============================================================
//
// ゲームオブジェクト [direction_arrow.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DIRECTION_ARROW_H_
#define _DIRECTION_ARROW_H_

#include "component.h"
#include "component/3d/mesh.h"
#include "component/2d/polygon.h"

class DirectionArrow : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetDestination(const D3DXVECTOR3& destination) { m_destination = destination; }
private:
	D3DXVECTOR3 m_destination;
	float m_angle;
	float m_fadeY;
	GameObject* m_vehicle;
	SingleComponent<CMesh>* m_arrowModel;
};

#endif // !_DIRECTION_ARROW_H_
