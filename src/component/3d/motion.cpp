//=============================================================
//
// モーション [motion.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "motion.h"
#include "manager.h"

//=============================================================
// [CMotionManager] コンストラクタ
//=============================================================
CMotionManager::CMotionManager(int nPartsNum)
{
	// モーション設定の初期化
	m_nBlendFrame = 8;

	// パーツ数の設定
	Resize(nPartsNum);
}

//=============================================================
// [CMotionManager] デストラクタ
//=============================================================
CMotionManager::~CMotionManager()
{
	// レイヤーを破棄する
	for (int i = 0; i < (int)m_layer.size(); i++)
	{
		if (m_layer[i] != nullptr)
		{
			delete m_layer[i];
			m_layer[i] = nullptr;
		}
	}
	m_layer.clear();

	// 再生中のレイヤーを空にする
	m_playingLayer.clear();

	// コンテンツを破棄する
	for (int i = 0; i < (int)m_contents.size(); i++)
	{
		if (m_contents[i] != nullptr)
		{
			delete m_contents[i];
			m_contents[i] = nullptr;
		}
	}
	m_contents.clear();
}

//=============================================================
// [CMotionManager] 更新
//=============================================================
void CMotionManager::Update()
{
	if (CManager::GetInstance()->GetPause())
		return;

	// 再生中のレイヤーを更新する（キーを進める）
	for (int i = 0; i < (int)m_playingLayer.size(); i++)
	{
		m_playingLayer[i]->Update();
	}

	// それぞれのパーツのモーション情報を割り出す
	bool* bPlayedParts = new bool[m_nPartsNum];		// 再生済みパーツ（レイヤーでのパーツ被りを防ぐ）
	memset(bPlayedParts, false, sizeof(bool)*m_nPartsNum);

	for (int i = 0; i < (int)m_playingLayer.size(); i++)
	{
		// 変数宣言
		CMotionLayer* pLayer = m_playingLayer[(m_playingLayer.size() - 1) - i];		// 対象のレイヤー（最後から順）
		std::vector<int> partsList = pLayer->GetPartsList();										// レイヤーのパーツリストを取得
		CMotionContents* contents = pLayer->GetPlayContents();									// 再生中のコンテンツを取得

		// パーツごとに割り出す
		for (int nCntParts = 0; nCntParts < (int)partsList.size(); nCntParts++)
		{
			// 対象パーツを取得
			int nTargetIdx = partsList[nCntParts];
			if (!(0 <= nTargetIdx && nTargetIdx < m_nPartsNum))
				continue;	// 対象外のパーツをスキップ

			// 対象パーツが既に再生済みではないかを確認する
			if (bPlayedParts[nTargetIdx])
			{ // 再生済みのとき
				continue;										// パーツスキップ
			}
			else
			{ // 未再生のとき
				bPlayedParts[nTargetIdx] = true;		// 再生済みに変更
			}

			// モーションデータ
			m_motionData[nTargetIdx].pContents = contents;
			m_motionData[nTargetIdx].nKeyFrame = pLayer->GetKeyFrame();
		}
	}

	// モーションが設定されていないパーツは初期状態にする
	for (int i = 0; i < m_nPartsNum; i++)
	{
		if (!bPlayedParts[i])
		{ // モーションが再生されていないとき
			m_motionData[i].pContents = nullptr;
			m_motionData[i].nKeyFrame = 0;
		}
	}

	// ブレンド判定
	bool* bBlendParts = new bool[m_nPartsNum];		// ブレンドパーツ
	memset(bBlendParts, false, sizeof(bool) * m_nPartsNum);
	for (int i = 0; i < m_nPartsNum; i++)
	{
		if (m_blendData[i].nBlendCounter <= 0 &&
			m_motionData[i].pContents != m_blendData[i].pContents)
		{ // 前回と異なっているとき（ブレンド条件: 初回のみ）
			m_blendData[i].nBlendCounter = m_nBlendFrame;	// ブレンドカウンターをブレンドフレーム数に設定する
		}

		// ブレンドするとき
		if (m_blendData[i].nBlendCounter > 0)
		{
			// カウンターを減らす
			m_blendData[i].nBlendCounter--;

			// ブレンドを1フレーム更新する
			m_blendData[i].nKeyFrame++;

			// 最大フレームに到達したとき
			if (m_blendData[i].pContents != nullptr &&
				m_blendData[i].nKeyFrame > m_blendData[i].pContents->GetMaxFrame())
			{
				if (m_blendData[i].pContents->GetLoop())
				{ // ループのとき
					m_blendData[i].nKeyFrame = 0;
				}
				else
				{ // ループしないとき
					m_blendData[i].nKeyFrame = m_blendData[i].pContents->GetMaxFrame();
				}
			}

			bBlendParts[i] = true;

			// ブレンドの最後まで到達したとき、ブレンドデータを最新データに更新する
			if (m_blendData[i].nBlendCounter <= 0)
			{
				m_blendData[i].pContents = m_motionData[i].pContents;
				m_blendData[i].nKeyFrame = m_motionData[i].nKeyFrame;
			}
		}
	}



	// モーションデータを元にトランスフォーム情報を格納する
	for (int i = 0; i < m_nPartsNum; i++)
	{
		// 変数
		Transform normalTransform, blendTransform;

		// 通常値を格納する
		if (m_motionData[i].pContents != nullptr)
		{ // モーション中のデータが有るとき
			normalTransform = m_motionData[i].pContents->GetTransform(i, m_motionData[i].nKeyFrame);
		}

		// ブレンド値を格納する
		if (bBlendParts[i] &&
			m_blendData[i].pContents != nullptr)
		{ // ブレンド中のとき
			blendTransform = m_blendData[i].pContents->GetTransform(i, m_blendData[i].nKeyFrame);
		}

		// トランスフォーム値を代入する
		if (bBlendParts[i])
		{ // ブレンド中のとき
			Transform resultTransform;	// ブレンド済みのトランスフォーム

			// ブレンドの計算
			resultTransform.SetPos(
				normalTransform.GetPos() * (float)((m_nBlendFrame - m_blendData[i].nBlendCounter) / (float)m_nBlendFrame) +
				blendTransform.GetPos() * (float)(m_blendData[i].nBlendCounter / (float)m_nBlendFrame)
			);

			resultTransform.SetRot(
				normalTransform.GetRot()* (float)((m_nBlendFrame - m_blendData[i].nBlendCounter) / (float)m_nBlendFrame) +
				blendTransform.GetRot() * (float)(m_blendData[i].nBlendCounter / (float)m_nBlendFrame)
			);

			resultTransform.SetScale(
				normalTransform.GetScale()* (float)((m_nBlendFrame - m_blendData[i].nBlendCounter) / (float)m_nBlendFrame) +
				blendTransform.GetScale() * (float)(m_blendData[i].nBlendCounter / (float)m_nBlendFrame)
			);


			// オフセットを加算する
			resultTransform.SetPos(resultTransform.GetPos() + m_offset[i].GetPos());		// 位置
			resultTransform.SetRot(resultTransform.GetRot() + m_offset[i].GetRot());		// 回転
			resultTransform.SetScale(																			// スケール
				resultTransform.GetScale().x * m_offset[i].GetScale().x,
				resultTransform.GetScale().y * m_offset[i].GetScale().y,
				resultTransform.GetScale().z * m_offset[i].GetScale().z
			);

			// トランスフォームに代入する
			m_transform[i].SetPos(resultTransform.GetPos());
			m_transform[i].SetRot(resultTransform.GetRot());
			m_transform[i].SetScale(resultTransform.GetScale());
		}
		else
		{ // ブレンドしていないとき
			m_transform[i].SetPos(normalTransform.GetPos() + m_offset[i].GetPos());		// 位置
			m_transform[i].SetRot(normalTransform.GetRot() + m_offset[i].GetRot());		// 回転
			m_transform[i].SetScale(																			// スケール
				normalTransform.GetScale().x * m_offset[i].GetScale().x,
				normalTransform.GetScale().y * m_offset[i].GetScale().y,
				normalTransform.GetScale().z * m_offset[i].GetScale().z
			);
		}
	}

	// 設定されているパーツに適用する
	for (int i = 0; i < m_nPartsNum; i++)
	{
		if (m_partsObject[i] != nullptr)
		{
			m_partsObject[i]->transform->SetPos(m_transform[i].GetPos());
			m_partsObject[i]->transform->SetRot(m_transform[i].GetRot());
			m_partsObject[i]->transform->SetScale(m_transform[i].GetScale());
		}
	}


	// ブレンドのために情報を保存しておく
	for (int i = 0; i < m_nPartsNum; i++)
	{
		if (!bBlendParts[i])
		{ // ブレンド中のパーツ以外を更新する
			m_blendData[i].pContents = m_motionData[i].pContents;
			m_blendData[i].nKeyFrame = m_motionData[i].nKeyFrame;
		}
	}

	// 作成した変数を破棄する
	delete[] bPlayedParts;
	bPlayedParts = nullptr;
	delete[] bBlendParts;
	bBlendParts = nullptr;
}

