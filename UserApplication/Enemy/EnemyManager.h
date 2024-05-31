#pragma once
#include "Vector3.h"
#include "Enemy.h"

#include <memory>
#include <list>
#include <cassert>
#include "ViewProjection.h"

class Player;
///<summary>
///�G���G�ƃ{�X�̐����̂܂Ƃ�
///</summary>
class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize(ViewProjection* viewProjection,Player* player);

	///<summary>
	////�X�V
	///</summary>
	void Update();
	///<summary>
	////�`��
	///</summary>
	void Draw(const ViewProjection& LightViewProjection_);

	///<summary>
	////�v���C���[�̃Z�b�g
	///</summary>
	void SetPlayer(Player* player) { player_ = player; };


	/// <summary>
	/// �G��CSV�̓ǂݍ���
	/// </summary>
	void LoadEnemyPopData();

	//CSV���Z�b�g
	void EnemyPopComandReset();

	/// <summary>
	/// �G��CSV�̉��
	/// </summary>
	void UpdateEnemyPopCommands();

	//�G�̓o�^
	void ExistenceEnemy(const Vector3& EnemyPos);

	//�t�H���_�ȉ��̃t�@�C���ꗗ���擾����֐�
	//folderPath  �t�H���_�p�X
	//file_names  �t�@�C�����ꗗ
	//return        true:����,false : ���s
	bool getFileNames(std::string folderPath, std::vector<std::string>& file_names);
private:

	Player* player_ = nullptr;

	bool waitflag = false;
	int waitTimer = 0;

	std::list<std::unique_ptr<Enemy>> enemy_;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	ViewProjection* view;

	int enemySpornCount = 2;
};