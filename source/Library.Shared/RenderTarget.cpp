#include "pch.h"
#include "RenderTarget.h"

using namespace std;
using namespace gsl;

namespace Library
{
	RTTI_DEFINITIONS(RenderTarget)

	stack<shared_ptr<Framebuffer>> RenderTarget::sRenderTargetStack;

	void RenderTarget::GenTexture(GLuint width, GLuint height)
	{
		mDrawTarget = Framebuffer::CreateFramebuffer(width, height);
	}

	GLuint RenderTarget::GetTextureID()
	{
		return mDrawTarget ? (mDrawTarget->GetTexture() ? mDrawTarget->GetTexture()->TextureID() : 0 ) : 0;
	}

	void RenderTarget::Begin()
	{
		sRenderTargetStack.emplace(mDrawTarget);
		if (mDrawTarget) glBindFramebuffer(GL_FRAMEBUFFER, mDrawTarget->FramebufferID());
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTarget::End()
	{
		sRenderTargetStack.pop();

		auto renderTargetData = sRenderTargetStack.size() > 0 ? sRenderTargetStack.top() : nullptr;
		if (renderTargetData) glBindFramebuffer(GL_FRAMEBUFFER, renderTargetData->FramebufferID());
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTarget::RebindCurrentRenderTargets()
	{
		auto renderTargetData = sRenderTargetStack.size() > 0 ? sRenderTargetStack.top() : nullptr;
		if (renderTargetData) glBindFramebuffer(GL_FRAMEBUFFER, renderTargetData->FramebufferID());
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}