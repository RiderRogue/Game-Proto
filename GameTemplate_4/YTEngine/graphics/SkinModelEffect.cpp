#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

namespace YTEngine {
	void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
	{
		switch (m_renderMode) {
		case enRenderMode_Normal: {
			//通常描画。
			//シェーダーを適用する。
			deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
			//todo シェーダーリソースビューを一気に設定する。
			ID3D11ShaderResourceView* srvArray[] = {
				m_albedoTex,							//アルベドテクスチャ。
				Shadow_map().GetShadowMapSRV()	//シャドウマップ。
			};
			/*第一引数はスロット、
			第二引数は設定するシェーダー リソースの数、
			第三引数はデバイスに設定されるSRV*/
			deviceContext->PSSetShaderResources(2, 2, srvArray);
		}break;
		case enRenderMode_CreateShadowMap:
			//シャドウマップ生成。
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
			break;
		}
	}
}