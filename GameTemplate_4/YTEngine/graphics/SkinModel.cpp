#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "graphics/SkinModelEffect.h"

namespace YTEngine {

	class ModelEffect;
	SkinModel::~SkinModel()
	{
		//定数バッファを解放。
		if (m_cb != nullptr) {		
			m_cb->Release();
		}
		//ライト用の定数バッファの解放。
		if (m_lightCb != nullptr) {
			m_lightCb->Release();
		}
		if (m_samplerState != nullptr) {
			//サンプラステートを解放。
			m_samplerState->Release();
		}
	}
	void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)//インスタンシングする場合はここに引数
	{
		//スケルトンのデータを読み込む。
		InitSkeleton(filePath);

		//定数バッファの作成。
		InitConstantBuffer();

		//サンプラステートの初期化。
		InitSamplerState();

		InitDirectionLight();
		//SkinModelDataManagerを使用してCMOファイルのロード。
		m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

		//m_enFbxUpAxis = enFbxUpAxis;
	}
	void SkinModel::InitSkeleton(const wchar_t* filePath)
	{
		//スケルトンのデータを読み込む。
		//cmoファイルの拡張子をtksに変更する。
		std::wstring skeletonFilePath = filePath;
		//文字列から.cmoファイル始まる場所を検索。
		int pos = (int)skeletonFilePath.find(L".cmo");
		//.cmoファイルを.tksに置き換える。
		skeletonFilePath.replace(pos, 4, L".tks");
		//tksファイルをロードする。
		bool result = m_skeleton.Load(skeletonFilePath.c_str());
		if (result == false) {
			//スケルトンが読み込みに失敗した。
			//アニメーションしないモデルは、スケルトンが不要なので
			//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
			char message[256];
			sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
			OutputDebugStringA(message);
#endif
		}
	}
	//定数バッファの初期化。
	void SkinModel::InitConstantBuffer()
	{
		//作成するバッファのサイズをsizeof演算子で求める。
		int bufferSize = sizeof(SVSConstantBuffer);
		//どんなバッファを作成するのかをせてbufferDescに設定する。
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																	//アライメントって→バッファのサイズが16の倍数ということです。
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																	//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
		bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																	//CPUアクセスが不要な場合は0。
																	//定数バッファをVRAM上に作成。
		g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

		//続いて、ライト用の定数バッファを作成。
		//作成するバッファのサイズを変更するだけ。
		bufferDesc.ByteWidth = (((sizeof(SLight) - 1) / 16) + 1) * 16;//Raundup16(sizeof(SLight));				//SDirectionLightは16byteの倍数になっているので、切り上げはやらない。
		g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
	}

	//ディレクションライトの初期化。
	void SkinModel::InitDirectionLight()
	{
		float right = 0.7f;
		m_light.directionLight.direction[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		m_light.directionLight.color[0] = { right, right, right, 1.0f };

		m_light.directionLight.direction[1] = { -1.0f, 0.0f, 0.0f, 0.0f };
		m_light.directionLight.color[1] = { right, right, right, 1.0f };

		m_light.directionLight.direction[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
		m_light.directionLight.color[2] = { right, right, right, 1.0f };

		//地面の色に反映される。
		m_light.directionLight.direction[3] = { 0.0f, 0.0f, -1.0f, 0.0f };
		m_light.directionLight.color[3] = { right, right, right, 1.0f };

		m_light.specPow = 100.0f;
	}

	void SkinModel::SetDirectionLight(float x, float y, float z) {
		m_light.directionLight.color[0] = { x, y, z, 1.0f };
		m_light.directionLight.color[1] = { x, y, z, 1.0f };
		m_light.directionLight.color[2] = { x, y, z, 1.0f };
		m_light.directionLight.color[3] = { x, y, z, 1.0f };
	}

	void SkinModel::SetDirectionDamage(){
		m_light.directionLight.color[0].x *= 6.0f;
		m_light.directionLight.color[1].x *= 6.0f;
		m_light.directionLight.color[2].x *= 6.0f;
		m_light.directionLight.color[3].x *= 6.0f;
	}

	void SkinModel::ReturnDirectionDamage() {
		m_light.directionLight.color[0].x /= 6.0f;
		m_light.directionLight.color[1].x /= 6.0f;
		m_light.directionLight.color[2].x /= 6.0f;
		m_light.directionLight.color[3].x /= 6.0f;
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
		//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  //テクスチャフィルタはバイリニアフィルタ
		//サンプラステートを作成。
		g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);

		//インスタンシング






	}
	void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale, EnFbxUpAxis enUpdateAxis)
	{
		//3dsMaxと軸を合わせるためのバイアス。
		CMatrix mBias = CMatrix::Identity();
		if (enUpdateAxis == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(CMath::PI * -0.5f);
		}
		CMatrix transMatrix, rotMatrix, scaleMatrix;
		//平行移動行列を作成する。
		transMatrix.MakeTranslation(position);
		//回転行列を作成する。
		rotMatrix.MakeRotationFromQuaternion(rotation);
		rotMatrix.Mul(mBias, rotMatrix);
		//拡大行列を作成する。
		scaleMatrix.MakeScaling(scale);
		//ワールド行列を作成する。
		//拡大×回転×平行移動の順番で乗算するように！
		//順番を間違えたら結果が変わるよ。
		m_worldMatrix.Mul(scaleMatrix, rotMatrix);
		m_worldMatrix.Mul(m_worldMatrix, transMatrix);

		//スケルトンの更新。
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
	//	//スケルトンの更新。
	//	m_skeleton.Update(m_worldMatrix);

	//}
	void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
	{
		DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());

		ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

		//定数バッファの内容を更新。
		SVSConstantBuffer vsCb;
		//InitDirectionLight();//ここ
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		//todo ライトカメラのビュー、プロジェクション行列を送る。
		vsCb.mLightProj = Shadow_map().GetLightProjMatrix();
		vsCb.mLightView = Shadow_map().GetLighViewMatrix();
		if (m_isShadowReciever == true) {
			vsCb.isShadowReciever = 1;
		}
		else {
			vsCb.isShadowReciever = 0;
		}
		//法線マップを使用するかどうかのフラグを送る。
		if (m_normalMapSRV != nullptr) {
			vsCb.isHasNormalMap = true;
		}
		else {
			vsCb.isHasNormalMap = false;
		}
		//メインメモリの内容をVRAMに転送する。
		//m_cb=転送先のバッファ。vsCb=転送元のメモリ。
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		
		//視点を設定。
		m_light.eyePos = g_camera3D.GetPosition();
		//ライト用の定数バッファを更新。
		d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_light, 0, 0);

		//定数バッファをGPUに転送。
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		//ピクセルシェーダーで使用する、ライト用の定数バッファを設定。
		d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
		//ピクセルシェーダーで使用する、定数バッファを設定。
		d3dDeviceContext->PSSetConstantBuffers(
			0,                   //定数バッファをバインドするスロット番号。
			1,                   //設定するバッファの数。
			&m_cb                //設定する定数バッファ配列。
		);
		//サンプラステートを設定。
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//ボーン行列をGPUに転送。
		m_skeleton.SendBoneMatrixArrayToGPU();

		if (m_normalMapSRV != nullptr) {
			//法線マップが設定されていたらをレジスタt4に設定する。
			d3dDeviceContext->PSSetShaderResources(4, 1, &m_normalMapSRV);
		}
		//描画。
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

		//定数バッファの内容を更新。
		SVSConstantBuffer vsCb;
		//InitDirectionLight();
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		//todo ライトカメラのビュー、プロジェクション行列を送る。
		vsCb.mLightProj = Shadow_map().GetLightProjMatrix();
		vsCb.mLightView = Shadow_map().GetLighViewMatrix();
		if (m_isShadowReciever == true) {
			vsCb.isShadowReciever = 1;
		}
		else {
			vsCb.isShadowReciever = 0;
		}
		//法線マップを使用するかどうかのフラグを送る。
		if (m_normalMapSRV != nullptr) {
			vsCb.isHasNormalMap = true;
		}
		else {
			vsCb.isHasNormalMap = false;
		}
		//メインメモリの内容をVRAMに転送する。
		//m_cb=転送先のバッファ。vsCb=転送元のメモリ。
		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);

		//視点を設定。
		m_light.eyePos = g_camera3D.GetPosition();
		//ライト用の定数バッファを更新。
		d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_light, 0, 0);

		//定数バッファをGPUに転送。
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
		//ピクセルシェーダーで使用する、ライト用の定数バッファを設定。
		d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
		//ピクセルシェーダーで使用する、定数バッファを設定。
		d3dDeviceContext->PSSetConstantBuffers(
			0,                   //定数バッファをバインドするスロット番号。
			1,                   //設定するバッファの数。
			&m_cb                //設定する定数バッファ配列。
		);
		//サンプラステートを設定。
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//ボーン行列をGPUに転送。
		m_skeleton.SendBoneMatrixArrayToGPU();

		//エフェクトにクエリを行う。
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			auto modelMaterial = reinterpret_cast<YTEngine::ModelEffect*>(material);
			modelMaterial->SetRenderMode(renderMode);
		});
		if (m_normalMapSRV != nullptr) {
			//法線マップが設定されていたらをレジスタt2に設定する。
			d3dDeviceContext->PSSetShaderResources(2, 1, &m_normalMapSRV);
		}
		//描画。
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix
		);
	}
}