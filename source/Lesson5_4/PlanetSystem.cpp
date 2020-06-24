#include "pch.h"
#include "PlanetSystem.h"
#include "Model.h"
#include "PointLight.h"
#include "ProxyModel.h"
#include "Camera.h"
#include "Game.h"
#include "MatrixHelper.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(PlanetSystem)

		PlanetSystem::PlanetSystem(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{		
	}

	PlanetSystem::~PlanetSystem()
	{
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void PlanetSystem::Initialize()
	{
		InitPlanets();

		for (auto& planet : mPlanets)
		{
			planet->Initialize();
		}

		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/PointLightDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/EmissiveTest.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Load the model
		Model model("Content/Models/Sphere.obj", true);

		// Create the vertex and index buffers
		auto mesh = model.Meshes().at(0);
		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
		glBindVertexArray(0);

		mAmbientLight = make_unique<Light>(*mGame);
		mAmbientLight->SetColor(ColorHelper::Black);

		mPointLight = make_unique<PointLight>(*mGame);
		mPointLight->SetRadius(500.0f);
		mPointLight->SetPosition(0.0f, 0.0f, 0.0f);
	}

	void PlanetSystem::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdatePointLight(gameTime);
		UpdateSpecularLight(gameTime);
		UpdateTimeScale(gameTime);

		mTotalTime += gameTime.ElapsedGameTimeSeconds().count() * mTimeScale;
		if(mPlanets.size() > 0) mPlanets[0]->Update(mTotalTime, glm::vec3(0,0,0));
	}

	void PlanetSystem::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();	

		mShaderProgram.AmbientColor() << mAmbientLight->Color();
		mShaderProgram.LightColor() << mPointLight->Color();
		mShaderProgram.LightPosition() << mPointLight->Position();
		mShaderProgram.LightRadius() << mPointLight->Radius();
		mShaderProgram.CameraPosition() << mCamera->Position();
		mShaderProgram.SpecularColor() << mSpecularColor;
		mShaderProgram.SpecularPower() << mSpecularPower;

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		for (auto& planet : mPlanets)
		{
			mat4 wvp = mCamera->ViewProjectionMatrix() * planet->Transform() * mWorldMatrix;
			mShaderProgram.WorldViewProjection() << wvp;
			mShaderProgram.World() << planet->Transform() * mWorldMatrix;
			mShaderProgram.EmissiveIntensity() << planet->EmissiveIntensity();
			glBindTexture(GL_TEXTURE_2D, planet->Texture());
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
	}

	void PlanetSystem::InitPlanets()
	{
		auto Sun = make_shared<Planet>("Content/Textures/2k_sun.jpg");
		Sun->SetScale(2 * Planet::EarthScale);
		Sun->SetEmissiveIntensity(1.f);
		mPlanets.push_back(Sun);

		auto Mercury = make_shared<Planet>("Content/Textures/2k_mercury.png", Sun.operator->());
		Mercury->SetRotationAxis(glm::vec3(0.f, 1.f, 0.f));
		Mercury->SetRotationPeriod(58.646f * Planet::EarthDay);
		Mercury->SetOrbitAxis(glm::vec3(0, 0.99255f, 0.12187f));
		Mercury->SetOrbitDistance(0.387f * Planet::AstronomicalUnit);
		Mercury->SetOrbitPeriod(87.969f * Planet::EarthDay);
		Mercury->SetScale(0.3824f * Planet::EarthScale);
		mPlanets.push_back(Mercury);

		auto Venus = make_shared<Planet>("Content/Textures/2k_venus_surface.png", Sun.operator->());
		Venus->SetRotationAxis(glm::vec3(0.f, -0.99897f, 0.04536f));
		Venus->SetRotationPeriod(243.01f * Planet::EarthDay);
		Venus->SetOrbitAxis(glm::vec3(0.f, 0.99813f, 0.06105f));
		Venus->SetOrbitDistance(0.728f * Planet::AstronomicalUnit);
		Venus->SetOrbitPeriod(224.7f * Planet::EarthDay);
		Venus->SetScale(0.949f * Planet::EarthScale);
		mPlanets.push_back(Venus);

		auto Earth = make_shared<Planet>("Content/Textures/Earthatday.png", Sun.operator->());
		Earth->SetRotationAxis(glm::vec3(0.f, 0.9175f, 0.3978f));
		Earth->SetRotationPeriod(Planet::EarthDay);
		Earth->SetOrbitAxis(glm::vec3(0.f, 1.f, 0.f));
		Earth->SetOrbitDistance(Planet::AstronomicalUnit);
		Earth->SetOrbitPeriod(Planet::EarthYear);
		Earth->SetScale(Planet::EarthScale);
		mPlanets.push_back(Earth);

		auto Moon = make_shared<Planet>("Content/Textures/2k_moon.png", Earth.operator->());
		Moon->SetRotationAxis(glm::vec3(0.f, 0.9932f, 0.11645f));
		Moon->SetRotationPeriod(27.321f * Planet::EarthDay);
		Moon->SetOrbitAxis(glm::vec3(0.f, 1.f, 0.f));
		Moon->SetOrbitDistance(0.5f * 0.3102317f * Planet::AstronomicalUnit);
		Moon->SetOrbitPeriod(27.321f * Planet::EarthDay);
		Moon->SetScale(0.2727f * Planet::EarthScale);
		mPlanets.push_back(Moon);

		auto Mars = make_shared<Planet>("Content/Textures/2k_mars.png", Sun.operator->());
		Mars->SetRotationAxis(glm::vec3(0.f, 0.90508f, 0.42524f));
		Mars->SetRotationPeriod(1.02f * Planet::EarthDay);
		Mars->SetOrbitAxis(glm::vec3(0.f, 0.99949f, 0.03199f));
		Mars->SetOrbitDistance(1.524f * Planet::AstronomicalUnit);
		Mars->SetOrbitPeriod(1.88f * Planet::EarthYear);
		Mars->SetScale(0.532f * Planet::EarthScale);
		mPlanets.push_back(Mars);

		auto Jupiter = make_shared<Planet>("Content/Textures/2k_jupiter.png", Sun.operator->());
		Jupiter->SetRotationAxis(glm::vec3(0.f, 0.99862f, 0.05251f));
		Jupiter->SetRotationPeriod(0.40972f * Planet::EarthDay);
		Jupiter->SetOrbitAxis(glm::vec3(0.f, 0.99979f, 0.02059f));
		Jupiter->SetOrbitDistance(5.203f * Planet::AstronomicalUnit);
		Jupiter->SetOrbitPeriod(11.86f * Planet::EarthYear);
		Jupiter->SetScale(11.19f * Planet::EarthScale);
		mPlanets.push_back(Jupiter);

		auto Saturn = make_shared<Planet>("Content/Textures/2k_saturn.png", Sun.operator->());
		Saturn->SetRotationAxis(glm::vec3(0.f, 0.89101f, 0.45399f));
		Saturn->SetRotationPeriod(0.42708f * Planet::EarthDay);
		Saturn->SetOrbitAxis(glm::vec3(0.f, 0.99905f, 0.04362f));
		Saturn->SetOrbitDistance(9.582f * Planet::AstronomicalUnit);
		Saturn->SetOrbitPeriod(29.41f * Planet::EarthYear);
		Saturn->SetScale(9.26f * Planet::EarthScale);
		mPlanets.push_back(Saturn);

		auto Uranus = make_shared<Planet>("Content/Textures/2k_uranus.png", Sun.operator->());
		Uranus->SetRotationAxis(glm::vec3(0.f, -0.13744f, 0.99051f));
		Uranus->SetRotationPeriod(0.71666f * Planet::EarthDay);
		Uranus->SetOrbitAxis(glm::vec3(0.f, 0.99991f, 0.01344f));
		Uranus->SetOrbitDistance(19.2f * Planet::AstronomicalUnit);
		Uranus->SetOrbitPeriod(84.04f * Planet::EarthYear);
		Uranus->SetScale(4.01f * Planet::EarthScale);
		mPlanets.push_back(Uranus);

		auto Neptune = make_shared<Planet>("Content/Textures/2k_neptune.png", Sun.operator->());
		Neptune->SetRotationAxis(glm::vec3(0.f, 0.86949f, 0.49394f));
		Neptune->SetRotationPeriod(0.67125f * Planet::EarthDay);
		Neptune->SetOrbitAxis(glm::vec3(0.f, 0.99952f, 0.03089f));
		Neptune->SetOrbitDistance(30.05f * Planet::AstronomicalUnit);
		Neptune->SetOrbitPeriod(163.72f * Planet::EarthYear);
		Neptune->SetScale(3.88f * Planet::EarthScale);
		mPlanets.push_back(Neptune);

		auto Pluto = make_shared<Planet>("Content/Textures/2k_pluto.jpg", Sun.operator->());
		Pluto->SetRotationAxis(glm::vec3(0.f, -0.52992f, 0.84805f));
		Pluto->SetRotationPeriod(6.3874f * Planet::EarthDay);
		Pluto->SetOrbitAxis(glm::vec3(0.f, 0.95579f, 0.29404f));
		Pluto->SetOrbitDistance(39.48f * Planet::AstronomicalUnit);
		Pluto->SetOrbitPeriod(247.93f * Planet::EarthYear);
		Pluto->SetScale(0.18f * Planet::EarthScale);
		mPlanets.push_back(Pluto);

	}

	void PlanetSystem::UpdateAmbientLight(const GameTime& gameTime)
	{
		static float ambientIntensity = mAmbientLight->Color().r;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_UP) && ambientIntensity < 1.0f)
		{
			ambientIntensity += gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::min(ambientIntensity, 1.0f);

			mAmbientLight->SetColor(vec4((vec3)ambientIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_DOWN) && ambientIntensity > 0.0f)
		{
			ambientIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::max(ambientIntensity, 0.0f);

			mAmbientLight->SetColor(vec4((vec3)ambientIntensity, 1.0f));
		}
	}

	void PlanetSystem::UpdatePointLight(const GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Upddate directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && directionalIntensity < 1.0f)
		{
			directionalIntensity += elapsedTime;
			directionalIntensity = std::min(directionalIntensity, 1.0f);

			mPointLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && directionalIntensity > 0.0f)
		{
			directionalIntensity -= elapsedTime;
			directionalIntensity = std::max(directionalIntensity, 0.0f);

			mPointLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}

		// Update the light's radius
		if (glfwGetKey(mGame->Window(), GLFW_KEY_V))
		{
			float radius = mPointLight->Radius() + LightModulationRate * elapsedTime;
			mPointLight->SetRadius(radius);
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_B))
		{
			float radius = mPointLight->Radius() - LightModulationRate * elapsedTime;
			radius = std::max(radius, 0.0f);
			mPointLight->SetRadius(radius);
		}
	}

	void PlanetSystem::UpdateSpecularLight(const GameTime& gameTime)
	{
		static float specularIntensity = 1.0f;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_INSERT) && specularIntensity < 1.0f)
		{
			specularIntensity += gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::min(specularIntensity, 1.0f);

			mSpecularColor = (vec4((vec3)specularIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_DELETE) && specularIntensity > 0.0f)
		{
			specularIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::max(specularIntensity, 0.0f);

			mSpecularColor = (vec4((vec3)specularIntensity, 1.0f));
		}

		static float specularPower = mSpecularPower;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_O) && specularPower < UCHAR_MAX)
		{
			specularPower += LightModulationRate * gameTime.ElapsedGameTimeSeconds().count();
			specularPower = std::min(specularPower, static_cast<float>(UCHAR_MAX));

			mSpecularPower = specularPower;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_P) && specularPower > 0.0f)
		{
			specularPower -= LightModulationRate * gameTime.ElapsedGameTimeSeconds().count();
			specularPower = std::max(specularPower, 0.0f);

			mSpecularPower = specularPower;
		}
	}

	void PlanetSystem::UpdateTimeScale(const Library::GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Upddate directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_DOWN))
		{
			mTimeScale -= elapsedTime;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_UP))
		{
			mTimeScale += elapsedTime;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT))
		{
			mTimeScale -= elapsedTime * 100;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT))
		{
			mTimeScale += elapsedTime * 100;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT_SHIFT))
		{
			mTimeScale = 1.f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_ENTER) == GLFW_PRESS && !mPausePressed)
		{
			if (mTimeScale)
			{
				mPausedTimescale = mTimeScale;
				mTimeScale = 0.f;
			}
			else
			{
				mTimeScale = mPausedTimescale;
			}
			mPausePressed = true;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_ENTER) == GLFW_RELEASE && mPausePressed)
		{
			mPausePressed = false;
		}
	}
}