//=============================================================
// [CMotionManager] 消失イベント
//=============================================================
void CMotionManager::OnDestroyOtherObject(GameObject* other)
{
	for (int nCntParts = 0; nCntParts < (int)m_partsObject.size(); nCntParts++)
	{ // パーツ回繰り返す
		if (m_partsObject[nCntParts] == other)
		{ // ゲームオブジェクトが一致した場合
			m_partsObject[nCntParts] = nullptr;	// 解除
		}
	}
}

//=============================================================
// [CMotionManager] サイズ変更
//=============================================================
void CMotionManager::Resize(int nPartsNum)
{
	// パーツ数の設定
	m_nPartsNum = nPartsNum;
	m_transform.resize(m_nPartsNum);
	m_partsObject.resize(m_nPartsNum);
	m_offset.resize(m_nPartsNum);
	m_motionData.resize(m_nPartsNum);
	m_blendData.resize(m_nPartsNum);

	for (int i = 0; i < (int)m_contents.size(); i++)
	{
		m_contents[i]->GetKey().resize(nPartsNum);
	}

	for (int i = 0; i < m_nPartsNum; i++)
	{
		m_partsObject[i] = nullptr;
	}

	// ベースレイヤーの追加
	CMotionLayer* pLayer = AddLayer("Base");
	for (int i = 0; i < m_nPartsNum; i++)
		pLayer->AddParts(i);
}

