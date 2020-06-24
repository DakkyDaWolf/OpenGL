#pragma once
#include "Texture2D.h"

namespace Library
{

	class Framebuffer : public RTTI
	{
		RTTI_DECLARATIONS(Framebuffer, RTTI)

	public:
		/// <summary>
		/// Default framebuffer ID of 0 refers to the viewport, drawing to screen
		/// </summary>
		Framebuffer() = default;

		/// <summary>
		/// Initialize a framebuffer with the given texture as the render target
		/// </summary>
		/// <param name="targetTexture">texture that the framebuffer will draw to</param>
		explicit Framebuffer(std::shared_ptr<Texture2D> targetTexture);

		Framebuffer(const Framebuffer&) = default;
		Framebuffer(Framebuffer&&) noexcept = default;
		Framebuffer& operator=(const Framebuffer&) = default;
		Framebuffer& operator=(Framebuffer&&) noexcept = default;
		~Framebuffer();

		GLuint FramebufferID() const;
		std::shared_ptr<Texture2D> GetTexture();

		/// <summary>
		/// Generate a framebuffer, also generating a new texture to associate with it
		/// </summary>
		/// <param name="width">width of the associated texture</param>
		/// <param name="height">height of the associated texture</param>
		/// <returns>shared_ptr to the newly created Framebuffer</returns>
		static std::shared_ptr<Framebuffer> CreateFramebuffer(GLuint width, GLuint height);
		static std::shared_ptr<Framebuffer> CreateFramebuffer(std::shared_ptr<Texture2D> targetTexture);

	private:

		GLuint mFramebufferID{ 0 };
		std::shared_ptr<Texture2D> mAssociatedTexture;
	};

}