#include "commonmagnumheader.h"
#include "AllDrawables.h"
#include "globalbeacon.h"
#ifdef USEORBITINGCAMERA
#include "OrbitingCamera.h"
#else
#include "BaseCamera.h"
#endif
namespace Magnum {
	namespace Examples {

		using namespace Magnum::Math::Literals;
		//main class
		class PrimitivesExample : public Platform::Application {
		public:
			explicit PrimitivesExample(const Arguments& arguments);
			~PrimitivesExample()
			{

				delete scene_scene;
				delete scene_drawables;

				delete _coloredShader;
				delete _plainShader;

#ifdef USEORBITINGCAMERA
				delete myOrbitingCamera;
#else
				delete greekCamera;
#endif
			}
		private:
			void drawEvent() override;
			void mousePressEvent(MouseEvent& event) override;
			void mouseReleaseEvent(MouseEvent& event) override;
			void mouseMoveEvent(MouseMoveEvent& event) override;
			void mouseScrollEvent(MouseScrollEvent& event) override;

			void keyPressEvent(KeyEvent& event) override;

			void activateSceneGraph();
			void addGridLinesToSceneGraph();
			void removeAllObjectsFromSceneGraphCollection();
			void AddItemsToSceneGraph();

			Scene3D *scene_scene;
			Object3D *scene_o;

			Object3D *scene_cameraObject;
			SceneGraph::Camera3D* scene_camera;
			#ifdef USEORBITINGCAMERA
			OrbitingCamera* myOrbitingCamera;
			#else
			BaseCamera* greekCamera;
			#endif
			PlainDrawable* drawablePtrToGrid;
			std::map<std::string, ColoredDrawable*> mirroredSceneGraphWithIDs;
			std::map<std::string, PlainDrawable*> mirroredSceneGraphWireframeWithIDs;
			std::vector<std::unique_ptr< GL::Mesh >> storedMeshes;

			GL::Buffer gridlines_indexBuffer{ NoCreate }, gridlines_vertexBuffer{ NoCreate };
			GL::Mesh gridlines_mesh{ NoCreate };
			SceneGraph::DrawableGroup3D* scene_drawables;
			Shaders::Phong* _coloredShader;
			Shaders::Flat3D* _plainShader;

			//used for mouse move events
			int deltaX = 0; int deltaY = 0;
		};
/// IMPLEMENTATION!
		PrimitivesExample::PrimitivesExample(const Arguments& arguments) :
			Platform::Application{ arguments, Configuration{}.setTitle("Magnum Example") }
		{
			GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
			GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
			Magnum::GL::Renderer::setClearColor(0x5f5fafff_rgbaf);
			GL::Renderer::enable(GL::Renderer::Feature::DebugOutput);
			GL::Renderer::enable(GL::Renderer::Feature::DebugOutputSynchronous);
			GL::DebugOutput::setDefaultCallback();

			/* Disable rather spammy "Buffer detailed info" debug messages on NVidia drivers */
			GL::DebugOutput::setEnabled(
				GL::DebugOutput::Source::Api, GL::DebugOutput::Type::Other, { 131185 }, false);
			activateSceneGraph();
			
		}

		void PrimitivesExample::drawEvent() {
			GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

			scene_camera->draw(*scene_drawables);
			swapBuffers();
		}

		void PrimitivesExample::mousePressEvent(MouseEvent& event) {
			if (event.button() != MouseEvent::Button::Left) return;
			Vector2i eventData = event.position();
			deltaX = abs(eventData.x());
			deltaY = abs(eventData.y());
			event.setAccepted();
		}

		void PrimitivesExample::mouseReleaseEvent(MouseEvent& event) {
			deltaX = 0;
			deltaY = 0;
			event.setAccepted();
			redraw();
		}

		void PrimitivesExample::mouseMoveEvent(MouseMoveEvent& event) {
			if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;

			Vector2i eventData = event.position();
			Vector2i relativePosition = eventData - Vector2i{ deltaX,deltaY };
			int relativePositionX = relativePosition.x();
			int relativePositionY = relativePosition.y();

			
#ifdef USEORBITINGCAMERA
			myOrbitingCamera->rotateAndTranslateInSphere(relativePositionY, relativePositionX);
#else
			greekCamera->rotate(relativePositionX, relativePositionY);
#endif

			deltaX = eventData.x();
			deltaY = eventData.y();
			event.setAccepted();
			redraw();
		}

		void PrimitivesExample::mouseScrollEvent(MouseScrollEvent & event) {
			Vector2 eventdata = event.offset();
			double mouseScroll = eventdata.y();
			float scalingFactor = 0.15f;
#ifdef USEORBITINGCAMERA
			myOrbitingCamera->move(1.0f - (mouseScroll > 0 ? 1 / (1.0f - (float)scalingFactor) : (1.0f - (float)scalingFactor)));
#else
			greekCamera->move(1.0f - (mouseScroll > 0 ? 1 / (1.0f - (float)scalingFactor) : (1.0f - (float)scalingFactor)));
#endif
			event.setAccepted();
			redraw();
		}

