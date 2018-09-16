#ifndef BASECAMERA_GUARD
#define BASECAMERA_GUARD
#include "commonmagnumheader.h"

// https://www.opengl.org/discussion_boards/showthread.php/198988-Implementing-an-orbit-camera
// https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
// https://upload.wikimedia.org/wikipedia/commons/thumb/d/dc/3D_Spherical_2.svg/240px-3D_Spherical_2.svg.png
class OrbitingCamera
{
public:
	explicit OrbitingCamera(Scene3D& scene, float in_distanceToOrigin, Magnum::Vector3 in_originPoint);
	//getter
	Object3D* cameraObject();
	//getter
	SceneGraph::Camera3D* camera();

	OrbitingCamera& zoom(Magnum::Float percentage);
	OrbitingCamera& rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity = 0.001f);
	// pitch is rotation around horizontal. yaw is rotation around vertical. Combine these to orbit around mesh
	OrbitingCamera& rotateAndTranslateInSphere(Magnum::Float dPhi, Magnum::Float dTheta, Magnum::Float sensitivity = 0.001f);
	OrbitingCamera& move(Magnum::Float speed);
	// strafe to left and to right. Changes origin point
	OrbitingCamera& strafe(Magnum::Float speed);
	// strafe up and down. Changes origin point
	OrbitingCamera& upDown(Magnum::Float speed);
protected:
	// no need to delete these manually, they'll be deleted on scene destruction
	Object3D * _cameraObject;
	SceneGraph::Camera3D* _camera;

	Magnum::Float _aspectRatio, _nearPlane, _farPlane;
	Magnum::Rad _fov;

	Magnum::Vector3 _up, _front;
	//phi is angle between vertical direction.
	//theta is angle in horizontal plane
	Magnum::Float theta, phi;
	Magnum::Float distanceToOrigin;
	Magnum::Vector3 storedOriginPoint;
};

#endif