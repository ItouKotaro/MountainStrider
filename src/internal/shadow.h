//=============================================================
//
// 影 [shadow.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "general/main.h"

class CCamera;

class CShadow
{
public:
    CShadow();
    ~CShadow();

    void Init();
    void Uninit();
    void Draw(CCamera* pCamera);
    static HRESULT GenerateShadowMesh(IDirect3DDevice9* pDevice, ID3DXMesh* pMesh, ID3DXMesh** ppOutMesh);
	bool CheckShadow(LPDIRECT3D9 pObject, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp);

    // エッジのマッピング
    struct CEdgeMapping
    {
        int m_anOldEdge[2];         // 元の辺の頂点インデックス
        int m_aanNewEdge[2][2]; // 新しいエッジの頂点インデックス
    public:
        CEdgeMapping()
        {
            FillMemory(m_anOldEdge, sizeof(m_anOldEdge), -1);
            FillMemory(m_aanNewEdge, sizeof(m_aanNewEdge), -1);
        }
    };

    // シャドウの描画タイプ
    enum RENDER_TYPE
    {
        RENDERTYPE_SCENE,                   // シーン（通常）
        RENDERTYPE_SHADOWVOLUME    // シャドウボリューム
    };

	// 定義
    static const float FAR_CLIP;
	static const bool USE_SHADOW;								// 影を有効にするか（有効にした場合でも影を生成しないオプションを選択できます）
	static const std::string SHADOW_VOLUME_PATH;		// シャドウボリュームのパス
	static const float ADJACENCY_EPSILON;
	static const float EXTRUDE_EPSILON;
    static const RENDER_TYPE RENDER_MODE;
private:
    void DrawShadow(CCamera* pCamera);
    void RenderMesh(CCamera* pCamera, bool bRenderLight);

	bool IsDeviceAcceptableShadow(LPDIRECT3D9 pObject, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp);	// 能力チェック
	void InitVertexDeclarationShadow(LPDIRECT3DDEVICE9 pDevice);																						// 頂点宣言の初期化
	BOOL IsDepthFormatOk(D3DFORMAT DepthFormat, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, LPDIRECT3D9 pObject);
    static int FindEdgeInMappingTable(int nV1, int nV2, CEdgeMapping* pMapping, int nCount);

	ID3DXEffect* m_pShadowVolume;
    IDirect3DVertexDeclaration9* m_pMeshDecl;
    IDirect3DVertexDeclaration9* m_pShadowDecl;
    D3DXHANDLE                       m_hRenderShadow;           // 影をレンダリングするためのテクニックハンドル
    D3DXHANDLE                       m_hShowShadow;             // シャドウボリュームを表示するテクニックハンドル
    D3DXHANDLE                       m_hRenderScene;              // シーンをレンダリングするためのテクニックハンドル
    IDirect3DTexture9*               m_pDefaultTex;
    LPD3DXMESH                        m_pLightMesh;
};

// 取得処理
//IDirect3DVertexDeclaration9* GetMeshDecl(void);
//D3DXHANDLE* GetRenderScene(void);
//IDirect3DTexture9* GetDefaultTex(void);
//ID3DXEffect* GetShadowEffect(void);
//
//
//
//void InitShadow(void);
//void UninitShadow(void);
//void DrawShadow(void);
//
//// 影メッシュの生成
//HRESULT GenerateShadowMesh(IDirect3DDevice9* pd3dDevice, ID3DXMesh* pMesh, ID3DXMesh** ppOutMesh);

#endif // !_SHADOW_H_
