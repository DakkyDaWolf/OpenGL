#include "pch.h"
#include "ImGuiComponent.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <gl/gl3w.h>
#include "Game.h"
#include <map>

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ImGuiComponent)

	ImGuiComponent::ImGuiComponent(Game& game, Styles style, bool useCustomDraw) :
		DrawableGameComponent(game), mStyle(style), mUseCustomDraw(useCustomDraw)
	{
	}

	ImGuiComponent::Styles ImGuiComponent::Style() const
	{
		return mStyle;
	}

	void ImGuiComponent::SetStyle(Styles style)
	{
		static const map<Styles, function<void(ImGuiStyle*)>> styleMap
		{
			{ Styles::Classic, ImGui::StyleColorsClassic },
			{ Styles::Light, ImGui::StyleColorsLight },
			{ Styles::Dark, ImGui::StyleColorsDark }
		};

		mStyle = style;
		styleMap.at(mStyle)(nullptr);
	}

	void ImGuiComponent::Initialize()
	{
		//IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(mGame->Window(), true);
		ImGui_ImplOpenGL3_Init("#version 440 core");

		SetStyle(mStyle);
	}

	void ImGuiComponent::Shutdown()
	{		
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiComponent::Draw(const GameTime&)
	{
		if (!mUseCustomDraw)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			for (auto& block : mRenderBlocks)
			{
				(*block)();
			}

			ImGui::Render();
			
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	bool ImGuiComponent::UseCustomDraw() const
	{
		return mUseCustomDraw;
	}

	void ImGuiComponent::SetUseCustomDraw(bool useCustomDraw)
	{
		mUseCustomDraw = useCustomDraw;
	}

	void ImGuiComponent::CustomDraw()
	{
		assert(mUseCustomDraw);
		ImGui::Render();
	}

	const std::vector<std::shared_ptr<ImGuiComponent::RenderBlock>>& ImGuiComponent::RenderBlocks() const
	{
		return mRenderBlocks;
	}

	void ImGuiComponent::AddRenderBlock(std::shared_ptr<RenderBlock> block)
	{
		mRenderBlocks.push_back(block);
	}

	void ImGuiComponent::RemoveRenderBlock(std::shared_ptr<RenderBlock> block)
	{
		mRenderBlocks.erase(find(mRenderBlocks.cbegin(), mRenderBlocks.cend(), block));
	}
}