//=============================================================
//
// シェーダー [shader.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SHADER_H_
#define _SHADER_H_

// シェーダー（基底）
class Shader
{
public:
	// コンストラクタ
	Shader() { }
	// デストラクタ
	virtual ~Shader() {}

	// 初期化
	virtual bool Init() = 0;
	// 終了
	virtual void Uninit() = 0;

	// 描画開始
	virtual HRESULT Begin();
	// 描画終了
	virtual HRESULT End();
	// パス開始
	virtual HRESULT BeginPass(const UINT& pass);
	// パス終了
	virtual HRESULT EndPass();

	// パス数の取得
	UINT GetNumPass() { return m_numPass; }

	// コンポーネントから受け取ったデータ
	struct ParamData
	{
		Component* component;
		D3DXCOLOR color;
		LPDIRECT3DTEXTURE9 texture;
		D3DXMATRIX mtx;
	};

	// 描画情報の設定
	virtual void SetParam(const ParamData& data){}

protected:
	ID3DXEffect* m_effect;				// エフェクト
	UINT m_numPass;						// パス数
};

// 標準シェーダー
class DefaultShader : public Shader
{
public:
	DefaultShader() {m_numPass = 1;}

	// 初期化
	bool Init() override{}
	// 終了
	void Uninit() override {}
	
	// 標準は固定パイプラインを使用するため設定しない
	HRESULT Begin() override {}
	HRESULT End() override {}
	HRESULT BeginPass(const UINT& pass) override {}
	HRESULT EndPass() override {}
};


#endif // !_SHADER_H_
