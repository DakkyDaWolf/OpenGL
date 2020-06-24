#pragma once

#include "RTTI.h"
#include <stack>
#include <gsl\gsl>
#include <GL/gl3w.h>
#include "Framebuffer.h"

namespace Library
{
	class RenderTarget : public RTTI
	{
		RTTI_DECLARATIONS(RenderTarget, RTTI)

	public:
		RenderTarget() = default;
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = default;
		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget& operator=(RenderTarget&&) = default;
		virtual ~RenderTarget() = default;

		void GenTexture(GLuint width, GLuint height);

		GLuint GetTextureID();

		void Begin();
		void End();
		void RebindCurrentRenderTargets();

	protected:
		std::shared_ptr<Framebuffer> mDrawTarget;

	private:
		static std::stack<std::shared_ptr<Framebuffer>> sRenderTargetStack;
	};
}