//=============================================================
// [CMotionManager] レイヤーの追加
//=============================================================
CMotionLayer* CMotionManager::AddLayer(std::string LayerName)
{
	// 同じ名前のレイヤーが存在しないかを確認する
	CMotionLayer* pFindLayer = FindLayer(LayerName);
	if (pFindLayer) return pFindLayer;	// 既に存在している

	// モーションレイヤーの作成
	CMotionLayer* layer = new CMotionLayer(&m_nBlendFrame);

	// レイヤーの設定を行う
	layer->SetName(LayerName);

	// レイヤーを登録する
	m_layer.push_back(layer);

	return layer;
}

//=============================================================
// [CMotionManager] レイヤーの削除
//=============================================================
void CMotionManager::RemoveLayer(std::string LayerName)
{
	// レイヤーを名前検索して取得
	CMotionLayer* pLayer = FindLayer(LayerName);
	if (!pLayer) return;	// 失敗

	// 対象がベースレイヤーではないか確認する
	if (LayerName == "Base")
		return;	// ベースレイヤーの場合は終了

	// 一致するレイヤーを登録解除
	for (int i = 0; i < (int)m_layer.size(); i++)
	{
		if (m_layer[i] == pLayer)
		{
			m_layer.erase(m_layer.begin() + i);
			break;
		}
	}

	// レイヤーを破棄する
	delete pLayer;
}

//=============================================================
// [CMotionManager] レイヤーの検索
//=============================================================
CMotionLayer* CMotionManager::FindLayer(std::string LayerName)
{
	// 登録されているレイヤーから名前が一致するものを返す
	for (int i = 0; i < (int)m_layer.size(); i++)
	{
		CMotionLayer* target = m_layer[i];

		if (LayerName == target->GetName())
		{	// 名前が一致したとき
			return target;		// 対象レイヤーを返す
		}
	}

	// 見つからなかったとき
	return nullptr;
}

//=============================================================
// [CMotionManager] コンテンツの追加
//=============================================================
CMotionContents* CMotionManager::AddContents(std::string ContentsName)
{
	// 名前が重複していないか
	for (int i = 0; i < (int)m_contents.size(); i++)
	{
		if (m_contents[i]->GetName() == ContentsName)
		{ // 名前が重複していた場合
			return nullptr;	// スルー
		}
	}

	// モーションコンテンツの作成
	CMotionContents* contents = new CMotionContents(m_nPartsNum);

	// レイヤーの設定を行う
	contents->SetName(ContentsName);

	// レイヤーを登録する
	m_contents.push_back(contents);

	return contents;
}

