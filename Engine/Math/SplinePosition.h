#pragma once
#include "Vector3.h"

#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <memory>
#include <vector>
MY_SUPPRESS_WARNINGS_END

/// <summary>
/// スプライン
/// </summary>
class SplinePosition {

public://基本関数
	/// <summary>
	/// 後で決める方
	/// </summary>
	SplinePosition();

	/// <summary>
	/// Vector3を4つ入れるバージョン(vector型で渡す方もある)
	/// </summary>
	/// <param name="Start">スタート地点</param>
	/// <param name="p1">制御点その1</param>
	/// <param name="p2">制御点その2</param>
	/// <param name="end">ゴール地点</param>
	SplinePosition(const Vector3& Start,const Vector3& p1,const Vector3& p2,const Vector3& end);

	/// <summary>
	/// スタート地点,制御点,ゴール地点を好きに決められる方
	/// </summary>
	/// <param name="points">points{ start, start, p1, p2, p3, end, end };みたいにp3を追加したり</param>
	SplinePosition(const std::vector<Vector3>& points);
	~SplinePosition();

	/// <summary>
	/// スプライン曲線アップデート
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(const float& time);

	/// <summary>
	/// スプライン曲線アップデート4つ打ち込むVer
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(const Vector3& Start,const Vector3& p1,const Vector3& p2,const Vector3& end,const float& time);

	/// <summary>
	/// スプライン曲線アップデート5つ打ち込むVer(5個までなら手打ちでいける)
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(const Vector3& Start,const Vector3& p1,const Vector3& p2,const Vector3& p3,const Vector3& end,const float& time);

	/// <summary>
	/// スプライン曲線アップデートpointsもアプデVer
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(const std::vector<Vector3>& points,const float& time);

	/// <summary>
	/// スタートに戻すVer
	/// </summary>
	void Reset();

	/// <summary>
	/// 細かくリセットを決められるVer
	/// </summary>
	/// <param name="ResetIndex">Indexはスタートに戻したい場合は1にする(0は無理)</param>
	/// <param name="time">MaxTime=1でtimeが0.5なら半分から始まる</param>
	void Reset(const size_t& ResetIndex,const float& time);

	/// <summary>
	/// 近くの曲線にする//今のところ当たったのなら限定
	/// </summary>
	void ResetNearSpline(const Vector3& Pos);

	/// <summary>
	/// ResetNearSplineで使ったフラグのリセット
	/// </summary>
	void ResetNearSplineReset();

private://プライベート関数
	//ポジションアップデート
	Vector3 SplinePositionUpdate(const std::vector<Vector3>& points, size_t& startIndex, float& t);


public://Setter
	//スプラインのマックス変更
	void SetSplineMaxTime(const float& maxTime) { MaxTime = maxTime; }
	//スプライン状態でないものをスプラインにする
	void SetNotSplineVector(const std::vector<Vector3>& points);
public://Getter
	//スプラインが終わったか
	bool GetFinishSpline()const { return isFinishSpline; }
	//指定位置に来たらtrueにする
	bool GetHowReturnIndex(const uint32_t& HowIndex)const;
	//今のIndexをリターンする
	size_t GetNowIndex()const { return startIndex; }
	//ポイントを入れると場所をリターンする
	Vector3 GetSplineVecPoint(const uint32_t& point) { return points[point]; }
	//最後のポイントの位置をゲットする
	Vector3 GetFinalSplineVecPoint() { return points[points.size() - 1]; }
public://公開変数
	Vector3 NowPos;

private://クラス変数
	bool isFinishSpline = false;
	bool isResetNearSpline = false;
	size_t startIndex = 1;
	float timeRate_ = 0.0f;
	float MaxTime = 1.0f;
	std::vector<Vector3> points{};

};