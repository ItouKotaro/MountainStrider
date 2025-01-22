//=============================================================
//
// ランキング [ranking_system.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "ranking_system.h"

const std::string RankingSystem::RANKING_PATH = "data\\ranking_data.bin";
const int RankingSystem::MAX_DATA = 10;
//=============================================================
// [RankingSystem] データを追加
//=============================================================
void RankingSystem::RegisterData(int score)
{
	int data[MAX_DATA];
	for (int i = 0; i < MAX_DATA; i++)
	{
		data[i] = -1;
	}

	// ファイルを開く
	FILE* file;
	file = fopen(RANKING_PATH.c_str(), "rb");

	if (file != nullptr)
	{
		// データ読み込み
		fread(&data, sizeof(data), 1, file);

		// ファイルを閉じる
		fclose(file);
	}

	// 最後尾にスコアを追加する
	if (data[MAX_DATA-1] <= score)
	{ // ランキングに入るとき
		data[MAX_DATA-1] = score;
	}
	
	// 入れ替える
	for (int i = MAX_DATA-1; i > 0; i--)
	{
		if (data[i] > data[i - 1] || data[i - 1] == -1)
		{
			int change = data[i];
			data[i] = data[i - 1];
			data[i - 1] = change;
		}
	}

	// ファイルを開く
	file = fopen(RANKING_PATH.c_str(), "wb");
	
	if (file != nullptr)
	{
		// ファイルに書き込む
		fwrite(data, sizeof(data), 1, file);

		// ファイルを閉じる
		fclose(file);
	}
}

//=============================================================
// [RankingSystem] スコアからランキング位置を取得する
//=============================================================
int RankingSystem::GetRanking(int score)
{
	int data[10];

	// ファイルを開く
	FILE* file;
	file = fopen(RANKING_PATH.c_str(), "rb");

	if (file != nullptr)
	{
		// データ読み込み
		fread(&data, sizeof(data), 1, file);

		// ファイルを閉じる
		fclose(file);
	}
	else
	{
		// データがない
		return 0;
	}

	// スコアから位置を特定する
	for (int i = 0; i < MAX_DATA; i++)
	{
		if (data[i] == score)
		{
			return i + 1;
		}
	}

	return 0;
}

//=============================================================
// [RankingSystem] スコアからランキング位置を取得する
//=============================================================
int RankingSystem::GetScore(int rank)
{
	int data[10];

	// ファイルを開く
	FILE* file;
	file = fopen(RANKING_PATH.c_str(), "rb");

	if (file != nullptr)
	{
		// データ読み込み
		fread(&data, sizeof(data), 1, file);

		// ファイルを閉じる
		fclose(file);
	}
	else
	{
		// データがない
		return -1;
	}

	// 順位からスコアを取得する
	if (0 <= rank && rank < MAX_DATA)
	{
		return data[rank];
	}
	return -1;
}