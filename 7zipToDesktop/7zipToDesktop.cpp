// 7zipToDesktop.cpp : Defines the entry point for the application.
//
// License: GPLv3
// author : github.com/takuya
// modified : 2024-05-29
//
#include<iostream>
#include <Windows.h>
#include <filesystem>
#include <regex>

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
wstring get_output_path(wstring src) {

	std::string dstDir = get_desktop().string();

	fs::path src_path(src);
	fs::path basename = src_path.stem();
	std::regex extensionRegex("\\.\\w+$");
	std::string basename_noext = std::regex_replace(basename.string(), extensionRegex, "");
	fs::path dstPath = fs::path(dstDir) / basename_noext;
	return dstPath.wstring();
}
void openExplorer(wstring path) {
	wstring application(L"explorer.exe");
	wstring out_dir = get_output_path(path);
	if (!fs::exists(out_dir)) {
		return;
	}
	HINSTANCE result = ShellExecute(nullptr, nullptr,
		application.c_str(),
		out_dir.c_str(),
		NULL,
		SW_SHOWNORMAL
	);
}

int extractToDesktop(wstring archivePath) {
	fs::path desktopPath = get_desktop();
	if (desktopPath.empty()) {
		return 1;
	}
	wstring args = archivePath;
	args = L" x " + args + L" -aos -o" + desktopPath.wstring() + L"\\*";
	wstring p7zip = L"\"C:\\Program Files\\7-Zip\\7zG.exe\"";
	wstring cmd = p7zip + args;
	//
	//wstring cmdW(cmd.begin(), cmd.end());
	//wstring cmdW = stringToWString(cmd);
	LPCWSTR lpwargs = cmd.c_str();
	LPWSTR  lpargs = const_cast<LPWSTR>(lpwargs);

	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};
	if (CreateProcessW(NULL, lpargs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else {
		std::cerr << "Failed to execute command." << std::endl;
		return 2;
	}
	return 0;
}


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

	if (lpCmdLine == NULL) {
		return 1;
	}
	//
	wstring args = lpCmdLine;
	extractToDesktop(args);
	openExplorer(args);
	return 0;


}
