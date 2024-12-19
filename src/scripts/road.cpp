//=============================================================
//
// 道 [road.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "road.h"
#include "terrain.h"
#include <fstream>

//=============================================================
// [Road] 初期化
//=============================================================
void Road::Init()
{

}

//=============================================================
// [Road] 終了
//=============================================================
void Road::Uninit()
{
	m_pointData.clear();

	for (auto itr = m_roadObj.begin(); itr != m_roadObj.end(); itr++)
	{
		(*itr)->Uninit();
		delete *itr;
	}
	m_roadObj.clear();
}

//=============================================================
// [Road] 描画
//=============================================================
void Road::Draw()
{
	for (auto itr = m_roadObj.begin(); itr != m_roadObj.end(); itr++)
	{
		(*itr)->SetShader(GetShaderInfo());
		(*itr)->Draw();
	}
}

//=============================================================
// [Road] ポイントの追加
//=============================================================
void Road::AddPoint(const int& idx, const int& x, const int& y)
{
	PointData data;
	data.x = x;
	data.y = y;
	m_pointData[idx].push_back(data);
}

//=============================================================
// [Road] すでにルートか（インデックス指定）
//=============================================================
bool Road::IsRoutedByIdx(const int& idx, const int& x, const int& y)
{
	for (auto itr = m_pointData[idx].begin(); itr != m_pointData[idx].end(); itr++)
	{
		if (x == (*itr).x && y == (*itr).y)
		{
			return true;
		}
	}
	return false;
}

//=============================================================
// [Road] すでにルートか
//=============================================================
bool Road::IsRouted(const int& x, const int& y)
{
	for (auto itr = m_pointData.begin(); itr != m_pointData.end(); itr++)
	{
		for (auto itrP = (*itr).begin(); itrP != (*itr).end(); itrP++)
		{
			if (x == (*itrP).x && y == (*itrP).y)
			{
				return true;
			}
		}
	}
	return false;
}

//=============================================================
// [Road] すでにルートか（ルートのインデックスを返す）
//=============================================================
int Road::IsIndexRouted(const int& x, const int& y)
{
	int idx = 0;
	for (auto itr = m_pointData.begin(); itr != m_pointData.end(); itr++, idx++)
	{
		for (auto itrP = (*itr).begin(); itrP != (*itr).end(); itrP++)
		{
			if (x == (*itrP).x && y == (*itrP).y)
			{
				return idx;
			}
		}
	}
	return -1;
}

//=============================================================
// [Road] 生成
//=============================================================
void Road::Generate()
{
	for (auto itr = m_pointData.begin(); itr != m_pointData.end(); itr++)
	{
		if ((*itr).empty())
			return;

		// 初期地点を設定する
		m_currentIdx = 0;
		m_currentPos.x = (*itr)[m_currentIdx].x;
		m_currentPos.y = (*itr)[m_currentIdx].y;

		// 目標地点を設定する（次のポイントが存在するとき）
		while (m_currentIdx + 1 < (*itr).size())
		{
			PointData nextData = (*itr)[m_currentIdx + 1];
			float halfField = (Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE) / static_cast<float>(2.0f);
			float halfScale = Terrain::TERRAIN_SCALE / static_cast<float>(2.0f);
			D3DXVECTOR2 nextPos = { Terrain::TERRAIN_SCALE * nextData.x - halfField, Terrain::TERRAIN_SCALE * nextData.y - halfField };

			// 目標地点の方向に移動する
			D3DXVECTOR2 dir = { (nextPos.x + halfScale) - m_currentPos.x, (nextPos.y + halfScale) - m_currentPos.y };
			D3DXVec2Normalize(&dir, &dir);		// 目標地点への方向ベクトル
			m_currentPos += dir * 70.0f;

			// 設置する
			CreateRoad(m_currentPos, 180.0f, 0.0f);

			// 目標地点に到着しているかを確認する
			if (nextPos.x <= m_currentPos.x && m_currentPos.x <= nextPos.x + Terrain::TERRAIN_SCALE &&
				nextPos.y <= m_currentPos.y && m_currentPos.y <= nextPos.y + Terrain::TERRAIN_SCALE)
			{
				// 次の地点を目標に設定する
				m_currentIdx++;
			}
		}
	}
}

//=============================================================
// [Road] 道の生成
//=============================================================
void Road::CreateRoad(const D3DXVECTOR2& pos, const float& size, const float& angle)
{
	// レイでメッシュの位置を割り出す
	D3DXVECTOR2 vtxPos[4];
	vtxPos[0] = { -size / 2 , size / 2 };
	vtxPos[1] = { size / 2 , size / 2 };
	vtxPos[2] = { -size / 2 , -size / 2 };
	vtxPos[3] = { size / 2 , -size / 2 };

	// 回転
	D3DXMATRIX rotMtx;
	D3DXMatrixRotationY(&rotMtx, angle);
	for (int i = 0; i < 4; i++)
	{
		D3DXVECTOR3 transVec = { vtxPos[i].x, 0.0f, vtxPos[i].y };
		D3DXVec3TransformCoord(&transVec, &transVec, &rotMtx);
		vtxPos[i] = { transVec.x, transVec.z };
		vtxPos[i] += m_currentPos;
	}

	// 高度格納用
	float vtxHeight[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	// レイで頂点座標を決める
	for (int i = 0; i < 4; i++)
	{
		btVector3 Start = btVector3(vtxPos[i].x, 3000.0f, vtxPos[i].y);
		btVector3 End = Start + btVector3(0.0f, -6000.0f, 0.0f);

		// レイ
		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit())
		{ // ヒットしたとき
			vtxHeight[i] = RayCallback.m_hitPointWorld.getY();
		}
		else
		{ // 失敗
			return;
		}
	}

	// 地面を生成する
	SingleComponent<CMeshField>* groundMesh = new SingleComponent<CMeshField>();
	groundMesh->Init();
	groundMesh->Create(1, 1, size);
	groundMesh->transform->SetPos({ pos.x, 3.0f, pos.y });
	groundMesh->SetHeight(0, 0, vtxHeight[0]);
	groundMesh->SetHeight(1, 0, vtxHeight[1]);
	groundMesh->SetHeight(0, 1, vtxHeight[2]);
	groundMesh->SetHeight(1, 1, vtxHeight[3]);
	groundMesh->SetTexture("data\\TEXTURE\\ROAD\\dirt.png");
	m_roadObj.push_back(groundMesh);
}

//=============================================================
// [Road] テキストファイルに出力
//=============================================================
void Road::OutputText()
{
	// ファイルに経路データを書き出す
	std::ofstream outputRoute("route.txt");
	for (int y = 0; y < Terrain::TERRAIN_SIZE; y++)
	{
		for (int x = 0; x < Terrain::TERRAIN_SIZE; x++)
		{
			int num = -1;
			for (auto itr = m_pointData.begin(); itr != m_pointData.end(); itr++)
			{
				int count = 0;
				for (auto itrIdx = (*itr).begin(); itrIdx != (*itr).end(); itrIdx++, count++)
				{
					if ((*itrIdx).x == x && (*itrIdx).y == y)
					{
						num = count;
						break;
					}
				}
			}
			
			char text = num != -1 ? '#' : 'X';
			//std::string text = num == -1 ? "^" : std::to_string(num);
			outputRoute << text;
		}
		outputRoute << std::endl;
	}
	outputRoute.close();
}