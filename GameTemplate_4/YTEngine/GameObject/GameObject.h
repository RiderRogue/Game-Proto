#pragma once
/*!
*@brief	�Q�[���I�u�W�F�N�g�̊��N���X�B
*/
namespace YTEngine {
	class GameObjectManager;
	typedef unsigned char	GameObjectPrio;  /*���s�D��x*/
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�B
	*/
	class IGameObject : Noncopyable
	{
	public:
		IGameObject():
		m_priority(0),
			m_isStart(false),
			m_isDead(false),
			m_isNewFromGameObjectManager(false)
		{
		}
		/*!
		*@brief	�f�X�g���N�^
		*/
		virtual ~IGameObject()
		{
		}
	public:
		/*!
		*@brief	Update�̒��O�ŌĂ΂��J�n�����B
		*@details
		* �{�֐���true��Ԃ��ƃQ�[���I�u�W�F�N�g�̏��������������Ɣ��f�����</br>
		* Update�֐����Ă΂�o���܂��Btrue��Ԃ��Ĉȍ~��Start�֐��͌Ă΂�Ȃ��Ȃ�܂��B</br>
		* �Q�[���I�u�W�F�N�g�̏������ɕ����t���[��������ꍇ�Ȃǂ�false��Ԃ��āA�������X�e�b�v�Ȃǂ��g����</br>
		* �K�؂ɏ��������s���Ă��������B
		*/
		virtual bool Start() { return true; }
		/*!
		*@brief	�X�V
		*/
		virtual void Update() {}
		/*!
		*@brief	�`��
		*/
		virtual void Draw(){}

		virtual void PostDraw() {}
		/*!
		*@brief �V���h�E�L���X�^�[�֓o�^
		*/
		virtual void SetShadowCasters(){}

		/*!
		*@brief	�폜�����Ƃ��ɌĂ΂��B
		*@details	CGameManager::DeleteGameObject���Ă񂾂Ƃ��Ɏ��s����܂��B
		* �f�X�g���N�^���O�Ɏ��s����܂��B
		*/
		virtual void OnDestroy() {}
		/*!
		*@brief	���s�D��x���擾�B
		*/
		GameObjectPrio GetPriority() const
		{
			return m_priority;
		}
	public:
		/*!
		*@brief	�C���X�^���X���������ꂽ����ɌĂ΂��֐��B
		*@details	�R���X�g���N�^���Ă΂ꂽ��ő��Ă΂�܂��B
		*/
		virtual void Awake() {}
		/*!
		*@brief	Update�֐������s�����O�ɌĂ΂��X�V�֐��B
		*/
		virtual void PreUpdate() {}
		/*!
		*@brief	Update�֐������s���ꂽ��ŌĂ΂��X�V�֐��B
		*/
		virtual void PostUpdate() {}
		
		
		/*!
		*@brief	���S�t���O�𗧂Ă�B
		*@details
		* ���̊֐��̓G���W���̊O����͎��s���Ȃ��ł��������B
		*/
		void SetDeadMark()
		{
			m_isDead = true;
		}
		/*!
		*@brief	���S����B
		*/
		bool IsDead() const
		{
			return m_isDead;
		}
		/*!
		*@brief Start�֐������������H
		*/
		virtual bool IsStart() const
		{
			return m_isStart;
		}
		/*!
		*@brief �A�N�e�B�u���ǂ����𔻒�B
		*/
		bool IsActive() const
		{
			return m_isActive;
		}
		/*!
		*@brief �A�N�e�B�u�t���O��ݒ�B
		*/
		void SetActiveFlag(bool flag)
		{
			m_isActive = flag;
		}
		/*!
		*@brief �^�O��ݒ�B
		*/
		void SetTags(unsigned int tags)
		{
			m_tags = tags;
		}
		/*!
		*@brief �^�O���擾�B
		*/
		unsigned int GetTags() const
		{
			return m_tags;
		}
	public:
		
		void PostUpdateWrapper()
		{
			//�A�N�e�B�u����Start�֐����I��莀�S���Ă��炸���S���X�g�ɐς܂�Ă��Ȃ��ꍇ��true�ɂȂ�B
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				PostUpdate();
			}
		}
		void PreUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				PreUpdate();
			}
		}
		void UpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				Update();
			}
		}
		void StartWrapper()
		{
			if (m_isActive && !m_isStart && !m_isDead && !m_isRegistDeadList) {
				if (Start()) {
					//���������������B
					m_isStart = true;
				}
			}
		}
		//�`��̃��b�v�֐��B
		void DrawWrapper()
		{
			//�A�N�e�B�u����Start�֐����I��莀�S���Ă��炸���S���X�g�ɐς܂�Ă��Ȃ��ꍇ��true�ɂȂ�B
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				//�`��B
				Draw();
			}
		}

		void PostDrawWrapper()
		{
			//�A�N�e�B�u����Start�֐����I��莀�S���Ă��炸���S���X�g�ɐς܂�Ă��Ȃ��ꍇ��true�ɂȂ�B
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				//�`��B
				PostDraw();
			}
		}
		//�V���h�E�L���X�^�[�o�^�̃��b�v�֐��B
		void SetShadowCastersWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
				SetShadowCasters();
			}
		}
		void SetMarkNewFromGameObjectManager()
		{
			m_isNewFromGameObjectManager = true;
		}
		bool IsNewFromGameObjectManager() const
		{
			return m_isNewFromGameObjectManager;
		}

		friend class CGameObjectManager;
	protected:
		GameObjectPrio	m_priority;			//!<���s�D��x�B
		bool m_isStart;						//!<Start�̊J�n�t���O�B
		bool m_isDead;						//!<���S�t���O�B
		bool m_isRegistDeadList = false;	//!<���S���X�g�ɐς܂�Ă���B
		bool m_isNewFromGameObjectManager;	//!<GameObjectManager��new���ꂽ�B
		bool m_isRegist = false;			//!<GameObjectManager�ɓo�^����Ă���H
		bool m_isActive = true;				//!<Active�t���O�B
		unsigned int m_tags = 0;			//!<�^�O�B
		unsigned int m_nameKey = 0;			//!<���O�L�[�B
	};
}
