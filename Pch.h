//=============================================================
//
// プリコンパイル済みヘッダー [Pch.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PCH_H_
#define _PCH_H_

// 定義
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ

// インクルード
#include <Windows.h>
#include "d3dx9.h"										// 描画処理に必要
#include "d3d9.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <Shlwapi.h>
#include "dinput.h"
#include "Xinput.h"
#include "input_system.h"
#include "gameobject.h"
class CInputSystem;
#define INPUT_INSTANCE	(CInputSystem::GetInstance())

// Eigen	ライブラリ
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

// OpenAL Soft ライブラリ
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "ex-common.lib")

// Json ライブラリ
#include "libs/json/json.hpp"
using json = nlohmann::json;

// libnoise ライブラリ
#pragma comment(lib, "noise.lib")

// ライブラリのリンク
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// DirectXコンポーネント
#pragma comment(lib, "winmm.lib")	// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// ジョイパッド処理に必要
#pragma comment(lib, "Shlwapi.lib")	// ファイルの存在確認

#endif // !_PCH_H_ 