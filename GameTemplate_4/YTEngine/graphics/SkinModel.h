#pragma once

#include "Skeleton.h"

namespace YTEngine {
	/*!
	*@brief	FBXの上方向。
	*/
	enum EnFbxUpAxis {
		enFbxUpAxisY,		//Y-up
		enFbxUpAxisZ,		//Z-up
	};
	const int NUM_DIRECTION_LIG = 4;
	/*!
	*@brief	ディレクションライト。
	*/
	struct SDirectionLight {
		CVector4 direction[NUM_DIRECTION_LIG];		//ライトの方向。
		CVector4 color[NUM_DIRECTION_LIG];			//ライトのカラー。
	};
	/*
	*@brief	ライト構造体。
	*/
	struct SLight {
		SDirectionLight		directionLight;		//ディレクションライト
		CVector3			eyePos;				//視点の座標。
		float				specPow;			//鏡面反射の絞り。
	};

	/*!
	*@brief	引数で渡された整数値を16の倍数に切り上げます。
	*@param[in]		n		整数の値。
	*@return		nを16の倍数に切り上げた値。
	*/
	/*int Raundup16(int n)
	{
		return (((n - 1) / 16) + 1) * 16;
	}*/
	/*!
	*@brief	スキンモデルクラス。
	*/
	class SkinModel
	{
	public:
		//メッシュが見つかったときのコールバック関数。
		using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
		/*!
		*@brief	デストラクタ。
		*/
		~SkinModel();

		/*!
		*@brief	初期化。
		*@param[in]	filePath		ロードするcmoファイルのファイルパス。
		*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
		*/
		void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
		/*!
		*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
		*@param[in]	position	モデルの座標。
		*@param[in]	rotation	モデルの回転。
		*@param[in]	scale		モデルの拡大率。
		*/
		void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale, EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ);
		/*!
		*@brief	更新。
		*@details
		* 通常描画の場合の行列更新はこちらを使用する。
		* インスタンシング描画の場合はUpdateInstancingDataを使用する。
		*@param[in]		trans		平行移動。
		*@param[in]		rot			回転。
		*@param[in]		scale		拡大。
		*@param[in]		enUpdateAxis	fbxの上方向。
		*/
		void Update(
			const CVector3& trans,
			const CQuaternion& rot,
			const CVector3& scale,
			EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ
		);
		/*!
		*@brief	ボーンを検索。
		*@param[in]		boneName	ボーンの名前。
		*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
		*/
		Bone* FindBone(const wchar_t* boneName)
		{
			int boneId = m_skeleton.FindBoneID(boneName);
			return m_skeleton.GetBone(boneId);
		}
		/*!
		*@brief	モデルを描画。
		*@param[in]	viewMatrix		カメラ行列。
		*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
		*@param[in]	projMatrix		プロジェクション行列。
		*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
		*/
		void Draw(CMatrix viewMatrix, CMatrix projMatrix);

		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="renderStep">
		///	描画モード。GraphicsEngine.hのEnRenderModeを参照。
		/// </param>
		/// <param name="viewMatrix">ビュー行列</param>
		/// <param name="projMatrix">プロジェクション行列行列</param>
		void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix);
		/*!
		*@brief	スケルトンの取得。
		*/
		Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
		/*!
		*@brief	メッシュを検索する。
		*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
		*/
		void FindMesh(OnFindMesh onFindMesh) const
		{
			for (auto& modelMeshs : m_modelDx->meshes) {
				for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
					onFindMesh(mesh);
				}
			}
		}
		/*!
		*@brief	SRVのレジスタ番号。
		*/
		enum EnSkinModelSRVReg {
			enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
			enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
		};
		void SetIsZPrePass(bool zPrePass)
		{
			isZPrePass = zPrePass;
		}

		void SetIsGBuffer(bool gBuffer)
		{
			isGBuffer = gBuffer;
		}

		/// <summary>
		/// シャドウレシーバーのフラグを設定する。
		/// </summary>
		/// <param name="flag">trueを渡すとシャドウレシーバーになる</param>
		/// <remarks>
		/// シャドウレシーバーとは影を落とされるオブジェクトのことです。
		/// シャドウキャスターによって生成された、シャドウマップを利用して
		/// 自身に影を落とします。
		/// オブジェクトがシャドウレシーバーかつシャドウキャスターになっている場合は
		/// セルフシャドウ(自分の影が自分に落ちる)を行うことができます。
		/// </remarks>
		void SetShadowReciever(bool flag)
		{
			m_isShadowReciever = flag;
		}

		/*!
		*@brief ディレクションライトの設定。
		*/
		void SetDirectionLight(float x, float y, float z);

		/*!
		*@brief ディレクションライト0の取得。
		*/
		CVector3 GetDirectionLight0() {
			CVector3 DirectionLight0;
			DirectionLight0.x = m_light.directionLight.color[0].x;
			DirectionLight0.y = m_light.directionLight.color[0].y;
			DirectionLight0.z = m_light.directionLight.color[0].z;
			return DirectionLight0;
		}
		/// <summary>
		/// ディレクションライトを赤くする。
		/// </summary>
		/// <param name="a">1フレームで変化させる量を設定する。</param>
		/// <remarks>
		/// </remarks>
		void DirectionLight_Red01(float a);

		/// <summary>
		/// ディレクションライトを白に戻す。
		/// </summary>
		/// <param name="a">1フレームで変化させる量を設定する。</param>
		/// <remarks>
		/// </remarks>
		void DirectionLight_ReturnRed(float a);

		/// <summary>
		/// 法線マップの設定。
		/// </summary>
		void SetNormalMap(ID3D11ShaderResourceView* srv)
		{
			m_normalMapSRV = srv;
		}
	private:
		/*!
		*@brief	サンプラステートの初期化。
		*/
		void InitSamplerState();
		/*!
		*@brief	定数バッファの作成。
		*/
		void InitConstantBuffer();
		/*!
		*@brief	スケルトンの初期化。
		*@param[in]	filePath		ロードするcmoファイルのファイルパス。
		*/
		void InitSkeleton(const wchar_t* filePath);
		/*!
		*@brief ディレクションライトの初期化。
		*/
		void InitDirectionLight();
		
	private:
		//定数バッファ。
		struct SVSConstantBuffer {
			CMatrix mWorld;			//ワールド行列。
			CMatrix mView;			//ビュー行列。
			CMatrix mProj;			//プロジェクション行列。
			CMatrix mLightView;		//todo ライトビュー行列。
			CMatrix mLightProj;		//todo ライトプロジェクション行列。
			int isShadowReciever;	//todo シャドウレシーバーのフラグ。
			int isHasNormalMap;		//法線マップを保持している？
		};
		//EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
		bool isZPrePass;
		bool isGBuffer;
		ID3D11Buffer*		m_cb = nullptr;					//!<定数バッファ。
		ID3D11Buffer*		m_lightCb = nullptr;			//!<ライト用の定数バッファ。
		SLight				m_light;							//!<ライト構造体。
		Skeleton			m_skeleton;						//!<スケルトン。
		CMatrix				m_worldMatrix;					//!<ワールド行列。
		DirectX::Model*		m_modelDx;						//!<DirectXTKが提供するモデルクラス。
		ID3D11SamplerState* m_samplerState = nullptr;		//!<サンプラステート。
		bool m_isShadowReciever = false;					//シャドウレシーバーのフラグ。
		ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//法線マップのSRV
	};
}