//=============================================================
// [CMotionManager] コンテンツの削除
//=============================================================
void CMotionManager::RemoveContents(std::string ContentsName)
{
	// コンテンツを名前検索して取得
	CMotionContents* pContents = FindContents(ContentsName);
	if (!pContents) return;	// 失敗

	// 一致するコンテンツを登録解除
	for (int i = 0; i < (int)m_contents.size(); i++)
	{
		if (m_contents[i] == pContents)
		{
			m_contents.erase(m_contents.begin() + i);
			break;
		}
	}

	// コンテンツを破棄する
	delete pContents;
}

//=============================================================
// [CMotionManager] コンテンツの検索
//=============================================================
CMotionContents* CMotionManager::FindContents(std::string ContentsName)
{
	// 登録されているコンテンツから名前が一致するものを返す
	for (int i = 0; i < (int)m_contents.size(); i++)
	{
		CMotionContents* target = m_contents[i];

		if (ContentsName == target->GetName())
		{	// 名前が一致したとき
			return target;		// 対象コンテンツを返す
		}
	}

	// 見つからなかったとき
	return nullptr;
}

//=============================================================
// [CMotionManager] 再生
//=============================================================
void CMotionManager::Play(std::string ContentsName, std::string LayerName)
{
	// 指定されたレイヤーを取得する
	CMotionLayer* pLayer = FindLayer(LayerName);
	if (!pLayer) return; // 失敗

	// 指定されたモーションを取得する
	CMotionContents* pContent = FindContents(ContentsName);
	if (!pContent) return; // 失敗

	// レイヤーの再生モーションに設定する
	pLayer->SetMotion(pContent);

	// プレイ中のときは停止する
	Stop(pLayer);

	// プレイ中のレイヤーに追加する
	m_playingLayer.push_back(pLayer);
}

//=============================================================
// [CMotionManager] 停止
//=============================================================
void CMotionManager::Stop(std::string LayerName)
{
	// 指定されたレイヤーを取得する
	CMotionLayer* pLayer = FindLayer(LayerName);
	if (!pLayer) return; // 失敗

	// プレイ中のレイヤーに入っているときの処理
	for (int i = 0; i < (int)m_playingLayer.size(); i++)
	{
		if (m_playingLayer[i] == pLayer)
		{ // 入っていたとき
			m_playingLayer.erase(m_playingLayer.begin() + i);	// リストから削除する
			return;
		}
	}
}
void CMotionManager::Stop(CMotionLayer* pLayer)
{
	// NULLチェック
	if (!pLayer) return; // 失敗

	// プレイ中のレイヤーに入っているときの処理
	for (int i = 0; i < (int)m_playingLayer.size(); i++)
	{
		if (m_playingLayer[i] == pLayer)
		{ // 入っていたとき
			m_playingLayer.erase(m_playingLayer.begin() + i);	// リストから削除する
			return;
		}
	}
}

//=============================================================
// [CMotionManager] すべて停止
//=============================================================
void CMotionManager::StopAll()
{
	// 再生中のレイヤーを空にする
	m_playingLayer.clear();
}

//=============================================================
// [CMotionManager] パーツの設定
//=============================================================
void CMotionManager::SetParts(int nPartsIdx, GameObject* pParts)
{
	if (!(0 <= nPartsIdx && nPartsIdx < m_nPartsNum))
		return;	// パーツの最大値を超えている

	// パーツの設定を行う
	m_partsObject[nPartsIdx] = pParts;
}

//=============================================================
// [CMotionManager] オフセットの設定
//=============================================================
void CMotionManager::SetOffset(const int& nPartsIdx, Transform offset)
{
	if (!(0 <= nPartsIdx && nPartsIdx < m_nPartsNum))
		return;	// パーツの最大値を超えている

	// オフセットの設定を行う
	m_offset[nPartsIdx].SetPos(offset.GetPos());
	m_offset[nPartsIdx].SetRot(offset.GetRot());
	m_offset[nPartsIdx].SetScale(offset.GetScale());
}

//=============================================================
// [CMotionManager] トランスフォーム値を取得
//=============================================================
Transform CMotionManager::GetTransform(int nPartsIdx)
{
	// パーツのインデックスが最大値を超えているか
	if (!(0 <= nPartsIdx && nPartsIdx < m_nPartsNum))
		return Transform();	// 超えているとき

	// トランスフォーム値を返す
	return m_transform[nPartsIdx];
}



