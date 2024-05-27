//�����_���}�b�v�N���X
//�}�b�v�������_���ɐ���

#pragma once
#include"Tile.h"
#include<vector>

class RandomMap
{
public:

	void Initialize();

	void Update();

	void Draw();

	//�V�����^�C���̃��[�h
	void LoadNewTile(std::unique_ptr<Tile> newTile);

private:

	static const uint16_t mapMaxX = 5;
	static const uint16_t mapMaxY = 5;

	float tileDistance = 8.0f;

	std::unique_ptr<Tile> map[mapMaxX][mapMaxY];

	std::vector<std::unique_ptr<Tile>> tileIndex;

	//�}�b�v����
	void CreateMap();

	
};

