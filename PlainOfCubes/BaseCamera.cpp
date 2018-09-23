#include "BaseCamera.h"

BaseCamera::BaseCamera(Scene3D& scene)
{
	using namespace Magnum::Math::Literals;

	_fov = 35.0_degf;
	_aspectRatio = 1.0f;
	_nearPlane = 0.01f;
	_farPlane = 100.f;

	_up = Magnum::Vector3{ 0.f, 1.f, 0.f };
	_yaw = -Magnum::Math::Constants<Magnum::Float>::piHalf();
	_pitch = 0.f;
	_front = (Magnum::Vector3{
		std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) })
		.normalized();

	/* Every scene needs a camera */
	(*(_cameraObject = new Object3D{ &scene }));
	// TO-DO: I should remove this transformation
	_cameraObject->translate(Magnum::Vector3::zAxis(5.f));
	Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();
	_cameraObject->transformation().lookAt(cameraPos, cameraPos + _front, _up);
	(*(_camera = new Magnum::SceneGraph::Camera3D{ *_cameraObject }))
		.setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
		.setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane))
		.setViewport(GL::defaultFramebuffer.viewport().size());
}

Object3D* BaseCamera::cameraObject() { return _cameraObject; }

SceneGraph::Camera3D* BaseCamera::camera() { return _camera; }

BaseCamera& BaseCamera::zoom(Magnum::Float percentage)
{
	Magnum::Float perc = std::max(-1.f, std::min(1.f, percentage));

	_fov += perc * _fov;

	_camera->setProjectionMatrix(
		Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane));

	return *this;
}

BaseCamera& BaseCamera::rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity)
{
	_pitch += dPitch * sensitivity;
	_pitch = std::max(-Magnum::Math::Constants<Magnum::Float>::piHalf(),
		std::min(Magnum::Math::Constants<Magnum::Float>::piHalf(), _pitch));
	_yaw += dYaw * sensitivity;

	_front = (Magnum::Vector3{
		std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) })
		.normalized();
	Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();

	_cameraObject->setTransformation(Magnum::Matrix4::lookAt(cameraPos, cameraPos + _front, _up));

	return *this;
}
// https://www.opengl.org/discussion_boards/showthread.php/198988-Implementing-an-orbit-camera
BaseCamera& BaseCamera::rotateAndTranslateInSphere(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity) {
	float distanceToOrigin = 5.0;
	_pitch += dPitch * sensitivity;
	_pitch = std::max(-Magnum::Math::Constants<Magnum::Float>::piHalf(),
		std::min(Magnum::Math::Constants<Magnum::Float>::piHalf(), _pitch));
	_yaw += dYaw * sensitivity;
	//recalculate front vector
	_front = (Magnum::Vector3{
		std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) })
		.normalized();
	//now I need to get a position on sphere
	//_cameraObject->translate();
	Magnum::Vector3 cameraPosOld = _cameraObject->transformation().translation();

	Magnum::Vector3 cameraPosNew = distanceToOrigin * Magnum::Vector3{ std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) };
	Magnum::Vector3 translationVector = cameraPosOld + cameraPosNew;
	_cameraObject->translate(translationVector);

	Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();

	_cameraObject->setTransformation(Magnum::Matrix4::lookAt(cameraPos, /*_front*/Magnum::Vector3{ 0.0f,0.0f,0.0f }, _up));
	return *this;
}

BaseCamera& BaseCamera::move(Magnum::Float speed)
{
	_cameraObject->translate(speed * _front);

	return *this;
}

BaseCamera& BaseCamera::strafe(Magnum::Float speed)
{
	_cameraObject->translate(speed * Magnum::Math::cross(_front, _up).normalized());

	return *this;
}

BaseCamera & BaseCamera::upDown(Magnum::Float speed)
{
	_cameraObject->translate(speed * _up.normalized());
	return *this;
}