//=============================================================
// [CMotionLayer] コンストラクタ
//=============================================================
CMotionLayer::CMotionLayer(int* pBlendFrame)
{
	// 変数を初期化する
	strcpy(&m_name[0], "");
	m_pPlayContents = nullptr;
	m_nKeyFrame = 0;
	m_nBlendCounter = 0;
	m_pBeforeContents = nullptr;
	m_nBeforeKeyFrame = 0;
	m_pBlendFrame = pBlendFrame;
	m_bEnded = true;
}

//=============================================================
// [CMotionLayer] 更新（キーを進める）
//=============================================================
void CMotionLayer::Update()
{
	if (m_pPlayContents == nullptr)
		return;	// 再生中のモーションがないときは終了

	// キーを進める
	m_nKeyFrame++;
	
	// キーが最大フレームに達したとき
	if (m_pPlayContents->GetMaxFrame() <= m_nKeyFrame)
	{
		if (m_pPlayContents->GetLoop())
		{ // ループするとき
			m_nKeyFrame = 0;												// キーを0に戻す
		}
		else
		{ // ループしないとき
			m_nKeyFrame = m_pPlayContents->GetMaxFrame();	// 最大フレームにキーを変更
			m_bEnded = true;
		}
	}
}

//=============================================================
// [CMotionLayer] パーツの追加
//=============================================================
void CMotionLayer::AddParts(int nPartsIdx)
{
	// パーツが既に登録されていないかを確認する
	for (int i = 0; i < (int)m_partsIdx.size(); i++)
	{
		if (m_partsIdx[i] == nPartsIdx)
		{ // 見つかった場合
			return;
		}
	}

	// パーツを登録する
	m_partsIdx.push_back(nPartsIdx);
}

//=============================================================
// [CMotionLayer] パーツの削除
//=============================================================
void CMotionLayer::RemoveParts(int nPartsIdx)
{
	// 対象のパーツの登録を解除する
	for (int i = 0; i < (int)m_partsIdx.size(); i++)
	{
		if (m_partsIdx[i] == nPartsIdx)
		{ // 削除対象パーツが見つかったとき
			m_partsIdx.erase(m_partsIdx.begin() + i);	// 登録を解除する
			return;
		}
	}
}

//=============================================================
// [CMotionLayer] モーションの設定
//=============================================================
void CMotionLayer::SetMotion(CMotionContents* pContents)
{
	if (m_pPlayContents != pContents || m_bEnded)
	{ // 違うモーションが設定されていたとき

		// 前回のモーションとしてデータを保存しておく
		m_pBeforeContents = m_pPlayContents;
		m_nBeforeKeyFrame = m_nKeyFrame;

		// ブレンドカウンターの設定
		m_nBlendCounter = *m_pBlendFrame * 2;

		// 再生モーションを変更する
		m_pPlayContents = pContents;
		m_bEnded = false;

		// キーフレームを0に設定する
		m_nKeyFrame = 0;
	}
}


//=============================================================
// [CMotionContents] コンストラクタ
//=============================================================
CMotionContents::CMotionContents(int nPartsNum)
{
	// キー格納ベクターを確保する
	m_partsKey.resize(nPartsNum);

	// 変数の初期化
	strcpy(&m_name[0], "");
	m_nMaxFrame = 20;
	m_bLoop = false;
}

//=============================================================
// [CMotionContents] キーの追加
//=============================================================
void CMotionContents::AddKey(int nPartsIdx, int nKey, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// キーを作成する
	Key key;

	// キーの設定
	key.nKeyFrame = nKey;
	key.transform.SetPos(pos);
	key.transform.SetRot(rot);
	key.transform.SetScale(scale);

	// パーツのインデックスが許容値を超えている場合
	if ((int)m_partsKey.size() <= nPartsIdx)
		return; // 失敗

	// キーが既に設定されているかを確認する
	for (int i = 0; i < (int)m_partsKey[nPartsIdx].size(); i++)
	{
		if (m_partsKey[nPartsIdx][i].nKeyFrame == nKey)
		{ // キーフレームが合致している場合
			m_partsKey[nPartsIdx][i] = key;	// 上書きする
			return;
		}
	}

	// キーを登録する
	m_partsKey[nPartsIdx].push_back(key);

	// 昇順に並べ替える
	SortKey(nPartsIdx);
}

//=============================================================
// [CMotionContents] キーの削除
//=============================================================
void CMotionContents::RemoveKey(int nPartsIdx, int nKey)
{
	// 指定のキーが存在するかを確認する
	for (int i = 0; i < (int)m_partsKey[nPartsIdx].size(); i++)
	{
		if (m_partsKey[nPartsIdx][i].nKeyFrame == nKey)
		{ // キーフレームが合致している場合
			m_partsKey[nPartsIdx].erase(m_partsKey[nPartsIdx].begin() + i);	// 削除
			return;
		}
	}
}

