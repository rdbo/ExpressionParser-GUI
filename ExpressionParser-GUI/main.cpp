#include "pch.h"
#include "pdirectx.h"
#include "colors.h"
#define MAX_DECIMALS 15

WNDCLASSEX wc;
HWND hwnd;
RECT wCRect;
IDirect3D9* pD3D;
D3DPRESENT_PARAMETERS d3dpp;
IDirect3DDevice9* pDevice;
ImGuiWindowFlags wf = 0;
bool resized;
ImFont* roboto;
ImFont* roboto2;

char expr[1024];
int decimals = 5;
int tdecimals = decimals;
double output = 0;
char buffer[10];

void ImGuiSetupStyle();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	snprintf(buffer, sizeof(buffer), "%s%i%s", "%.", decimals, "f");
	D3D::Init(hInstance, L"ExpressionParser by rdbo");
	return 0;
}

void Solve()
{
	output = pinterp(expr);
	snprintf(buffer, sizeof(buffer), "%s%i%s", "%.", decimals, "f");
}

bool initstyle = false;
void D3D::DrawImGui()
{
	if (!initstyle)
	{
		ImGuiSetupStyle();
		initstyle = true;
	}

	ImGui::Begin("ExpressionParser", 0, wf);

	ImGui::PushFont(roboto2);
	ImGui::TextColored(COLOR_BLUE, "ExpressionParser");
	ImGui::PopFont();

	ImGui::PushFont(roboto);
	ImGui::Text("Enter your expression:");
	if (ImGui::InputText("", expr, sizeof(expr), ImGuiInputTextFlags_EnterReturnsTrue))
		Solve();
	if (ImGui::Button("Parse"))
		Solve();
	ImGui::Text("Output: "); ImGui::SameLine();
	ImGui::InputDouble(" ", &output, 0, 0, buffer, ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Decimals: "); ImGui::SameLine();
	if (ImGui::InputInt("   ", &tdecimals))
	{
		if (tdecimals > MAX_DECIMALS)
			tdecimals = MAX_DECIMALS;

		else if (tdecimals < 0)
			tdecimals = 0;

		decimals = tdecimals;
		snprintf(buffer, sizeof(buffer), "%s%i%s", "%.", decimals, "f");
	}
	ImGui::PopFont();
	ImGui::End();
}

void ImGuiSetupStyle()
{
	wf |= ImGuiWindowFlags_NoTitleBar;
	wf |= ImGuiWindowFlags_NoMove;
	wf |= ImGuiWindowFlags_NoResize;
	wf |= ImGuiWindowFlags_NoCollapse;
	wf |= ImGuiWindowFlags_NoSavedSettings;
	wf |= ImGuiWindowFlags_NoBackground;
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = COLOR_WHITE;
	colors[ImGuiCol_FrameBg] =
		colors[ImGuiCol_FrameBgHovered] =
		colors[ImGuiCol_FrameBgActive] = COLOR_GRAY;
	colors[ImGuiCol_Button] = COLOR_DARK_BLUE;
	colors[ImGuiCol_ButtonHovered] = COLOR_BLUE;
	colors[ImGuiCol_ButtonActive] = COLOR_LIGHT_BLUE;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK D3D::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (pDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
			GetClientRect(hwnd, &wCRect);
			resized = true;
		}
		return 0;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = MIN_WIDTH;
		lpMMI->ptMinTrackSize.y = MIN_HEIGHT;
		break;
	}
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}