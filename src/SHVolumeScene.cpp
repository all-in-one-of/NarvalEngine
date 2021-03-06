#include "SHVolumeScene.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


SHVolumeScene::SHVolumeScene()
{
}

void SHVolumeScene::renderImGUI() {
	ImGui::SetNextWindowPos(ImVec2(40, 170), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Once);
	ImGui::Begin("Volume Properties", p_open, window_flags);
	ImGui::DragFloat3("Absorption (1/m)", &absorption[0], 0.001f, 0.001f, 5.0f);
	ImGui::DragFloat3("Scattering (1/m)", &scattering[0], 0.001f, 0.001f, 5.0f);
	if (lockScattering) {
		scattering[1] = scattering[0];
		scattering[2] = scattering[0];
	}
	ImGui::Checkbox("Lock Scattering", &lockScattering);

	ImGui::DragFloat("Density Multiplier", &densityCoef, 0.01, 0.0, 30.0);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(40, 410), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);

	ImGui::Begin("Light Properties", p_open, window_flags);
	ImGui::DragFloat3("Light Pos (m)", &lightPosition[0], 0.1, 0.1, 0.1);
	ImGui::DragFloat3("L. color", &lightColor[0], 0.01, 0.0, 30.0);

	ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.1, 0.0, 100.0);
	ImGui::DragFloat("g", &g, 0.01, -1.0, 1.0);
	ImGui::DragFloat("N. of Steps", &numberOfSteps, 1, 0.0, 256);
	ImGui::DragFloat("Shadow Steps", &shadowSteps, 1, 0.0, 256);
	ImGui::Checkbox("Enable Shadow", &enableShadow);


	ImGui::Checkbox("Night Mode", &nightMode);

	const char* items[] = { "Isotropic", "Rayleigh", "Henyey-Greenstein" };
	ImGui::Combo("Phase Function", &phaseFunctionOption, items, IM_ARRAYSIZE(items));
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(40, 810), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_Once);
}


SHVolumeScene::~SHVolumeScene()
{
}
