//=============================================================
//
// 道 [road.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "road.h"
#include "terrain.h"
#include "renderer.h"
#include "scene/game.h"
#include <fstream>

//=============================================================
// [Road] 初期化
//=============================================================
void Road::Init()
{
	m_texturePath = "data\\TEXTURE\\ROAD\\dirt.png";
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
	// 地形を取得する
	auto terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();

	for (auto itr = m_pointData.begin(); itr != m_pointData.end(); itr++)
	{
		if ((*itr).empty())
			return;

		// 初期地点を設定する
		m_currentIdx = 0;
		m_currentPos.x = static_cast<float>((*itr)[m_currentIdx].x);
		m_currentPos.y = static_cast<float>((*itr)[m_currentIdx].y);

		// ジャンプ台をどこで設置するか
		int numJumpStand = rand() % 5 + 3;
		std::vector<int> putJumpStand;
		int tryCounter = 0;
		for (int i = 0; i < numJumpStand; tryCounter++)
		{
			int num = rand() % (*itr).size();

			bool putted = false;
			for (auto numItr = putJumpStand.begin(); numItr != putJumpStand.end(); numItr++)
			{
				if (*numItr == num)
				{
					putted = true;
					break;
				}
			}

			if (!putted)
			{
				putJumpStand.push_back(num);
				i++;
			}

			if (tryCounter > 30) break;
		}

		// 目標地点を設定する（次のポイントが存在するとき）
		while (m_currentIdx + 1 < (*itr).size())
		{
			PointData nextData = (*itr)[m_currentIdx + 1];
			float halfField = (terrain->GetTerrainSize() * terrain->GetTerrainScale()) / static_cast<float>(2.0f);
			float halfScale = terrain->GetTerrainScale() / static_cast<float>(2.0f);
			D3DXVECTOR2 nextPos = { terrain->GetTerrainScale() * nextData.x - halfField, terrain->GetTerrainScale() * nextData.y - halfField };

			// 目標地点の方向に移動する
			D3DXVECTOR2 dir = { (nextPos.x + halfScale) - m_currentPos.x, (nextPos.y + halfScale) - m_currentPos.y };
			D3DXVec2Normalize(&dir, &dir);		// 目標地点への方向ベクトル
			m_currentPos += dir * 20.0f;
			m_currentPos += D3DXVECTOR2(Benlib::RandomFloat(-5.0f, 5.0f), Benlib::RandomFloat(-5.0f, 5.0f));

			// 設置する
			CreateRoad(m_currentPos, 40.0f, Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f));

			// ジャンプ台
			for (auto numItr = putJumpStand.begin(); numItr != putJumpStand.end(); numItr++)
			{
				if (*numItr == m_currentIdx)
				{
					CreateJumpStand(m_currentPos, Benlib::PosAngle({ m_currentPos.x, 0.0f, m_currentPos.y }, { nextPos.x, 0.0f, nextPos.y }));
					m_currentPos = nextPos;
					break;
				}
			}

			// 目標地点に到着しているかを確認する
			if (nextPos.x <= m_currentPos.x && m_currentPos.x <= nextPos.x + terrain->GetTerrainScale() &&
				nextPos.y <= m_currentPos.y && m_currentPos.y <= nextPos.y + terrain->GetTerrainScale())
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
		vtxPos[i] += pos;
	}

	// 高度格納用
	float vtxHeight[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	// レイで頂点座標を決める
	for (int i = 0; i < 4; i++)
	{
		btVector3 Start = btVector3(vtxPos[i].x, 6000.0f, vtxPos[i].y);
		btVector3 End = Start + btVector3(0.0f, -12000.0f, 0.0f);

		// レイ
		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit())
		{ // ヒットしたとき
			vtxHeight[i] = RayCallback.m_hitPointWorld.getY();

			if (CCollision::GetGameObjectFromCollisionObj(RayCallback.m_collisionObject)->GetName() == "JumpStand")
			{
				return;
			}
		}
		else
		{ // 失敗
			return;
		}
	}

	// 地面を生成する
	SingleComponent<RoadMesh>* groundMesh = new SingleComponent<RoadMesh>();
	groundMesh->Init();
	groundMesh->Create(1, 1, size);
	groundMesh->transform->SetPos({ pos.x, 1.5f + Benlib::RandomFloat(0.0f, 0.5f), pos.y });
	groundMesh->transform->SetRot(0.0f, angle, 0.0f);
	groundMesh->SetHeight(0, 0, vtxHeight[0]);
	groundMesh->SetHeight(1, 0, vtxHeight[1]);
	groundMesh->SetHeight(0, 1, vtxHeight[2]);
	groundMesh->SetHeight(1, 1, vtxHeight[3]);
	groundMesh->SetTexture(m_texturePath);
	m_roadObj.push_back(groundMesh);
}

//=============================================================
// [Road] ジャンプ台の生成
//=============================================================
void Road::CreateJumpStand(const D3DXVECTOR2& pos, const float& angle)
{
	// レイでメッシュの位置を割り出す
	D3DXVECTOR2 vtxPos[9];
	vtxPos[0] = { -60.0f, 0.0f };
	vtxPos[1] = { 0.0f, 0.0f };
	vtxPos[2] = { 60.0f, 0.0f };
	vtxPos[3] = { -60.0f, 100.0f };
	vtxPos[4] = { 0.0f, 100.0f };
	vtxPos[5] = { 60.0f, 100.0f };
	vtxPos[6] = { -60.0f, -100.0f };
	vtxPos[7] = { 0.0f, -100.0f };
	vtxPos[8] = { 60.0f, -100.0f };

	// 回転
	D3DXMATRIX rotMtx;
	D3DXMatrixRotationY(&rotMtx, angle);
	for (int i = 0; i < 9; i++)
	{
		D3DXVECTOR3 transVec = { vtxPos[i].x, 0.0f, vtxPos[i].y };
		D3DXVec3TransformCoord(&transVec, &transVec, &rotMtx);
		vtxPos[i] = { transVec.x, transVec.z };
		vtxPos[i] += pos;
	}

	// 高度格納用
	float vtxHeight = 6000.0f;

	// レイで頂点座標を決める
	for (int i = 0; i < 9; i++)
	{
		btVector3 Start = btVector3(vtxPos[i].x, 6000.0f, vtxPos[i].y);
		btVector3 End = Start + btVector3(0.0f, -12000.0f, 0.0f);

		// レイ
		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit())
		{ // ヒットしたとき
			if (RayCallback.m_hitPointWorld.getY() < vtxHeight)
			{
				vtxHeight = RayCallback.m_hitPointWorld.getY();
			}
		}
		else
		{ // 失敗
			return;
		}
	}

	// 地面を生成する
	Transform jumpTrans;
	jumpTrans.SetPos({pos.x, vtxHeight, pos.y});
	jumpTrans.SetRot(0.0f, angle, 0.0f);
	GameObject* jumpStand = GameObject::LoadPrefab("data\\PREFAB\\jump_stand.pref", jumpTrans);
}

//=============================================================
// [Road] テキストファイルに出力
//=============================================================
void Road::OutputText()
{
	// 地形を取得する
	auto terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();

	// ファイルに経路データを書き出す
	std::ofstream outputRoute("route.txt");
	for (int y = 0; y < terrain->GetTerrainSize(); y++)
	{
		for (int x = 0; x < terrain->GetTerrainSize(); x++)
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

//=============================================================
// [RoadMesh] 描画
//=============================================================
void RoadMesh::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得

	// Zバッファの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CMeshField::Draw();

	// Zバッファの書き込みを有効に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}