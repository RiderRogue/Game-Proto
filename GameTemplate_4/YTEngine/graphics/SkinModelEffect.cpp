#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

namespace YTEngine {
	void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
	{
		switch (m_renderMode) {
		case enRenderMode_Normal: {
			//�ʏ�`��B
			//�V�F�[�_�[��K�p����B
			deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
			//todo �V�F�[�_�[���\�[�X�r���[����C�ɐݒ肷��B
			ID3D11ShaderResourceView* srvArray[] = {
				m_albedoTex,							//�A���x�h�e�N�X�`���B
				Shadow_map().GetShadowMapSRV()	//�V���h�E�}�b�v�B
			};
			/*�������̓X���b�g�A
			�������͐ݒ肷��V�F�[�_�[ ���\�[�X�̐��A
			��O�����̓f�o�C�X�ɐݒ肳���SRV*/
			deviceContext->PSSetShaderResources(2, 2, srvArray);
		}break;
		case enRenderMode_CreateShadowMap:
			//�V���h�E�}�b�v�����B
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
			break;
		}
	}
}