#include "pch.h"
#include "Framebuffer.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Framebuffer)

	Framebuffer::Framebuffer(shared_ptr<Texture2D> targetTexture) :
		mAssociatedTexture(targetTexture)
	{
		glGenFramebuffers(1, &mFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mAssociatedTexture->TextureID(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //revert to default framebuffer after initialization
	}

	Framebuffer::~Framebuffer()
	{
	}

	GLuint Framebuffer::FramebufferID() const
	{
		return mFramebufferID;
	}

	std::shared_ptr<Texture2D> Framebuffer::GetTexture()
	{
		return mAssociatedTexture;
	}

	std::shared_ptr<Framebuffer> Framebuffer::CreateFramebuffer(GLuint width, GLuint height)
	{
		auto texture = Texture2D::CreateTexture2D(width, height);
		return CreateFramebuffer(texture);
	}

	std::shared_ptr<Framebuffer> Framebuffer::CreateFramebuffer(std::shared_ptr<Texture2D> targetTexture)
	{
		return make_shared<Framebuffer>(targetTexture);
	}

}