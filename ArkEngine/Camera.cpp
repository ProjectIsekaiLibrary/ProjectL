#include <DirectXMath.h>
#include "ResourceManager.h"
#include "MathHeaders.h"
#include "Camera.h"

ArkEngine::ArkDX11::Camera::Camera(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& worldUp)
	: _positionVector(pos), _targetVector(target), _lookVector(), _rightVector(), _upVector(), _viewMatrix(), _projectionMatrix(),
	_isMain(false), _frustumPlanes()
{
	LookAt(pos, target, worldUp);

	for (int i = 0; i < 6; i++)
	{
		_frustumPlanes.emplace_back();
	}

	UpdateFrustum();
}

ArkEngine::ArkDX11::Camera::~Camera()
{
	_frustumPlanes.clear();
}

void ArkEngine::ArkDX11::Camera::Update()
{
	UpdateFrustum();
}

bool ArkEngine::ArkDX11::Camera::GetMain()
{
	return _isMain;
}

void ArkEngine::ArkDX11::Camera::SetMain(bool tf)
{
	_isMain = tf;
}

void ArkEngine::ArkDX11::Camera::Delete()
{
	ResourceManager::GetInstance()->DeleteCamera(this);
}

void ArkEngine::ArkDX11::Camera::SetProjectionMatrix(float fovY, float aspect, float nz, float fz, bool isPerspective)
{
	DirectX::XMMATRIX projMatrix;

	if (isPerspective)
	{
		projMatrix = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nz, fz);
	}
	else
	{
		projMatrix = DirectX::XMMatrixOrthographicLH(fovY, aspect, nz, fz);
	}

	DirectX::XMStoreFloat4x4(&_projectionMatrix, projMatrix);
}

void ArkEngine::ArkDX11::Camera::Strafe(float deltaTime)
{
	DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&_rightVector);
	DirectX::XMVECTOR move = DirectX::XMVectorReplicate(deltaTime);
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&_positionVector);

	DirectX::XMStoreFloat3(&_positionVector, DirectX::XMVectorMultiplyAdd(right, move, position));

	UpdateViewMatrix();
}

void ArkEngine::ArkDX11::Camera::Walk(float deltaTime)
{
	DirectX::XMVECTOR look = DirectX::XMLoadFloat3(&_lookVector);
	DirectX::XMVECTOR move = DirectX::XMVectorReplicate(deltaTime);
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&_positionVector);

	DirectX::XMStoreFloat3(&_positionVector, DirectX::XMVectorMultiplyAdd(look, move, position));

	UpdateViewMatrix();
}

void ArkEngine::ArkDX11::Camera::UpDown(float deltaTime)
{
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&_upVector);
	DirectX::XMVECTOR move = DirectX::XMVectorReplicate(deltaTime);
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&_positionVector);

	DirectX::XMStoreFloat3(&_positionVector, DirectX::XMVectorMultiplyAdd(up, move, position));

	UpdateViewMatrix();
}

std::vector<DirectX::XMFLOAT4>& ArkEngine::ArkDX11::Camera::GetFrstum()
{
	return _frustumPlanes;
}

DirectX::XMFLOAT3 ArkEngine::ArkDX11::Camera::GetCameraPosition()
{
	return _positionVector;
}

void ArkEngine::ArkDX11::Camera::SetCameraPosition(DirectX::XMFLOAT3 position)
{
	_positionVector = position;

	DirectX::XMFLOAT3 upVector = { 0.0f, 1.0f, 0.0f };

	UpdateViewMatrix();
	//LookAt(_positionVector, _targetVector, upVector);
}

void ArkEngine::ArkDX11::Camera::SetTargetPosition(DirectX::XMFLOAT3 position)
{
	_targetVector = position;

	DirectX::XMFLOAT3 upVector = { 0.0f, 1.0f, 0.0f };

	LookAt(_positionVector, _targetVector, upVector);
}

