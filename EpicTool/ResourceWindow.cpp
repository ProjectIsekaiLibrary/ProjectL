#include "ResourceWindow.h"
#include <filesystem>
#include "KunrealAPI.h"

EpicTool::ResourceWindow::ResourceWindow()
{

}

EpicTool::ResourceWindow::~ResourceWindow()
{

}

void EpicTool::ResourceWindow::ShowWindow()
{
    ImGui::Begin("Resource Window");

    ImGui::Text("Resource List");

    ImGui::End();
}

void EpicTool::ResourceWindow::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
