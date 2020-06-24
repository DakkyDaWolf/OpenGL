#include "pch.h"
#include "GraphicsGame.h"
#include "FirstPersonCamera.h"
#include "ColorHelper.h"
#include "Grid.h"
#include "Skybox.h"
#include "ImGuiComponent.h"
#include "SpotLight.h"
#include <glm/gtx/transform.hpp>
#include "GraphicsProgram.h"

using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(GraphicsGame)

	GraphicsGame::GraphicsGame(const string& windowTitle) :
		Game(windowTitle)
	{
		mDepthStencilBufferEnabled = true;
	}

	void GraphicsGame::Initialize()
	{
		auto camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

		auto grid = make_shared<Grid>(*this, camera);
		mComponents.push_back(grid);

		auto skybox = make_shared<Skybox>(*this, camera, "Content/Textures/Maskonaive2_1024/posx.jpg", "Content/Textures/Maskonaive2_1024/negx.jpg", "Content/Textures/Maskonaive2_1024/posy.jpg", "Content/Textures/Maskonaive2_1024/negy.jpg", "Content/Textures/Maskonaive2_1024/posz.jpg", "Content/Textures/Maskonaive2_1024/negz.jpg", 100.0f);
		mComponents.push_back(skybox);

		auto graphicsProgram = make_shared<GraphicsProgram>(*this, camera);
		mComponents.push_back(graphicsProgram);
		
		auto imGui = make_shared<ImGuiComponent>(*this);
		mComponents.push_back(imGui);
		mServices.AddService(ImGuiComponent::TypeIdClass(), imGui.get());


		auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([&, grid, skybox, graphicsProgram, camera]()
			{
				ImGui::Begin("Controls");
				ImGui::SetNextWindowPos(ImVec2(100, 200));
				ImGui::SetNextWindowSize(ImVec2(200, 200));

				{
					stringstream fpsLabel;
					fpsLabel << setprecision(3) << "Frame Rate: " << (mGameTime.ElapsedGameTime().count() ? 1000.f / mGameTime.ElapsedGameTime().count() : 0.f) << "    Total Elapsed Time: " << mGameTime.TotalGameTimeSeconds().count();
					ImGui::Text(fpsLabel.str().c_str());
				}

				ImGui::Text("Camera (WASD + Left-Click-Mouse-Look)");
				ImGui::Text("Rotate Projector (Arrow Keys)");
				AddImGuiTextField("Toggle Grid (G): "s, (grid->Visible() ? "Visible"s : "Not Visible"s));
				AddImGuiTextField("Toggle Skybox (K): "s, (skybox->Visible() ? "Visible"s : "Not Visible"s));
				AddImGuiTextField("Ambient Light Intensity (+PgUp/-PgDown): "s, graphicsProgram->AmbientLightIntensity(), 2);
				AddImGuiTextField("Camera Movement Rate (+U/-I): "s, camera->MovementRate());
				AddImGuiTextField("Camera Mouse Sensitivity (+O/-P): "s, camera->MouseSensitivity());
				AddImGuiTextField("ShadowMapping: "s, (graphicsProgram->ShowingShadowMapping() ? "On" : "Off"));

				ImGui::End();
			});
		imGui->AddRenderBlock(helpTextImGuiRenderBlock);
		
		AddKeyboardHandler([&, grid, skybox](int key, int /*scancode*/, int action, int /*mods*/) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				Exit();
			}

			if (key == GLFW_KEY_G && action == GLFW_PRESS)
			{
				grid->SetVisible(!grid->Visible());
			}

			if (key == GLFW_KEY_K && action == GLFW_PRESS)
			{
				skybox->SetVisible(!skybox->Visible());
			}
		});

		Game::Initialize();

		camera->SetPosition(0, 5, 30);
	}

	void GraphicsGame::Draw(const GameTime& gameTime)
	{
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, &ColorHelper::CornflowerBlue[0]);
		glClearBufferfv(GL_DEPTH, 0, &one);

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}