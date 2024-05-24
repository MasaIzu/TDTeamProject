#include "randomMap.h"
#include"MyMath.h"

#include"Input.h"

void RandomMap::Initialize()
{

	//�^�C�����Ȃɂ����[�h����ĂȂ��������̂��߂ɔ��^�C�������[�h���Ă���
	std::unique_ptr<Tile> baseTile = std::make_unique<Tile>();
	baseTile->Initialize();
	baseTile->SetSpriteColor({ 1,1,1,1 });
	tileIndex.push_back(std::move(baseTile));
}

void RandomMap::Update()
{
	//R�L�[�Ń}�b�v����
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		CreateMap();
	}
}

void RandomMap::Draw()
{
	for (uint16_t x = 0; x < mapMaxX; x++) {
		for (uint16_t y = 0; y < mapMaxY; y++) {
			if (map[x][y]) {
				map[x][y]->Draw();
			}
		}
	}
}

void RandomMap::LoadNewTile(std::unique_ptr<Tile> newTile)
{
	int a = 0;

	tileIndex.push_back(std::move(newTile));

	a++;
}

void RandomMap::CreateMap()
{
	Vector2 tilePosBase = { 320.0f,180.0f };

	Vector2 tilePos = tilePosBase;
	uint16_t tileNumber = (uint16_t)tileIndex.size();



	for (uint16_t x = 0; x < mapMaxX; x++) {
		for (uint16_t y = 0; y < mapMaxY; y++) {
			//�^�C���̍��W���Z
			tilePos.y += tileDistance + Tile::tileSpriteSize.y;

			uint16_t randNumber = 0;
			//�^�C����1�ȏ�ǂݍ��܂�Ă���Ȃ烉���_���ɂ���
			if (tileNumber > 0) {
				randNumber = MyMath::Random(1, tileNumber-1);
			}

			std::unique_ptr<Tile> newTile = std::make_unique<Tile>();
			Tile* randTile = tileIndex[randNumber].get();
			newTile->Initialize();
			newTile->SetSpriteColor(randTile->GetSpriteColor());
		//	newTile->SetSpriteColor({1,0,0,1});
			newTile->SetSpritePos(tilePos);
			map[x][y] = std::move(newTile);

		}
		//�^�C�����W���Z
		tilePos.x += tileDistance + Tile::tileSpriteSize.x;
		tilePos.y = tilePosBase.y;
	}

}
