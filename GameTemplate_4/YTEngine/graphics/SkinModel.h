#pragma once

#include "Skeleton.h"

namespace YTEngine {
	/*!
	*@brief	FBX�̏�����B
	*/
	enum EnFbxUpAxis {
		enFbxUpAxisY,		//Y-up
		enFbxUpAxisZ,		//Z-up
	};
	const int NUM_DIRECTION_LIG = 4;
	/*!
	*@brief	�f�B���N�V�������C�g�B
	*/
	struct SDirectionLight {
		CVector4 direction[NUM_DIRECTION_LIG];		//���C�g�̕����B
		CVector4 color[NUM_DIRECTION_LIG];			//���C�g�̃J���[�B
	};
	/*
	*@brief	���C�g�\���́B
	*/
	struct SLight {
		SDirectionLight		directionLight;		//�f�B���N�V�������C�g
		CVector3			eyePos;				//���_�̍��W�B
		float				specPow;			//���ʔ��˂̍i��B
	};

	/*!
	*@brief	�����œn���ꂽ�����l��16�̔{���ɐ؂�グ�܂��B
	*@param[in]		n		�����̒l�B
	*@return		n��16�̔{���ɐ؂�グ���l�B
	*/
	/*int Raundup16(int n)
	{
		return (((n - 1) / 16) + 1) * 16;
	}*/
	/*!
	*@brief	�X�L�����f���N���X�B
	*/
	class SkinModel
	{
	public:
		//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
		using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
		/*!
		*@brief	�f�X�g���N�^�B
		*/
		~SkinModel();

