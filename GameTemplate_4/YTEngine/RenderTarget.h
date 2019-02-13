#pragma once

/// レンダリングターゲット
namespace YTEngine {
	class RenderTarget
	{
	public:

		/// コンストラクタ。
		RenderTarget();

		/// デストラクタ。
		~RenderTarget();


		/// レンダリングターゲットの解放。
		/// この関数はデストラクタから呼ばれています。
		/// 明示的なタイミングでレンダリングターゲットを解放したい場合に使用してください。
		void Release();


		/// レンダリングターゲットの作成。
		/// <param name="w">テクスチャの幅</param>
		/// <param name="h">テクスチャの高さ</param>
		/// <param name="texFormat">テクスチャのフォーマット</param>
		void Create(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

		/// レンダリングターゲットとデプスステンシルのクリア。
		/// <param name="clearColor">クリアカラー</param>
		void ClearRenderTarget(float* clearColor);

		/// レンダリングターゲットビューを取得。
		/// <returns>レンダリングターゲットビュー</returns>
		ID3D11RenderTargetView* GetRenderTargetView()
		{
			return m_renderTargetView;
		}

		/// デプスステンシルビューを取得。
		/// <returns>デプスステンシルビュー</returns>
		ID3D11DepthStencilView* GetDepthStensilView()
		{
			return m_depthStencilView;
		}

		/// レンダリングターゲットのSRVを取得。
		/// <returns>レンダリングターゲットのSRV</returns>
		ID3D11ShaderResourceView* GetRenderTargetSRV()
		{
			return m_renderTargetSRV;
		}

		/// <summary>
		/// ビューポートの取得。
		/// </summary>
		/// <returns>ビューポート</returns>
		D3D11_VIEWPORT* GetViewport()
		{
			return &m_viewport;
		}

		/// <summary>
		/// レンダリングターゲットの幅を取得。
		/// </summary>
		/// <returns></returns>
		float GetWidth() const
		{
			return m_width;
		}
		/// <summary>
		/// レンダリングターゲットの高さを取得。
		/// </summary>
		/// <returns></returns>
		float GetHeight() const
		{
			return m_height;
		}
		/// <summary>
		/// レンダリングターゲットとなっているテクスチャのフォーマットを取得。
		/// </summary>
		/// <returns></returns>
		DXGI_FORMAT GetTextureFormat() const
		{
			return m_format;
		}
	private:
		float m_width = 0;										//レンダリングターゲットの幅。
		float m_height = 0;										//レンダリングターゲットの高さ。
		DXGI_FORMAT m_format = DXGI_FORMAT_UNKNOWN;
		ID3D11Texture2D * m_renderTargetTex = nullptr;		//!<レンダリングターゲットとなるテクスチャ。
		ID3D11RenderTargetView*		m_renderTargetView = nullptr;		//!<レンダーターゲットビュー。
		ID3D11ShaderResourceView*	m_renderTargetSRV = nullptr;		//!<レンダリングターゲットのSRV
		ID3D11Texture2D*			m_depthStencilTex = nullptr;		//!<デプスステンシルとなるテクスチャ。
		ID3D11DepthStencilView*		m_depthStencilView = nullptr;		//!<デプスステンシルビュー。
		D3D11_VIEWPORT				m_viewport;							//!<ビューポート。
	};
}
