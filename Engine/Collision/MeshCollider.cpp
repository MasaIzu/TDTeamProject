#include "MeshCollider.h"
#include "Collision.h"
#include "MyMath.h"

using namespace DirectX;
void MeshCollider::ConstructTriangles(Model* model, Matrix4& matWorld)
{
	triangles.clear();

	int start = 0;


	const std::vector<Mesh::VertexPosNormalUv>& vertices = model->GetVertices();
	const std::vector<unsigned short>& indices = model->GetIndices();

	size_t triangleNum = indices.size() / 3;

	triangles.resize(triangles.size() + triangleNum);

	for (size_t i = 0; i < triangleNum; i++)
	{

		Triangle& tri = triangles[start + i];
		int idx0 = indices[i * 3 + 0];
		int idx1 = indices[i * 3 + 1];
		int idx2 = indices[i * 3 + 2];

		tri.p0 = {
			vertices[idx0].pos.x,
			vertices[idx0].pos.y,
			vertices[idx0].pos.z,
			1 };

		tri.p0 = MyMath::Vec4Mat4Mul(tri.p0, matWorld);


		tri.p1 = {
			vertices[idx1].pos.x,
			vertices[idx1].pos.y,
			vertices[idx1].pos.z,
			1 };

		tri.p1 = MyMath::Vec4Mat4Mul(tri.p1, matWorld);

		tri.p2 = {
			vertices[idx2].pos.x,
			vertices[idx2].pos.y,
			vertices[idx2].pos.z,
			1 };

		tri.p2 = MyMath::Vec4Mat4Mul(tri.p2, matWorld);

		tri.ComputeNormal();
	}

}


//void MeshCollider::ConstructTriangles(fbxModel* model)
//{
//	triangles.clear();
//
//	const std::vector<ModelMesh>& meshes = model->GetMeshs();
//
//	int start = 0;
//
//	std::vector<ModelMesh>::const_iterator it = meshes.cbegin();
//	for (; it != meshes.cend(); ++it) {
//		ModelMesh mesh = *it;
//		const std::vector<PosNormUvTangeCol>& vertices = mesh.GetVertices();
//		const std::vector<uint32_t>& indices = mesh.GetIndices();
//
//		size_t triangleNum = indices.size() / 3;
//
//		triangles.resize(triangles.size() + triangleNum);
//
//		for (size_t i = 0; i < triangleNum; i++) {
//
//			Triangle& tri = triangles[start + i];
//			int idx0 = indices[i * 3 + 0];
//			int idx1 = indices[i * 3 + 1];
//			int idx2 = indices[i * 3 + 2];
//
//			tri.p0 = {
//				vertices[idx0].position.x,
//				vertices[idx0].position.y,
//				vertices[idx0].position.z,
//				1 };
//
//			tri.p1 = {
//				vertices[idx1].position.x,
//				vertices[idx1].position.y,
//				vertices[idx1].position.z,
//				1 };
//
//			tri.p2 = {
//				vertices[idx2].position.x,
//				vertices[idx2].position.y,
//				vertices[idx2].position.z,
//				1 };
//
//			tri.ComputeNormal();
//		}
//	}
//}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const float& radius)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const float& radius,[[maybe_unused]] const float& speed,[[maybe_unused]] const Vector3& look)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const uint32_t& Cooltime,[[maybe_unused]] const bool& isCoolTime)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const uint32_t& Cooltime,[[maybe_unused]] const uint32_t& FirstCoolTime,[[maybe_unused]] const bool& isCoolTime)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const float& radius,[[maybe_unused]] const uint32_t& Cooltime)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const float& radius,[[maybe_unused]] const uint32_t& Cooltime,[[maybe_unused]] const bool& isCoolTime)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

void MeshCollider::Update([[maybe_unused]] const Matrix4& worldPos,[[maybe_unused]] const float& radius,[[maybe_unused]] const uint32_t& Cooltime,[[maybe_unused]] const uint32_t& FirstCoolTime,[[maybe_unused]] const bool& isCoolTime)
{
	
	Matrix4 tmp = MyMath::MakeIdentity();
	invMatWorld = MyMath::MakeInverse(&tmp);
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector4* Inter, Vector4* reject)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = MyMath::Vec4Mat4Mul(sphere.center, invMatWorld);
	localSphere.radius *= Vector4(invMatWorld.m[0][0], invMatWorld.m[0][1], invMatWorld.m[0][2], invMatWorld.m[0][3]).Vector3Length();

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle& triangle = *it;

		if (Collision::CheckSphere2Triangle(localSphere, triangle, Inter, reject)) {
			if (Inter) {
				const Matrix4& matWorld = MyMath::MakeIdentity();

				*Inter = MyMath::Vec4Mat4Mul(*Inter, matWorld);
			}
			if (reject) {
				const Matrix4& matWorld = MyMath::MakeIdentity();
				//ワールド座標系で排斥ベクトルに変換
				*reject = MyMath::Vec4Mat4Mul(*reject, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, Vector4* Inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = MyMath::Vec4Mat4Mul(ray.start, invMatWorld);
	localRay.dir = MyMath::Vec4Mat4Mul(ray.dir, invMatWorld).Vector3Normal();

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle& triangle = *it;

		Vector4 tempInter;

		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter)) {

			const Matrix4& matWorld = MyMath::MakeIdentity();

			tempInter = MyMath::Vec4Mat4Mul(tempInter, matWorld);


			if (distance) {
				Vector4 sub = tempInter - ray.start;
				*distance = sub.Vector3Dot(ray.dir);
			}

			if (Inter) {
				*Inter = tempInter;
			}

			return true;
		}
	}

	return false;
}