//=============================================================
// [CMotionContents] キーの並べ替え
//=============================================================
void CMotionContents::SortKey(int nPartsIdx)
{
	// 昇順に並べ替える
	sort(m_partsKey[nPartsIdx].begin(), m_partsKey[nPartsIdx].end(),
		[](const Key& a, const Key& b) {return a.nKeyFrame < b.nKeyFrame; });
}

//=============================================================
// [CMotionContents] キー位置のトランスフォームを取得する
//=============================================================
Transform CMotionContents::GetTransform(int nPartsIdx, int nKey)
{
	// パーツが最大数を超えていないか確認
	if (!(0 <= nPartsIdx && nPartsIdx < (int)m_partsKey.size()))
	{
		return Transform(); // 超えていたとき
	}
		
	// パーツのキーリストを取得する
	std::vector<Key>* pKeyList = &m_partsKey[nPartsIdx];
	if (pKeyList->size() <= 0)
	{
		return Transform(); // キーが存在しないとき
	}

	// 現在と次のキーを取得
	Key* pKey = nullptr;
	Key* pCurrentKey = nullptr;
	Key* pNextKey = nullptr;
	int nIntervalKeyFrame = 0;

	// トランスフォームを決める
	Transform currentTrans, nextTrans;

	for (int i = 0; i < (int)pKeyList->size(); i++)
	{
		pKey = &pKeyList->at(i);
	
		if (pKey->nKeyFrame <= nKey)
		{ // キーフレームが指定のキーより小さいとき
			pCurrentKey = &pKeyList->at(i); // 現在のキーに設定
			pNextKey = nullptr;					// 次のキーをリセット

			if (i+1 < (int)pKeyList->size())
			{ // 次のキーが存在するとき
				pNextKey = &pKeyList->at(i + 1);
				nIntervalKeyFrame = pNextKey->nKeyFrame - pCurrentKey->nKeyFrame;
			}
			else
			{
				if (m_bLoop)
				{ // ループするとき
					if (pKeyList->at(0).nKeyFrame == 0)
					{ // 次のキーが0番目にあるとき
						pNextKey = &pKeyList->at(0);	// 最初のキーを次のキーに設定する
					}
					nIntervalKeyFrame = m_nMaxFrame - pCurrentKey->nKeyFrame;
				}
				else
				{
					return pKey->transform;	 // 現在のキーのトランスフォーム値を返す
				}
			}
		}
		else
		{	// 次のキーが存在しないとき
			break;
		}
	}



	// 現座のキーは存在しないが次のキーは存在するとき
	if (pCurrentKey == nullptr)
	{
		pNextKey = &pKeyList->at(0);
		nIntervalKeyFrame = pNextKey->nKeyFrame;
	}
	else
	{
		currentTrans = pCurrentKey->transform;
	}

	// 次のキーが存在するとき
	if (pNextKey != nullptr)
	{
		nextTrans = pNextKey->transform;
	}

	// 1フレームの変化量を計算する
	D3DXVECTOR3 perPos;	// 1フレーム毎の移動量
	perPos = nextTrans.GetPos() - currentTrans.GetPos();
	perPos /= (float)nIntervalKeyFrame;

	D3DXVECTOR3 perRot;	// 1フレーム毎の回転量
	perRot = nextTrans.GetRot() - currentTrans.GetRot();
	perRot /= (float)nIntervalKeyFrame;

	D3DXVECTOR3 perScale;	// 1フレーム毎のスケール量
	perScale = nextTrans.GetScale() - currentTrans.GetScale();
	perScale /= (float)nIntervalKeyFrame;


	// 現在のキーから何フレーム経過したか
	int nProgressFrame = nKey;
	if (pCurrentKey != nullptr)
	{
		nProgressFrame -= pCurrentKey->nKeyFrame;
	}

	// キー位置に基づくトランスフォーム値を計算する
	Transform trans = Transform();
	trans.SetPos(currentTrans.GetPos() + perPos * (float)nProgressFrame);
	trans.SetRot(currentTrans.GetRot() + perRot * (float)nProgressFrame);
	trans.SetScale(currentTrans.GetScale() + perScale * (float)nProgressFrame);

	return trans;	// 結果を返す
}
