#include "SplinePosition.h"

SplinePosition::SplinePosition()
{
}

SplinePosition::SplinePosition(const Vector3& Start,const Vector3& p1,const Vector3& p2,const Vector3& end)
{
	std::vector<Vector3> creatPoints{ Start, Start, p1, p2, end, end };
	this->points = creatPoints;
}

SplinePosition::SplinePosition(const std::vector<Vector3>& AllPoints)
{
	this->points = AllPoints;
}

SplinePosition::~SplinePosition() {}

void SplinePosition::Update(const float& time)
{
	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < points.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
		}
	}
	NowPos = SplinePositionUpdate(points, startIndex, timeRate_);
}

void SplinePosition::Update(const Vector3& Start,const Vector3& p1,const Vector3& p2,const Vector3& end,const float& time)
{
	std::vector<Vector3> creatPoints{ Start, Start, p1, p2, end, end };
	this->points = creatPoints;

	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < creatPoints.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
			isResetNearSpline = false;
		}
	}
	NowPos = SplinePositionUpdate(creatPoints, startIndex, timeRate_);
}

void SplinePosition::Update(const Vector3& Start,const  Vector3& p1,const Vector3& p2,const Vector3& p3,const Vector3& end,const float& time)
{
	std::vector<Vector3> creatPoints{ Start, Start, p1, p2, p3, end, end };
	this->points = creatPoints;

	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < creatPoints.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
		}
	}
	NowPos = SplinePositionUpdate(creatPoints, startIndex, timeRate_);
}

void SplinePosition::Update(const std::vector<Vector3>& Points,const float& time)
{
	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < Points.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
		}
	}
	NowPos = SplinePositionUpdate(Points, startIndex, timeRate_);
}

void SplinePosition::Reset()
{
	startIndex = 1;
	timeRate_ = 0.0f;
	isFinishSpline = false;
	isResetNearSpline = false;
}

void SplinePosition::Reset(const size_t& ResetIndex,const float& time)
{
	startIndex = ResetIndex;
	timeRate_ = time;
	isFinishSpline = false;
}

void SplinePosition::ResetNearSpline(const Vector3& Pos)
{
	if (isResetNearSpline == false) {
		for (uint32_t i = 1; i < points.size() - 1; i++) {
			Vector3 pos = Pos - points[i];
			Vector3 nextPoint = points[ static_cast<std::vector<Vector3,std::allocator<Vector3>>::size_type>( i ) + 1 ] - points[ i ];
			if (isFinishSpline == true) {
				if (0.0f < pos.length() && pos.length() < nextPoint.length()) {
					startIndex = i;
					timeRate_ = pos.length() / nextPoint.length();
					isFinishSpline = false;
					isResetNearSpline = true;
					break;
				}
			}
			else {
				if (0.0f < pos.length() && pos.length() < nextPoint.length()) {
					startIndex = i;
					timeRate_ = pos.length() / nextPoint.length();
					isResetNearSpline = true;
					break;
				}
			}
		}
	}
}

void SplinePosition::ResetNearSplineReset()
{
	isResetNearSpline = false;
}

Vector3 SplinePosition::SplinePositionUpdate(const std::vector<Vector3>& Points, size_t& StartIndex, float& t)
{
	//補間すべき点の数
	size_t n = Points.size() - 2;

	if (StartIndex > n)return Points[n];
	if (StartIndex < 1)return Points[1];

	Vector3 p0 = Points[StartIndex - 1];
	Vector3 p1 = Points[StartIndex];
	Vector3 p2 = Points[StartIndex + 1];
	Vector3 p3 = Points[StartIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}


void SplinePosition::SetNotSplineVector(const std::vector<Vector3>& Points)
{
	std::vector<Vector3> setPoints;
	uint32_t vecCount = 0;
	for (Vector3 ground : Points) {
		setPoints.push_back(ground);
		if (vecCount == 0) {
			setPoints.push_back(ground);
		}
		else if (setPoints.size() - 1 == Points.size()) {
			setPoints.push_back(ground);
		}
		vecCount++;
	}
	this->points = setPoints;
}

bool SplinePosition::GetHowReturnIndex(const uint32_t& HowIndex) const
{
	if (HowIndex == startIndex) {
		return true;
	}
	return false;
}