void ArkEngine::ArkDX11::Camera::RotateCamera(DirectX::XMFLOAT2 angle)
{
	float Pitch_X = DirectX::XMConvertToRadians(angle.x);
	float Yaw_y = DirectX::XMConvertToRadians(angle.y);

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(Pitch_X, Yaw_y, 0.0f);

	DirectX::XMVECTOR rotatedDirection = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&_lookVector), rotationMatrix);

	DirectX::XMStoreFloat3(&_lookVector, rotatedDirection);

	DirectX::XMVECTOR lookV = DirectX::XMLoadFloat3(&_lookVector);

	DirectX::XMFLOAT3 worldUp = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR worldUpV = DirectX::XMLoadFloat3(&worldUp);

	DirectX::XMVECTOR rightV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUpV, lookV));
	DirectX::XMStoreFloat3(&_rightVector, rightV);

	DirectX::XMVECTOR upV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(lookV, rightV));
	DirectX::XMStoreFloat3(&_upVector, upV);

	UpdateViewMatrix();
}

void ArkEngine::ArkDX11::Camera::SetCameraPos(DirectX::XMFLOAT3 targetPos, DirectX::XMFLOAT3 direction, float scaleFactor)
{
	DirectX::XMVECTOR myPos = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&direction));

	auto newVec = DirectX::XMVectorScale(myPos, scaleFactor);

	DirectX::XMFLOAT3 myPosition;
	DirectX::XMStoreFloat3(&myPosition, newVec);

	_positionVector = myPosition;

	_targetVector = targetPos;

	DirectX::XMFLOAT3 upVector = { 0.0f, 1.0f, 0.0f };

	UpdateViewMatrix();
	LookAt(_positionVector, _targetVector, upVector);
}

void ArkEngine::ArkDX11::Camera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& worldUp)
{
	DirectX::XMVECTOR posVector = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR targetVector = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR worldUpVector = DirectX::XMLoadFloat3(&worldUp);

	DirectX::XMVECTOR lookVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(targetVector, posVector));
	DirectX::XMStoreFloat3(&_lookVector, lookVector);

	DirectX::XMVECTOR rightVector = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUpVector, lookVector));
	DirectX::XMStoreFloat3(&_rightVector, rightVector);

	DirectX::XMVECTOR upVector = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(lookVector, rightVector));
	DirectX::XMStoreFloat3(&_upVector, upVector);

	UpdateViewMatrix();
}

void ArkEngine::ArkDX11::Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR posV = DirectX::XMLoadFloat3(&_positionVector);
	DirectX::XMVECTOR lookV = DirectX::XMLoadFloat3(&_lookVector);
	DirectX::XMVECTOR rightV = DirectX::XMLoadFloat3(&_rightVector);
	DirectX::XMVECTOR upV = DirectX::XMLoadFloat3(&_upVector);

	float look = -(DirectX::XMVectorGetX(DirectX::XMVector3Dot(posV, lookV)));
	float up = -(DirectX::XMVectorGetY(DirectX::XMVector3Dot(posV, upV)));
	float right = -(DirectX::XMVectorGetZ(DirectX::XMVector3Dot(posV, rightV)));

	_viewMatrix._11 = _rightVector.x;    _viewMatrix._12 = _upVector.x;    _viewMatrix._13 = _lookVector.x;    _viewMatrix._14 = 0.0f;
	_viewMatrix._21 = _rightVector.y;    _viewMatrix._22 = _upVector.y;    _viewMatrix._23 = _lookVector.y;    _viewMatrix._24 = 0.0f;
	_viewMatrix._31 = _rightVector.z;    _viewMatrix._32 = _upVector.z;    _viewMatrix._33 = _lookVector.z;    _viewMatrix._34 = 0.0f;
	_viewMatrix._41 = right;			   _viewMatrix._42 = up;			   _viewMatrix._43 = look;				 _viewMatrix._44 = 1.0f;
}

