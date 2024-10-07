//=============================================================
//
// ワールドローダー [world_loader.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "world_loader.h"
#include <fstream>
#include "component/3d/mesh.h"
#include "component/3d/light.h"
#include "component/3d/collision.h"

//=============================================================
// [WorldLoader] ロード
//=============================================================
void WorldLoader::Load(const std::string& path)
{
	// jsonファイルを読み込む
	std::ifstream ifs(path.c_str());

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jWorld = json::parse(sInputData);

	// Xオブジェクトを読み込む
	for (auto jXObj = jWorld["x-objects"].items().begin(); jXObj != jWorld["x-objects"].items().end(); jXObj++)
	{
		for (unsigned int i = 0; i < jXObj.value().size(); i++)
		{
			// オブジェクトを作成する
			GameObject* pXObject = new GameObject;

			// メッシュを読み込む
			CMesh* pMesh = pXObject->AddComponent<CMesh>();
			if (FAILED(pMesh->LoadMeshX(jXObj.key())))
			{ // 失敗したとき
				pMesh->enabled = false;
				pXObject->Destroy();
				break;
			}

			// 名前とタグ
			pXObject->SetName(jXObj.value()[i]["name"]);
			pXObject->SetTag(jXObj.value()[i]["tag"]);

			// トランスフォーム情報
			pXObject->transform->SetPos({
				jXObj.value()[i]["transform"]["position"][0],  jXObj.value()[i]["transform"]["position"][1],  jXObj.value()[i]["transform"]["position"][2]
				});
			pXObject->transform->SetRot({
				jXObj.value()[i]["transform"]["rotation"][0],  jXObj.value()[i]["transform"]["rotation"][1],  jXObj.value()[i]["transform"]["rotation"][2]
				});
			pXObject->transform->SetScale({
				jXObj.value()[i]["transform"]["scale"][0],  jXObj.value()[i]["transform"]["scale"][1],  jXObj.value()[i]["transform"]["scale"][2]
				});

			// カスタムオブジェクト
			CustomObject(pXObject);

			// オブジェクト管理に追加する
			m_pWorldObjects.push_back(pXObject);
		}
	}

	// プレハブオブジェクトを読み込む
	for (auto jPrefabObj = jWorld["prefab-objects"].items().begin(); jPrefabObj != jWorld["prefab-objects"].items().end(); jPrefabObj++)
	{
		for (unsigned int i = 0; i < jPrefabObj.value().size(); i++)
		{
			// トランスフォーム情報
			Transform prefTrans = Transform();
			prefTrans.SetPos({
				jPrefabObj.value()[i]["transform"]["position"][0],  jPrefabObj.value()[i]["transform"]["position"][1],  jPrefabObj.value()[i]["transform"]["position"][2]
				});
			prefTrans.SetRot({
				jPrefabObj.value()[i]["transform"]["rotation"][0],  jPrefabObj.value()[i]["transform"]["rotation"][1],  jPrefabObj.value()[i]["transform"]["rotation"][2]
				});
			prefTrans.SetScale({
				jPrefabObj.value()[i]["transform"]["scale"][0],  jPrefabObj.value()[i]["transform"]["scale"][1],  jPrefabObj.value()[i]["transform"]["scale"][2]
				});

			// オブジェクトを作成する
			GameObject* pPrefabObject = GameObject::LoadPrefab(jPrefabObj.key(), prefTrans);

			if (pPrefabObject == nullptr)
			{ // 失敗したとき
				break;
			}

			// 名前とタグ
			pPrefabObject->SetName(jPrefabObj.value()[i]["name"]);
			pPrefabObject->SetTag(jPrefabObj.value()[i]["tag"]);

			// データフィールド情報の上書き
			for (int n = 0; n < (int)jPrefabObj.value()[i]["data-field"].size(); n++)
			{
				std::string sNameDF = jPrefabObj.value()[i]["data-field"][n]["name"];
				std::string sTypeDF = jPrefabObj.value()[i]["data-field"][n]["type"];

				// 情報の取得
				CDataField* pDataField = CDataField::GetDataField(pPrefabObject);
				CDF* pDF = pDataField->GetData(sNameDF);

				if (pDF != nullptr)
				{ // データが存在するとき
					CDF::TYPE dfType = pDF->GetType();
					if (dfType == CDF::INT)
					{
						CDF_Int* pIntDF = (CDF_Int*)pDF;
						pIntDF->SetValue(jPrefabObj.value()[i]["data-field"][n]["value"]);
					}
					else if (dfType == CDF::FLOAT)
					{
						CDF_Float* pFloatDF = (CDF_Float*)pDF;
						pFloatDF->SetValue(jPrefabObj.value()[i]["data-field"][n]["value"]);
					}
					else if (dfType == CDF::BOOL)
					{
						CDF_Bool* pBoolDF = (CDF_Bool*)pDF;
						pBoolDF->SetValue(jPrefabObj.value()[i]["data-field"][n]["value"]);
					}
					else if (dfType == CDF::STRING)
					{
						CDF_String* pStringDF = (CDF_String*)pDF;
						pStringDF->SetValue(jPrefabObj.value()[i]["data-field"][n]["value"]);
					}
					else if (dfType == CDF::VECTOR3)
					{
						CDF_Vector3* pVector3DF = (CDF_Vector3*)pDF;
						pVector3DF->SetValue({ jPrefabObj.value()[i]["data-field"][n]["value"][0], jPrefabObj.value()[i]["data-field"][n]["value"][1], jPrefabObj.value()[i]["data-field"][n]["value"][2] });
					}
					else if (dfType == CDF::VECTOR2)
					{
						CDF_Vector2* pVector2DF = (CDF_Vector2*)pDF;
						pVector2DF->SetValue({ jPrefabObj.value()[i]["data-field"][n]["value"][0], jPrefabObj.value()[i]["data-field"][n]["value"][1] });
					}
				}
			}

			// カスタムオブジェクト
			CustomObject(pPrefabObject);

			// オブジェクト管理に追加する
			m_pWorldObjects.push_back(pPrefabObject);
		}
	}

	// ライトオブジェクトを読み込む
	for (auto jLightObj = jWorld["light-objects"].items().begin(); jLightObj != jWorld["light-objects"].items().end(); jLightObj++)
	{
		// オブジェクトを作成する
		GameObject* pLightObject = new GameObject;

		// ライトを設定する
		CLight* pLight = pLightObject->AddComponent<CLight>();

		// 名前とタグ
		pLightObject->SetName(jLightObj.value()["name"]);
		pLightObject->SetTag(jLightObj.value()["tag"]);

		// ライトの設定
		pLight->SetIntensity(jLightObj.value()["intensity"]);		// 強さ
		pLight->SetColor(D3DXCOLOR(										// 色
			jLightObj.value()["color"][0], jLightObj.value()["color"][1], jLightObj.value()["color"][2], jLightObj.value()["color"][3]
		));

		// トランスフォーム情報
		pLightObject->transform->SetPos({
			jLightObj.value()["transform"]["position"][0],  jLightObj.value()["transform"]["position"][1],  jLightObj.value()["transform"]["position"][2]
			});
		pLightObject->transform->SetRot({
			jLightObj.value()["transform"]["rotation"][0],  jLightObj.value()["transform"]["rotation"][1],  jLightObj.value()["transform"]["rotation"][2]
			});
		pLightObject->transform->SetScale({
			jLightObj.value()["transform"]["scale"][0],  jLightObj.value()["transform"]["scale"][1],  jLightObj.value()["transform"]["scale"][2]
			});

		// カスタムオブジェクト
		CustomObject(pLightObject);

		// オブジェクト管理に追加する
		m_pWorldObjects.push_back(pLightObject);
	}

	// ファイルを閉じる
	ifs.close();
}