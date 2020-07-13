#include "pch.h"
#include "Game.h"
#include "DeferredFramebuffer.h"

namespace Library
{

	RTTI_DEFINITIONS(DeferredFramebuffer)

	DeferredFramebuffer::DeferredFramebuffer(Game& game) :
		GameComponent(game)
	{
	}

	DeferredFramebuffer::~DeferredFramebuffer()
	{
		glDeleteTextures(1, &mPositionTextureID);
		glDeleteTextures(1, &mNormalTextureID);
		glDeleteTextures(1, &mAlbedoSpecTextureID);
		glDeleteFramebuffers(1, &mFramebufferID);
		glDeleteRenderbuffers(1, &mDepthRenderbufferID);
	}

	void DeferredFramebuffer::Initialize()
	{
		GameComponent::Initialize();

		glGenFramebuffers(1, &mFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

		glGenTextures(1, &mPositionTextureID);
		glBindTexture(GL_TEXTURE_2D, mPositionTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPositionTextureID, 0);

		glGenTextures(1, &mNormalTextureID);
		glBindTexture(GL_TEXTURE_2D, mNormalTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormalTextureID, 0);

		glGenTextures(1, &mAlbedoSpecTextureID);
		glBindTexture(GL_TEXTURE_2D, mAlbedoSpecTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mAlbedoSpecTextureID, 0);

		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		glGenRenderbuffers(1, &mDepthRenderbufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderbufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mGame->ScreenWidth(), mGame->ScreenHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderbufferID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); //revert to default framebuffer after initialization
	}

	GLuint DeferredFramebuffer::PositionID() const
	{
		return mPositionTextureID;
	}

	GLuint DeferredFramebuffer::NormalID() const
	{
		return mNormalTextureID;
	}

	GLuint DeferredFramebuffer::ColorID() const
	{
		return mAlbedoSpecTextureID;
	}

	GLuint DeferredFramebuffer::BufferID() const
	{
		return mFramebufferID;
	}

	void DeferredFramebuffer::ClearBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}