void ArkEngine::ArkDX11::Camera::UpdateFrustum()
{
	DirectX::XMFLOAT4X4 view, proj;
	DirectX::XMStoreFloat4x4(&view, this->GetViewMatrix());
	DirectX::XMStoreFloat4x4(&proj, this->GetProjMatrix());

	float zMinimum = -proj._43 / proj._33;
	float r = 1000.0f / (1000.0f - zMinimum);
	proj._33 = r;
	proj._43 = -r * zMinimum;

	DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&view), DirectX::XMLoadFloat4x4(&proj));

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, tempMatrix);

	// Calculate near plane of frustum.
	_frustumPlanes[0].x = matrix._14 + matrix._13;
	_frustumPlanes[0].y = matrix._24 + matrix._23;
	_frustumPlanes[0].z = matrix._34 + matrix._33;
	_frustumPlanes[0].w = matrix._44 + matrix._43;

	DirectX::XMVECTOR nearPlane = DirectX::XMLoadFloat4(&_frustumPlanes[0]);
	DirectX::XMPlaneNormalize(nearPlane);
	DirectX::XMStoreFloat4(&_frustumPlanes[0], nearPlane);

	// Calculate far plane of frustum.
	_frustumPlanes[1].x = matrix._14 - matrix._13;
	_frustumPlanes[1].y = matrix._24 - matrix._23;
	_frustumPlanes[1].z = matrix._34 - matrix._33;
	_frustumPlanes[1].w = matrix._44 - matrix._43;

	DirectX::XMVECTOR farPlane = DirectX::XMLoadFloat4(&_frustumPlanes[1]);
	DirectX::XMPlaneNormalize(farPlane);
	DirectX::XMStoreFloat4(&_frustumPlanes[1], farPlane);

	// Calculate left plane of frustum.
	_frustumPlanes[2].x = matrix._14 + matrix._11;
	_frustumPlanes[2].y = matrix._24 + matrix._21;
	_frustumPlanes[2].z = matrix._34 + matrix._31;
	_frustumPlanes[2].w = matrix._44 + matrix._41;

	DirectX::XMVECTOR leftPlane = DirectX::XMLoadFloat4(&_frustumPlanes[2]);
	DirectX::XMPlaneNormalize(leftPlane);
	DirectX::XMStoreFloat4(&_frustumPlanes[2], leftPlane);

	// Calculate right plane of frustum.
	_frustumPlanes[3].x = matrix._14 - matrix._11;
	_frustumPlanes[3].y = matrix._24 - matrix._21;
	_frustumPlanes[3].z = matrix._34 - matrix._31;
	_frustumPlanes[3].w = matrix._44 - matrix._41;

	DirectX::XMVECTOR rightPlane = DirectX::XMLoadFloat4(&_frustumPlanes[3]);
	DirectX::XMPlaneNormalize(rightPlane);
	DirectX::XMStoreFloat4(&_frustumPlanes[3], rightPlane);

	// Calculate top plane of frustum.
	_frustumPlanes[4].x = matrix._14 - matrix._12;
	_frustumPlanes[4].y = matrix._24 - matrix._22;
	_frustumPlanes[4].z = matrix._34 - matrix._32;
	_frustumPlanes[4].w = matrix._44 - matrix._42;

	DirectX::XMVECTOR topPlane = DirectX::XMLoadFloat4(&_frustumPlanes[4]);
	DirectX::XMPlaneNormalize(topPlane);
	DirectX::XMStoreFloat4(&_frustumPlanes[4], topPlane);

	// Calculate bottom plane of frustum.
	_frustumPlanes[5].x = matrix._14 + matrix._12;
	_frustumPlanes[5].y = matrix._24 + matrix._22;
	_frustumPlanes[5].z = matrix._34 + matrix._32;
	_frustumPlanes[5].w = matrix._44 + matrix._42;

	DirectX::XMVECTOR bottomPlane = DirectX::XMLoadFloat4(&_frustumPlanes[5]);
	DirectX::XMPlaneNormalize(bottomPlane);
	DirectX::XMStoreFloat4(&_frustumPlanes[5], bottomPlane);
}

const DirectX::XMMATRIX ArkEngine::ArkDX11::Camera::GetViewMatrix()
{
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&_viewMatrix);

	return view;
}

const DirectX::XMMATRIX ArkEngine::ArkDX11::Camera::GetProjMatrix()
{
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&_projectionMatrix);

	return proj;
}

const DirectX::XMFLOAT3 ArkEngine::ArkDX11::Camera::GetCameraPos()
{
	return _positionVector;
}
