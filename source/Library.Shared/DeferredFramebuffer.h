#pragma once
#include "GL/gl3w.h"
#include "GameComponent.h"

namespace Library
{

	class DeferredFramebuffer : public GameComponent
	{
		RTTI_DECLARATIONS(DeferredFramebuffer, GameComponent)

	public:
		DeferredFramebuffer() = default;
		explicit DeferredFramebuffer(Game & game);
		DeferredFramebuffer(const DeferredFramebuffer&) = default;
		DeferredFramebuffer& operator=(const DeferredFramebuffer&) = default;
		DeferredFramebuffer(DeferredFramebuffer&&) = default;
		DeferredFramebuffer& operator=(DeferredFramebuffer&&) = default;
		virtual ~DeferredFramebuffer();

		virtual void Initialize() override;

		GLuint PositionID() const;
		GLuint NormalID() const;
		GLuint ColorID() const;
		GLuint BufferID() const;

		void ClearBuffer();

	protected:
		GLuint mFramebufferID{ 0 };
		GLuint mPositionTextureID{ 0 };
		GLuint mNormalTextureID{ 0 };
		GLuint mAlbedoSpecTextureID{ 0 };
		GLuint mDepthRenderbufferID{ 0 };

	};

}