		void PrimitivesExample::keyPressEvent(KeyEvent & event)
		{
			Magnum::Platform::Sdl2Application::KeyEvent::Key in_key = event.key();
			switch (in_key)
			{

			case Magnum::Platform::Sdl2Application::KeyEvent::Key::Up: {
#ifdef USEORBITINGCAMERA
				myOrbitingCamera->upDown(0.25f);
#else
				greekCamera->upDown(0.25f);
#endif
				break;
			}
			case Magnum::Platform::Sdl2Application::KeyEvent::Key::Down: {
#ifdef USEORBITINGCAMERA
				myOrbitingCamera->upDown(-0.25f);
#else
				greekCamera->upDown(-0.25f);
#endif
				break;
			}
			case Magnum::Platform::Sdl2Application::KeyEvent::Key::Left: {
#ifdef USEORBITINGCAMERA
				myOrbitingCamera->strafe(-0.5f);
#else
				greekCamera->strafe(-0.5f);
#endif
				break;
			}
			case Magnum::Platform::Sdl2Application::KeyEvent::Key::Right: {
#ifdef USEORBITINGCAMERA
				myOrbitingCamera->strafe(0.5f);
#else
				greekCamera->strafe(0.5f);
#endif
				break;
			}

			default:
				break;
			}
			event.setAccepted();
			redraw();
		}

		void PrimitivesExample::activateSceneGraph() {

			_coloredShader = new Shaders::Phong();
			_coloredShader
				->setAmbientColor(0x111111ff_rgbaf);
			_coloredShader
				->setSpecularColor(0xffffffff_rgbaf);
			_coloredShader
				->setShininess(80.0f);
			// http://doc.magnum.graphics/magnum/classMagnum_1_1Shaders_1_1Phong.html#a921e59ee974ad8b7a88f02e625034a8e

			_plainShader = new Shaders::Flat3D();
			_plainShader->setColor(0xffbbffff_rgbaf);

			scene_scene = new Scene3D();
			scene_drawables = new SceneGraph::DrawableGroup3D();


			/* Every scene needs a camera */
			//greekCamera = new BaseCamera(*scene_scene);
			//scene_camera = greekCamera->camera();
			myOrbitingCamera = new OrbitingCamera(*scene_scene, 5.0f, Magnum::Vector3{ 0.0f,0.0f,0.0f });
			scene_camera = myOrbitingCamera->camera();
			/* Default object, parent of all (for manipulation) */
			scene_o = new Object3D{ scene_scene };

			addGridLinesToSceneGraph();
			AddItemsToSceneGraph();
		}


		void PrimitivesExample::addGridLinesToSceneGraph() {
			// you can construct the instances with NoInit, but once the GL context is ready you have to replace them with actual live instances ... and only then you can do things with them
			Magnum::Trade::MeshData3D instMeshData = Primitives::grid3DWireframe({ 19, 19 });
			Matrix4 scalingMatrix; scalingMatrix = scalingMatrix.scaling(Vector3{ 5.0,5.0,1.0 });
			MeshTools::transformPointsInPlace(scalingMatrix, instMeshData.positions(0));

			gridlines_mesh = MeshTools::compile(instMeshData);

			drawablePtrToGrid = new PlainDrawable{ *scene_o, *_plainShader, gridlines_mesh, 0xffbbffff_rgbaf, *scene_drawables };
			drawablePtrToGrid->rotateX(Rad(Constants::piHalf()));
		}

		void PrimitivesExample::removeAllObjectsFromSceneGraphCollection()
		{
			for (std::map<std::string, ColoredDrawable*>::iterator itr = mirroredSceneGraphWithIDs.begin(); itr != mirroredSceneGraphWithIDs.end(); itr++)
			{
				delete itr->second;
			}
			mirroredSceneGraphWithIDs.clear();
			for (std::map<std::string, PlainDrawable*>::iterator itr2 = mirroredSceneGraphWireframeWithIDs.begin(); itr2 != mirroredSceneGraphWireframeWithIDs.end(); itr2++) {
				delete itr2->second;
			}
			mirroredSceneGraphWireframeWithIDs.clear();
		}

		void PrimitivesExample::AddItemsToSceneGraph() {

			//Trade::MeshData3D cube = Primitives::cubeSolid();
			Trade::MeshData3D cube = Primitives::cylinderSolid(1, 10, 0.5f, Primitives::CylinderFlags{ Primitives::CylinderFlag::CapEnds });
			storedMeshes.push_back(std::unique_ptr<GL::Mesh>( new GL::Mesh( MeshTools::compile(cube) ) ));
			ColoredDrawable* drawablePtr = new ColoredDrawable{ *scene_o, *_coloredShader, *(*(storedMeshes.end() - 1)), 0xaaaaffff_rgbaf, *scene_drawables };
			this->mirroredSceneGraphWithIDs.insert(std::pair<std::string, ColoredDrawable*>("THING_LOADED", drawablePtr));
			
			Trade::MeshData3D cube2 = Primitives::cubeSolid();
			storedMeshes.push_back(std::unique_ptr<GL::Mesh>(new GL::Mesh(MeshTools::compile(cube2))));
			ColoredDrawable* drawablePtr2 = new ColoredDrawable{ *scene_o, *_coloredShader, *(*(storedMeshes.end()-1)), 0xaaaaffff_rgbaf, *scene_drawables };
			drawablePtr2->translate(Vector3{ 0.5, 0.5, 2.0 });
			this->mirroredSceneGraphWithIDs.insert(std::pair<std::string, ColoredDrawable*>("THING_LOADED2", drawablePtr2));
			
		}

	}
}

MAGNUM_APPLICATION_MAIN(Magnum::Examples::PrimitivesExample)