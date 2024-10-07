//=============================================================
//
// ワールドローダー [world_loader.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _WORLD_LOADER_H_
#define _WORLD_LOADER_H_

#include "gameobject.h"

// ワールドを読み込むためのクラスです
// ワールドごとでオブジェクトを管理できます
class WorldLoader
{
public:
	// ワールドファイル(.wdit) の読み込み
	void Load(const std::string& path);

	// オブジェクトに対して実行するプログラム
	virtual void CustomObject(GameObject* gameObject) {}
private:
	std::vector<GameObject*> m_pWorldObjects;		// ワールド内に含まれるオブジェクト
};

#endif // !_WORLD_LOADER_H_