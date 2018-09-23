#ifndef COMMONMAGNUMHEADER_GUARD
#define COMMONMAGNUMHEADER_GUARD
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>

#include <Magnum/Mesh.h>
#include <Magnum/PixelFormat.h>

#include <Magnum/Platform/Sdl2Application.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Grid.h>
#include <Magnum/Primitives/Cylinder.h>
#include <Magnum/Primitives/Line.h>

#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Flat.h>

#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/SceneData.h>

#include <string>
#include <map>
#include <exception>


using namespace Magnum;
using namespace Math::Literals;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;



#endif