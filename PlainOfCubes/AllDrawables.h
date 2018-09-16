#ifndef ALLDRAWABLES_GUARD
#define ALLDRAWABLES_GUARD

#include "commonmagnumheader.h"

using namespace Magnum;
using namespace Math::Literals;

class ColoredDrawable : public Object3D, public SceneGraph::Drawable3D {
public:
	explicit ColoredDrawable(Object3D/*Quaternionica*/& object, Shaders::Phong& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group) : Object3D{ &object }, SceneGraph::Drawable3D{ *this, &group }, _shader(shader), _mesh(mesh), _color{ color } {}

private:
	void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

	Shaders::Phong& _shader;
	GL::Mesh& _mesh;
	Color4 _color;
};

class PlainDrawable : public Object3D, public SceneGraph::Drawable3D {
public:
	explicit PlainDrawable(Object3D/*Quaternionica*/& object, Shaders::Flat3D& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group) : Object3D{ &object }, SceneGraph::Drawable3D{ *this, &group }, _shader(shader), _mesh(mesh), _color{ color } {
	}
	void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;
private:
	Shaders::Flat3D& _shader;
	GL::Mesh& _mesh;
	Color4 _color;
};

#endif
