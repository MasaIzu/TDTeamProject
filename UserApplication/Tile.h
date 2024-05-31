//�^�C���N���X
//�����_����������邤����1���̃}�b�v

#pragma once
#include"Sprite.h"
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"

class Tile
{
public:
	/// <summary>
	/// �ÓI�����o
	/// </summary>

	static Vector2 tileSpriteSize;
	static ViewProjection viewProjection;
	static ViewProjection lightViewProjection;

	/// <summary>
	/// �r���[�s��̃Z�b�g
	/// </summary>
	/// <param name="view"></param>
	static void SetViewProjection(const ViewProjection& view) { Tile::viewProjection = view; }

	/// <summary>
	/// ���C�g�̃r���[�s��̃Z�b�g
	/// </summary>
	/// <param name="lightView"></param>
	static void SetLightView(const ViewProjection& lightView) { Tile::lightViewProjection = lightView; }

public:
	///���I�����o


	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���f���ǂ݂�������
	/// </summary>
	/// <param name="modelFileName"></param>
	void Initialize(const std::string& modelFileName);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��(�X�v���C�g)
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// �`��(�I�u�W�F�N�g)
	/// </summary>
	void DrawObject();

	//setter
	void SetSpritePos(const Vector2& pos) { spritePos = pos; }
	void SetSpriteColor(const Vector4& color) { spriteColor = color; }

	//getter
	const Vector2& GetSpritePos() const { return spritePos; }
	const Vector4& GetSpriteColor() const { return spriteColor; }

private:

	

	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Model> model = nullptr;

	WorldTransform world;
	Vector2 spritePos = { 0,0 };
	Vector4 spriteColor = { 1,1,1,1 };


};

