#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager final
{
public: // メンバ関数

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ポストエフェクをかけるオブジェクトの描画
	/// </summary>
	void PostEffectDraw();

	/// <summary>
	/// 背景用影
	/// </summary>
	void BackgroundDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	//CSアップデート
	void CSUpdate();

	/// <summary>
	/// 終了させるかどうか
	/// </summary>
	bool IsBreak();

	/// <summary>
	/// 終了させるかどうか
	/// </summary>
	bool IsSlow();

	// 次のシーン予約
	void SetSceneFactory(AbstractSceneFactory* sceneFactory);
	//シーンを変更
	void ChangeScene(const std::string& sceneName);

	static SceneManager* GetInstance();

private:
	static SceneManager* SceneManager_;

private:
	// 今のシーン（実行中シーン）
	BaseScene* scene_ = nullptr;
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(const SceneManager&) = delete;
};
