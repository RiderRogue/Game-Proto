#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "graphics/SkinModelEffect.h"

namespace YTEngine {

	class ModelEffect;
	SkinModel::~SkinModel()
	{
		//�萔�o�b�t�@������B
		if (m_cb != nullptr) {		
			m_cb->Release();
		}
		//���C�g�p�̒萔�o�b�t�@�̉���B
		if (m_lightCb != nullptr) {
			m_lightCb->Release();
		}
		if (m_samplerState != nullptr) {
			//�T���v���X�e�[�g������B
			m_samplerState->Release();
		}
	}
	void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)//�C���X�^���V���O����ꍇ�͂����Ɉ���
	{
		//�X�P���g���̃f�[�^��ǂݍ��ށB
		InitSkeleton(filePath);

		//�萔�o�b�t�@�̍쐬�B
		InitConstantBuffer();

		//�T���v���X�e�[�g�̏������B
		InitSamplerState();

		InitDirectionLight();
		//SkinModelDataManager���g�p����CMO�t�@�C���̃��[�h�B
		m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

		//m_enFbxUpAxis = enFbxUpAxis;
	}
	void SkinModel::InitSkeleton(const wchar_t* filePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��ށB
		//cmo�t�@�C���̊g���q��tks�ɕύX����B
		std::wstring skeletonFilePath = filePath;
		//�����񂩂�.cmo�t�@�C���n�܂�ꏊ�������B
		int pos = (int)skeletonFilePath.find(L".cmo");
		//.cmo�t�@�C����.tks�ɒu��������B
		skeletonFilePath.replace(pos, 4, L".tks");
		//tks�t�@�C�������[�h����B
		bool result = m_skeleton.Load(skeletonFilePath.c_str());
		if (result == false) {
			//�X�P���g�����ǂݍ��݂Ɏ��s�����B
			//�A�j���[�V�������Ȃ����f���́A�X�P���g�����s�v�Ȃ̂�
			//�ǂݍ��݂Ɏ��s���邱�Ƃ͂���̂ŁA���O�o�͂����ɂ��Ă����B
#ifdef _DEBUG
			char message[256];
			sprintf(message, "tks�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B%ls\n", skeletonFilePath.c_str());
			OutputDebugStringA(message);
#endif
		}
	}
	//�萔�o�b�t�@�̏������B
	void SkinModel::InitConstantBuffer()
	{
		//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
		int bufferSize = sizeof(SVSConstantBuffer);
		//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																	//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																	//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
		bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																	//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
																	//�萔�o�b�t�@��VRAM��ɍ쐬�B
		g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

		//�����āA���C�g�p�̒萔�o�b�t�@���쐬�B
		//�쐬����o�b�t�@�̃T�C�Y��ύX���邾���B
		bufferDesc.ByteWidth = (((sizeof(SLight) - 1) / 16) + 1) * 16;//Raundup16(sizeof(SLight));				//SDirectionLight��16byte�̔{���ɂȂ��Ă���̂ŁA�؂�グ�͂��Ȃ��B
		g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
	}

	//�f�B���N�V�������C�g�̏������B
	void SkinModel::InitDirectionLight()
	{
		m_light.directionLight.direction[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		m_light.directionLight.color[0] = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_light.directionLight.direction[1] = { -1.0f, 0.0f, 0.0f, 0.0f };
		m_light.directionLight.color[1] = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_light.directionLight.direction[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
		m_light.directionLight.color[2] = { 1.0f, 1.0f, 1.0f, 1.0f };

		//�n�ʂ̐F�ɔ��f�����B
		m_light.directionLight.direction[3] = { 0.0f, 0.0f, -1.0f, 0.0f };
		m_light.directionLight.color[3] = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_light.specPow = 100.0f;
	}

	void SkinModel::SetDirectionLight(float x, float y, float z) {
		m_light.directionLight.color[0] = { x, y, z, 1.0f };
		m_light.directionLight.color[1] = { x, y, z, 1.0f };
		m_light.directionLight.color[2] = { x, y, z, 1.0f };
		m_light.directionLight.color[3] = { x, y, z, 1.0f };
	}

	void SkinModel::DirectionLight_Red01(float a) {
		for (int i = 0; i < 4; i++) {
			if (m_light.directionLight.color[i].x < a) {
				m_light.directionLight.color[i].x += 0.4f;	
			}
			else {
				m_light.directionLight.color[i].x = a;
			}
			if (m_light.directionLight.color[i].y > 0.6f) {
				m_light.directionLight.color[i].y -= 0.1f;
			}
			else {
				m_light.directionLight.color[i].y = 0.6f;
			}
			if (m_light.directionLight.color[i].z > 0.6f) {
				m_light.directionLight.color[i].z -= 0.1f;
			}
			else {
				m_light.directionLight.color[i].z = 0.6f;
			}
		}

	}

	void SkinModel::DirectionLight_ReturnRed(float a) {
		for (int i = 0; i < 4; i++) {
			if (m_light.directionLight.color[i].x > 1.0f) {
				m_light.directionLight.color[i].x -= a/10;
			}
			else {
				m_light.directionLight.color[i].x = 1.0f;
			}

			
			if (m_light.directionLight.color[i].y < 1.0f) {
				m_light.directionLight.color[i].y += a/30;
			}
			else {
				m_light.directionLight.color[i].y = 1.0f;
			}

		    if (m_light.directionLight.color[i].z < 1.0f) {
				m_light.directionLight.color[i].z += a/30;
			}
			else {
				m_light.directionLight.color[i].z = 1.0f;
			}
		}

	}
	void SkinModel::InitSamplerState()
	{
		//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  //�e�N�X�`���t�B���^�̓o�C���j�A�t�B���^
		//�T���v���X�e�[�g���쐬�B
		g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);

		//�C���X�^���V���O






	}
	void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale, EnFbxUpAxis enUpdateAxis)
	{
		//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
		CMatrix mBias = CMatrix::Identity();
		if (enUpdateAxis == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(CMath::PI * -0.5f);
		}
		CMatrix transMatrix, rotMatrix, scaleMatrix;
		//���s�ړ��s����쐬����B
		transMatrix.MakeTranslation(position);
		//��]�s����쐬����B
		rotMatrix.MakeRotationFromQuaternion(rotation);
		rotMatrix.Mul(mBias, rotMatrix);
		//�g��s����쐬����B
		scaleMatrix.MakeScaling(scale);
		//���[���h�s����쐬����B
		//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
		//���Ԃ��ԈႦ���猋�ʂ��ς���B
		m_worldMatrix.Mul(scaleMatrix, rotMatrix);
		m_worldMatrix.Mul(m_worldMatrix, transMatrix);

		//�X�P���g���̍X�V�B
		m_skeleton.Update(m_worldMatrix);
		
	}
	//void SkinModel::Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, EnFbxUpAxis enUpdateAxis)
	//{
	//	UpdateWorldMatrix(trans, rot, scale, enUpdateAxis);

	//	if (isZPrePass)
	//	{
	//		GraphicsEngine().GetZPrepass().AddSkinModel(this);
	//	}
	//	if (isGBuffer)
	//	{
	//		GraphicsEngine().GetGBufferRender().AddSkinModel(this);
	//	}

	//	if (m_isShadowCaster) {
	//		GraphicsEngine().GetShadowMap().Entry(&m_shadowCaster);
	//	}
	//	m_numInstance = 1;
	//	//�X�P���g���̍X�V�B
	//	m_skeleton.Update(m_worldMatrix);

	//}
	void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
	{
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());

		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

		//�萔�o�b�t�@�̓��e���X�V�B
		SVSConstantBuffer vsCb;
		//InitDirectionLight();//����
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		//todo ���C�g�J�����̃r���[�A�v���W�F�N�V�����s��𑗂�B
		vsCb.mLightProj = Shadow_map().GetLightProjMatrix();
		vsCb.mLightView = Shadow_map().GetLighViewMatrix();
		if (m_isShadowReciever == true) {
			vsCb.isShadowReciever = 1;
		}
		else {
			vsCb.isShadowReciever = 0;
		}
		//�@���}�b�v���g�p���邩�ǂ����̃t���O�𑗂�B
		if (m_normalMapSRV != nullptr) {
			vsCb.isHasNormalMap = true;
		}
		else {
			vsCb.isHasNormalMap = false;
		}
		//���C���������̓��e��VRAM�ɓ]������B
		//m_cb=�]����̃o�b�t�@�BvsCb=�]�����̃������B
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		
		//���_��ݒ�B
		m_light.eyePos = g_camera3D.GetPosition();
		//���C�g�p�̒萔�o�b�t�@���X�V�B
		d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_light, 0, 0);

		//�萔�o�b�t�@��GPU�ɓ]���B
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		//�s�N�Z���V�F�[�_�[�Ŏg�p����A���C�g�p�̒萔�o�b�t�@��ݒ�B
		d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
		//�s�N�Z���V�F�[�_�[�Ŏg�p����A�萔�o�b�t�@��ݒ�B
		d3dDeviceContext->PSSetConstantBuffers(
			0,                   //�萔�o�b�t�@���o�C���h����X���b�g�ԍ��B
			1,                   //�ݒ肷��o�b�t�@�̐��B
			&m_cb                //�ݒ肷��萔�o�b�t�@�z��B
		);
		//�T���v���X�e�[�g��ݒ�B
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//�{�[���s���GPU�ɓ]���B
		m_skeleton.SendBoneMatrixArrayToGPU();

		if (m_normalMapSRV != nullptr) {
			//�@���}�b�v���ݒ肳��Ă���������W�X�^t4�ɐݒ肷��B
			d3dDeviceContext->PSSetShaderResources(4, 1, &m_normalMapSRV);
		}
		//�`��B
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix
		);
	}

	void SkinModel::Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix)
	{
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());

		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

		//�萔�o�b�t�@�̓��e���X�V�B
		SVSConstantBuffer vsCb;
		//InitDirectionLight();
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		//todo ���C�g�J�����̃r���[�A�v���W�F�N�V�����s��𑗂�B
		vsCb.mLightProj = Shadow_map().GetLightProjMatrix();
		vsCb.mLightView = Shadow_map().GetLighViewMatrix();
		if (m_isShadowReciever == true) {
			vsCb.isShadowReciever = 1;
		}
		else {
			vsCb.isShadowReciever = 0;
		}
		//�@���}�b�v���g�p���邩�ǂ����̃t���O�𑗂�B
		if (m_normalMapSRV != nullptr) {
			vsCb.isHasNormalMap = true;
		}
		else {
			vsCb.isHasNormalMap = false;
		}
		//���C���������̓��e��VRAM�ɓ]������B
		//m_cb=�]����̃o�b�t�@�BvsCb=�]�����̃������B
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);

		//���_��ݒ�B
		m_light.eyePos = g_camera3D.GetPosition();
		//���C�g�p�̒萔�o�b�t�@���X�V�B
		d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_light, 0, 0);

		//�萔�o�b�t�@��GPU�ɓ]���B
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		//�s�N�Z���V�F�[�_�[�Ŏg�p����A���C�g�p�̒萔�o�b�t�@��ݒ�B
		d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
		//�s�N�Z���V�F�[�_�[�Ŏg�p����A�萔�o�b�t�@��ݒ�B
		d3dDeviceContext->PSSetConstantBuffers(
			0,                   //�萔�o�b�t�@���o�C���h����X���b�g�ԍ��B
			1,                   //�ݒ肷��o�b�t�@�̐��B
			&m_cb                //�ݒ肷��萔�o�b�t�@�z��B
		);
		//�T���v���X�e�[�g��ݒ�B
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//�{�[���s���GPU�ɓ]���B
		m_skeleton.SendBoneMatrixArrayToGPU();

		//�G�t�F�N�g�ɃN�G�����s���B
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			auto modelMaterial = reinterpret_cast<YTEngine::ModelEffect*>(material);
			modelMaterial->SetRenderMode(renderMode);
		});
		if (m_normalMapSRV != nullptr) {
			//�@���}�b�v���ݒ肳��Ă���������W�X�^t2�ɐݒ肷��B
			d3dDeviceContext->PSSetShaderResources(2, 1, &m_normalMapSRV);
		}
		//�`��B
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix
		);
	}
}