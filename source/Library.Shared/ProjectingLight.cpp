#include "pch.h"
#include "ProjectingLight.h"

namespace Library
{
	RTTI_DEFINITIONS(ProjectingLight)

	ProjectingLight::ProjectingLight(Game& game) :
		PerspectiveCamera(game), Light(game)
	{
	}

	void ProjectingLight::Initialize()
	{
		PerspectiveCamera::Initialize();
		Light::Initialize();
		SetAspectRatio(1.f);

		glGenFramebuffers(1, &mDepthMapFBOID);

		glGenTextures(1, &mDepthMapTextureID);
		glBindTexture(GL_TEXTURE_2D, mDepthMapTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mDepthMapW, mDepthMapH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBOID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMapTextureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ProjectingLight::Update(const GameTime& gameTime)
	{
		PerspectiveCamera::Update(gameTime);
		UpdateProjectionMatrix();
	}

	void ProjectingLight::ClearBuffer()
	{
		static const GLfloat one = 1.0f;
		glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBOID);
		glClearBufferfv(GL_DEPTH, 0, &one);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	float ProjectingLight::InnerAngle() const
	{
		return mInnerAngle;
	}

	void ProjectingLight::SetInnerAngle(float value)
	{
		mInnerAngle = value;
	}

	float ProjectingLight::OuterAngle() const
	{
		return mOuterAngle;
	}

	void ProjectingLight::SetOuterAngle(float value)
	{
		mOuterAngle = value;
	}

	float ProjectingLight::AttenuationRadius() const
	{
		return mAttenuationRadius;
	}

	void ProjectingLight::SetAttenuationRadius(float radius)
	{
		mAttenuationRadius = radius;
	}

	GLuint ProjectingLight::FrameBuffer() const
	{
		return mDepthMapFBOID;
	}

	GLuint ProjectingLight::DepthMapTexture() const
	{
		return mDepthMapTextureID;
	}

	GLuint ProjectingLight::DepthMapWidth() const
	{
		return mDepthMapW;
	}

	GLuint ProjectingLight::DepthMapHeight() const
	{
		return mDepthMapH;
	}

}