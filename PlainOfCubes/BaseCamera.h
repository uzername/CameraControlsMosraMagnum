#ifndef BASECAMERA_GUARD
#define BASECAMERA_GUARD
#include "commonmagnumheader.h"

#include <cmath>
/*
Class for camera for mosra/magnum
Author: Konstantinos Chatzilygeroudis (costashatz@gmail.com)
*/
class BaseCamera {
public:
	explicit BaseCamera(Scene3D& scene);

	Object3D* cameraObject();
	SceneGraph::Camera3D* camera();

	BaseCamera& zoom(Magnum::Float percentage);
	BaseCamera& rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity = 0.001f);
	// pitch is rotation around horizontal. yaw is rotation around vertical. Combine these to orbit around mesh
	BaseCamera& rotateAndTranslateInSphere(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity = 0.001f);
	BaseCamera& move(Magnum::Float speed);
	// strafe to left and to right
	BaseCamera& strafe(Magnum::Float speed);
	BaseCamera& upDown(Magnum::Float speed);

protected:
	// no need to delete these manually, they'll be deleted on scene destruction
	Object3D * _cameraObject;
	SceneGraph::Camera3D* _camera;

	Magnum::Float _aspectRatio, _nearPlane, _farPlane;
	Magnum::Rad _fov;

	Magnum::Vector3 _up, _front;
	Magnum::Float _yaw, _pitch;
};

#endif