		/*!
		*@brief	�������B
		*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
		*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
		*/
		void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
		/*!
		*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
		*@param[in]	position	���f���̍��W�B
		*@param[in]	rotation	���f���̉�]�B
		*@param[in]	scale		���f���̊g�嗦�B
		*/
		void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale, EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ);
		/*!
		*@brief	�X�V�B
		*@details
		* �ʏ�`��̏ꍇ�̍s��X�V�͂�������g�p����B
		* �C���X�^���V���O�`��̏ꍇ��UpdateInstancingData���g�p����B
		*@param[in]		trans		���s�ړ��B
		*@param[in]		rot			��]�B
		*@param[in]		scale		�g��B
		*@param[in]		enUpdateAxis	fbx�̏�����B
		*/
		void Update(
			const CVector3& trans,
			const CQuaternion& rot,
			const CVector3& scale,
			EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ
		);
		/*!
		*@brief	�{�[���������B
		*@param[in]		boneName	�{�[���̖��O�B
		*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
		*/
		Bone* FindBone(const wchar_t* boneName)
		{
			int boneId = m_skeleton.FindBoneID(boneName);
			return m_skeleton.GetBone(boneId);
		}
		/*!
		*@brief	���f����`��B
		*@param[in]	viewMatrix		�J�����s��B
		*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
		*@param[in]	projMatrix		�v���W�F�N�V�����s��B
		*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
		*/
		void Draw(CMatrix viewMatrix, CMatrix projMatrix);

		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="renderStep">
		///	�`�惂�[�h�BGraphicsEngine.h��EnRenderMode���Q�ƁB
		/// </param>
		/// <param name="viewMatrix">�r���[�s��</param>
		/// <param name="projMatrix">�v���W�F�N�V�����s��s��</param>
		void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix);
		/*!
		*@brief	�X�P���g���̎擾�B
		*/
		Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
		/*!
		*@brief	���b�V������������B
		*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
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
		*@brief	SRV�̃��W�X�^�ԍ��B
		*/
		enum EnSkinModelSRVReg {
			enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
			enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
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
		/// �V���h�E���V�[�o�[�̃t���O��ݒ肷��B
		/// </summary>
		/// <param name="flag">true��n���ƃV���h�E���V�[�o�[�ɂȂ�</param>
		/// <remarks>
		/// �V���h�E���V�[�o�[�Ƃ͉e�𗎂Ƃ����I�u�W�F�N�g�̂��Ƃł��B
		/// �V���h�E�L���X�^�[�ɂ���Đ������ꂽ�A�V���h�E�}�b�v�𗘗p����
		/// ���g�ɉe�𗎂Ƃ��܂��B
		/// �I�u�W�F�N�g���V���h�E���V�[�o�[���V���h�E�L���X�^�[�ɂȂ��Ă���ꍇ��
		/// �Z���t�V���h�E(�����̉e�������ɗ�����)���s�����Ƃ��ł��܂��B
		/// </remarks>
		void SetShadowReciever(bool flag)
		{
			m_isShadowReciever = flag;
		}

		/*!
		*@brief �f�B���N�V�������C�g�̐ݒ�B
		*/
		void SetDirectionLight(float x, float y, float z);

		/*!
		*@brief �f�B���N�V�������C�g0�̎擾�B
		*/
		CVector3 GetDirectionLight0() {
			CVector3 DirectionLight0;
			DirectionLight0.x = m_light.directionLight.color[0].x;
			DirectionLight0.y = m_light.directionLight.color[0].y;
			DirectionLight0.z = m_light.directionLight.color[0].z;
			return DirectionLight0;
		}
		/// <summary>
		/// �f�B���N�V�������C�g��Ԃ�����B
		/// </summary>
		/// <param name="a">1�t���[���ŕω�������ʂ�ݒ肷��B</param>
		/// <remarks>
		/// </remarks>
		void DirectionLight_Red01(float a);

		/// <summary>
		/// �f�B���N�V�������C�g�𔒂ɖ߂��B
		/// </summary>
		/// <param name="a">1�t���[���ŕω�������ʂ�ݒ肷��B</param>
		/// <remarks>
		/// </remarks>
		void DirectionLight_ReturnRed(float a);

		/// <summary>
		/// �@���}�b�v�̐ݒ�B
		/// </summary>
		void SetNormalMap(ID3D11ShaderResourceView* srv)
		{
			m_normalMapSRV = srv;
		}
	private:
		/*!
		*@brief	�T���v���X�e�[�g�̏������B
		*/
		void InitSamplerState();
		/*!
		*@brief	�萔�o�b�t�@�̍쐬�B
		*/
		void InitConstantBuffer();
		/*!
		*@brief	�X�P���g���̏������B
		*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
		*/
		void InitSkeleton(const wchar_t* filePath);
		/*!
		*@brief �f�B���N�V�������C�g�̏������B
		*/
		void InitDirectionLight();
		
	private:
		//�萔�o�b�t�@�B
		struct SVSConstantBuffer {
			CMatrix mWorld;			//���[���h�s��B
			CMatrix mView;			//�r���[�s��B
			CMatrix mProj;			//�v���W�F�N�V�����s��B
			CMatrix mLightView;		//todo ���C�g�r���[�s��B
			CMatrix mLightProj;		//todo ���C�g�v���W�F�N�V�����s��B
			int isShadowReciever;	//todo �V���h�E���V�[�o�[�̃t���O�B
			int isHasNormalMap;		//�@���}�b�v��ێ����Ă���H
		};
		//EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBX�̏�����B
		bool isZPrePass;
		bool isGBuffer;
		ID3D11Buffer*		m_cb = nullptr;					//!<�萔�o�b�t�@�B
		ID3D11Buffer*		m_lightCb = nullptr;			//!<���C�g�p�̒萔�o�b�t�@�B
		SLight				m_light;							//!<���C�g�\���́B
		Skeleton			m_skeleton;						//!<�X�P���g���B
		CMatrix				m_worldMatrix;					//!<���[���h�s��B
		DirectX::Model*		m_modelDx;						//!<DirectXTK���񋟂��郂�f���N���X�B
		ID3D11SamplerState* m_samplerState = nullptr;		//!<�T���v���X�e�[�g�B
		bool m_isShadowReciever = false;					//�V���h�E���V�[�o�[�̃t���O�B
		ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//�@���}�b�v��SRV
	};
}