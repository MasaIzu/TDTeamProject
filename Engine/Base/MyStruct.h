#pragma once
#include "Vector4.h"

/// <summary>
/// ストラクトをまとめる
/// </summary>
namespace MyStruct {
	//メッシュパーティクル用
	struct Meshes {
		Vector4 meshPos[3];    // xyz
	};

	//ブーストパーティクル用
	struct BoostPos
	{
		Vector4 BoostStartPos[ 4 ];
		Vector4 BoostEndPos[ 4 ];
	};

	//敵のブースト用のパーティクル
	struct EnemyBoostPos
	{
		Vector4 BoostStartPos[ 2 ];
		Vector4 BoostEndPos[ 2 ];
	};

}