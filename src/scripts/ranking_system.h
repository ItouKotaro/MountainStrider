//=============================================================
//
// ランキング [ranking_system.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RANKING_SYSTEM_H_
#define _RANKING_SYSTEM_H_

// ランキングシステム
class RankingSystem
{
public:
	// データ登録
	void RegisterData(int score);

	// 順位を取得する
	int GetRanking(int score);

	// 順位からスコアを取得する
	int GetScore(int rank);

	static const std::string RANKING_PATH;	// ランキングのパス
	static const int MAX_DATA;						// 最大データ
private:
	
};

#endif // !_RANKING_SYSTEM_H_
