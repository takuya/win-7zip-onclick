// 7zipToDesktop.cpp : Defines the entry point for the application.
//
// License: GPLv3
// author : github.com/takuya
// modified : 2024-05-29
//

#include<iostream>
#include <Windows.h>
#include <filesystem>

namespace fs = std::filesystem;
using string = std::string;
using wstring = std::wstring;

fs::path get_desktop() {
	char* env_var = nullptr;
	size_t size;
	_dupenv_s(&env_var, &size, "USERPROFILE");
	if (env_var == nullptr) {
		return "";
	}
	std::string UserProfile(env_var);
	return fs::path(UserProfile) / "Desktop";
}
string get_output_path(string src) {

	std::string dstDir = get_desktop().string();

	fs::path src_path(src);
	fs::path basename = src_path.stem().replace_extension();
	fs::path dstPath = fs::path(dstDir) / basename;
	return dstPath.string();
}
void openExplorer(string path) {
	string application("explorer.exe");
	HINSTANCE result = ShellExecuteA(nullptr, nullptr,
		application.c_str(),
		get_output_path(path).c_str(),
		NULL,
		SW_SHOWNORMAL
	);
}
int extractToDesktop(string archivePath) {
	fs::path desktopPath = get_desktop();
	if (desktopPath.empty()) {
		return 1;
	}
	string args = archivePath;
	args = " x " + args + " -aos -o" + desktopPath.string() + "\\*";
	string cmd = R"(C:\Program Files\7-Zip\7zG.exe)" + args;
	wstring cmdW(cmd.begin(), cmd.end());
	LPCWSTR lpwargs = cmdW.c_str();
	LPWSTR  lpargs = const_cast<LPWSTR>(lpwargs);

	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};
	if (CreateProcessW(NULL, lpargs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		// 終了コードを取得
		DWORD exitCode;
		if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
			std::cerr << "GetExitCodeProcess failed, error: " << GetLastError() << std::endl;
			return 1;
		}
	}
	else {
		std::cerr << "Failed to execute command." << std::endl;
		return 2;
	}
	return 0;
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	if (lpCmdLine == NULL) {
		return 1;
	}
	// std::string を std::wstring に変換
	string args = lpCmdLine;
	int ret = extractToDesktop(args);
	if (ret > 0) {
		return ret;
	}
	openExplorer(args);
	return 0;


}

