#include "pch.h"
#include "pdirectx.h"
#include "colors.h"
#define MAX_DECIMALS 15
#define MAX_OPERATIONS 1000
#define DEFAULT_ID 0
#define DEFAULT_BTN 25
#define BTN_VEC ImVec2(DEFAULT_BTN, DEFAULT_BTN)

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

char expr[MAX_OPERATIONS][1024];
int decimals[MAX_OPERATIONS];
int tdecimals[MAX_OPERATIONS];
double output[MAX_OPERATIONS];
char buffer[MAX_OPERATIONS][10];
int id = DEFAULT_ID;
int operations = 1;

void ImGuiSetupStyle();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	snprintf(buffer[1], sizeof(buffer[1]), "%s%i%s", "%.", decimals[1], "f");
	D3D::Init(hInstance, L"ExpressionParser by rdbo");
	return 0;
}

void Solve(int i)
{
	output[i] = pinterp(expr[i]);
	snprintf(buffer[i], sizeof(buffer[i]), "%s%i%s", "%.", decimals[i], "f");
}

std::string CreateNewLabel(const char* label)
{
	id += 1;
	std::string s;
	s = label;
	s += "##";
	s += std::to_string(id);
	return s;
}

void ResetID()
{
	id = DEFAULT_ID;
}

void ResetOperation(int i)
{
	memset(expr[i], {}, sizeof(expr[i]));
	decimals[i] = 0;
	tdecimals[i] = 0;
	output[i] = 0;
	memset(buffer[i], {}, sizeof(buffer[i]));
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
	ImGui::Separator();
	ImGui::PushFont(roboto);
	for(int i = 1; i <= operations; i++)
	{
		ImGui::Text("Enter your expression: ");
		if (ImGui::InputText(CreateNewLabel("").c_str(), expr[i], sizeof(expr[i]), ImGuiInputTextFlags_EnterReturnsTrue))
			Solve(i);
		if (ImGui::Button(CreateNewLabel("Parse").c_str()))
			Solve(i);
		ImGui::Text("Output: "); ImGui::SameLine();
		ImGui::InputDouble(CreateNewLabel("").c_str(), &output[i], 0, 0, buffer[i], ImGuiInputTextFlags_ReadOnly);
		ImGui::Text("Decimals: "); ImGui::SameLine();
		if (ImGui::InputInt(CreateNewLabel("").c_str(), &tdecimals[i]))
		{
			if (tdecimals[i] > MAX_DECIMALS)
				tdecimals[i] = MAX_DECIMALS;

			else if (tdecimals[i] < 0)
				tdecimals[i] = 0;

			decimals[i] = tdecimals[i];
			snprintf(buffer[i], sizeof(buffer[i]), "%s%i%s", "%.", decimals[i], "f");
		}
		ImGui::Separator();
	}
	ImGui::Text("Add/Remove operations"); ImGui::SameLine();
	if(ImGui::Button(CreateNewLabel("+").c_str(), BTN_VEC))
		operations + 1 <= MAX_OPERATIONS ? operations += 1 : 0;
	ImGui::SameLine();
	if (ImGui::Button(CreateNewLabel("-").c_str(), BTN_VEC))
	{
		ResetOperation(operations);
		operations - 1 >= 0 ? operations -= 1 : 0;
	}
	ResetID();
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
	style.WindowRounding = 0.f;
	colors[ImGuiCol_Text] = COLOR_WHITE;
	colors[ImGuiCol_FrameBg] =
		colors[ImGuiCol_FrameBgHovered] =
		colors[ImGuiCol_FrameBgActive] = COLOR_GRAY;
	colors[ImGuiCol_Button] = COLOR_DARK_BLUE;
	colors[ImGuiCol_ButtonHovered] = COLOR_BLUE;
	colors[ImGuiCol_ButtonActive] = COLOR_LIGHT_BLUE;
	colors[ImGuiCol_ScrollbarBg] = COLOR_DARK_GRAY;
	colors[ImGuiCol_ScrollbarGrab] = COLOR_LIGHT_GRAY;
	colors[ImGuiCol_ScrollbarGrabHovered] = COLOR_LIGHTER_GRAY;
	colors[ImGuiCol_ScrollbarGrabActive] = COLOR_LIGHTER_GRAY_2;
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