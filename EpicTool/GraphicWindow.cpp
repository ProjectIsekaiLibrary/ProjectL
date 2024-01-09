#include "imgui.h"
#include "GraphicWindow.h"
#include "KunrealAPI.h"

EpicTool::GraphicWindow::GraphicWindow()
	
{

}

EpicTool::GraphicWindow::~GraphicWindow()
{

}

void EpicTool::GraphicWindow::ShowWindow(void* _texture)
{
	// 생성 실패
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");


	ImGui::Begin("GraphicsWindow");//, ImGuiWindowFlags_NoMove);

	ImVec2 imageSize(1280, 720); // 이미지 크기
	
	ImGui::Image(_texture, imageSize); // 텍스처를 이미지로 출력

	ImGuiIO& io = ImGui::GetIO();
	auto _fps = io.Framerate;
	ImGui::Text("FPS: %.1f", _fps);

	ImGui::End();
	
}

void EpicTool::GraphicWindow::ShowWindow()
{
    
}

void EpicTool::GraphicWindow::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
