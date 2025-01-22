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
#include "benlib.h"
#include "component/other/audio.h"
class CInputSystem;
#define INPUT_INSTANCE	(CInputSystem::GetInstance())

// Eigen	ライブラリ
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

// Json ライブラリ
#include "libs/json/json.hpp"
using json = nlohmann::json;

// FMOD ライブラリ
#if _DEBUG	// デバッグ用
#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmodstudioL_vc.lib")
#else				// リリース用
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")
#endif

// libnoise ライブラリ
#include "noise/noise.h"
#include "noise/noiseutils.h"
#pragma comment(lib, "noise.lib")
#pragma comment(lib, "noiseutils.lib")

// ライブラリのリンク
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// DirectXコンポーネント
#pragma comment(lib, "winmm.lib")	// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// ジョイパッド処理に必要
#pragma comment(lib, "Shlwapi.lib")	// ファイルの存在確認

#endif // !_PCH_H_ 