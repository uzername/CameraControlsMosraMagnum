#include "AllDrawables.h"

void ColoredDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
	_shader
		.setDiffuseColor(_color)
		.setLightPosition(camera.cameraMatrix().transformPoint({ -3.0f, 10.0f, 10.0f }))
		.setTransformationMatrix(transformationMatrix)
		.setNormalMatrix(transformationMatrix.rotation())
		.setProjectionMatrix(camera.projectionMatrix());

	_mesh.draw(_shader);
}

void PlainDrawable::draw(const Matrix4 & transformationMatrix, SceneGraph::Camera3D & camera)
{
	_shader.setColor(_color).setTransformationProjectionMatrix(camera.projectionMatrix()*(transformationMatrix));
	_mesh.draw(_shader);
}