#pragma once

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <d3d12.h>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// パーティクルマネージャー
/// </summary>
class ParticleManager {

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize(ID3D12Device* device);

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();


};