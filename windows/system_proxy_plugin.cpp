#include "system_proxy_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>
#include <wininet.h>            
#include <iostream>


#pragma comment(lib, "wininet.lib")

namespace system_proxy {

	// static
	void SystemProxyPlugin::RegisterWithRegistrar(
		flutter::PluginRegistrarWindows* registrar) {
		auto plugin = std::make_unique<SystemProxyPlugin>();
		SystemProxyHostApi::SetUp(
			registrar->messenger(), plugin.get());
		registrar->AddPlugin(std::move(plugin));
	}

	SystemProxyPlugin::SystemProxyPlugin() {}

	SystemProxyPlugin::~SystemProxyPlugin() {}

	//void SystemProxyPlugin::HandleMethodCall(
	//    const flutter::MethodCall<flutter::EncodableValue> &method_call,
	//    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
	//  if (method_call.method_name().compare("getPlatformVersion") == 0) {
	//    std::ostringstream version_stream;
	//    version_stream << "Windows ";
	//    if (IsWindows10OrGreater()) {
	//      version_stream << "10+";
	//    } else if (IsWindows8OrGreater()) {
	//      version_stream << "8";
	//    } else if (IsWindows7OrGreater()) {
	//      version_stream << "7";
	//    }
	//    result->Success(flutter::EncodableValue(version_stream.str()));
	//  } else {
	//    result->NotImplemented();
	//  }
	//}
	//static void PrintWString(const std::wstring& wstr) {
	//	std::wcout << wstr << std::endl;
	//}

	static std::string LastErrorToString(DWORD errorCode) {
		if (errorCode == 0) {
			return std::string();
		}

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer,
			0,
			NULL
		);

		std::string message(messageBuffer, size);

		LocalFree(messageBuffer);

		return message;
	}

	// Helper function to convert std::string to std::wstring
	static std::wstring StringToWstring(const std::string& str) {
		if (str.empty()) {
			throw std::runtime_error("str is empty");
		}

		// Get the required buffer size
		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		if (size <= 0) {
			throw std::runtime_error("Failed to convert string to wide string");
		}

		// Create the buffer and convert
		std::vector<wchar_t> buf(size);
		if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), size) <= 0) {
			throw std::runtime_error("Failed to convert string to wide string");
		}

		return std::wstring(buf.data());
	}


	std::optional<FlutterError> SystemProxyPlugin::SetSystemProxy(const SystemProxySettings& settings) {
		INTERNET_PER_CONN_OPTION_LIST optList;
		INTERNET_PER_CONN_OPTION opts[3];
		DWORD   dwBufSize = sizeof(optList);
		std::wstring httpAddress;
		std::wstring socksAddress;
		std::wstring bypass;
		// Setup the options
		opts[0].dwOption = INTERNET_PER_CONN_FLAGS;
		opts[0].Value.dwValue = PROXY_TYPE_PROXY | PROXY_TYPE_DIRECT;

		opts[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
		if (settings.socks_proxy() && !(*settings.socks_proxy()).empty()) {
			socksAddress = StringToWstring(*settings.socks_proxy());
			opts[1].Value.pszValue = const_cast<LPWSTR>(socksAddress.c_str());
		}
		else if (settings.http_proxy() && !(*settings.http_proxy()).empty()) {
			httpAddress = StringToWstring(*settings.http_proxy());
			opts[1].Value.pszValue = const_cast<LPWSTR>(httpAddress.c_str());
		}
		else {
			return FlutterError("No proxy server specified");
		}
		
		if (settings.bypass() && !(*settings.bypass()).empty()) {
			opts[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
			bypass = StringToWstring(*settings.bypass());
			opts[2].Value.pszValue = const_cast<LPWSTR>(bypass.c_str());
		}

		// Fill the options list structure
		optList.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
		optList.pszConnection = NULL;    
		optList.dwOptionCount = settings.bypass() && !(*settings.bypass()).empty()? 3 : 2;
		optList.dwOptionError = 0;
		optList.pOptions = opts;

		// Set the options
		BOOL result = InternetSetOption(
			NULL,
			INTERNET_OPTION_PER_CONNECTION_OPTION,
			&optList,
			dwBufSize
		);

		if (result) {
			// Notify the system that the settings have changed
			InternetSetOption(NULL, INTERNET_OPTION_PROXY_SETTINGS_CHANGED, NULL, 0);
			InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
			InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
		}
		else {
			DWORD errorCode = GetLastError();
			std::string errorMsg = LastErrorToString(errorCode);
			return FlutterError("InternetSetOption failed, " + errorMsg);
		}

		return std::nullopt;
	}

	std::optional<FlutterError> SystemProxyPlugin::RemoveSystemProxy() {
		// Define the options for disabling the proxy
		INTERNET_PER_CONN_OPTION_LIST optionList;
		INTERNET_PER_CONN_OPTION options[1];

		// Set the option to not use a proxy
		options[0].dwOption = INTERNET_PER_CONN_FLAGS;
		options[0].Value.dwValue = PROXY_TYPE_DIRECT | PROXY_TYPE_AUTO_DETECT;

		// Set up the option list structure
		optionList.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
		optionList.pszConnection = NULL; 
		optionList.dwOptionCount = 1;
		optionList.dwOptionError = 0;
		optionList.pOptions = options;

		// Apply the settings
		if (!InternetSetOptionW(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &optionList, sizeof(optionList))) {
			DWORD errorCode = GetLastError();
			return FlutterError("InternetSetOption failed, ", LastErrorToString(errorCode));
		}
		// Notify the system that the settings have changed
		InternetSetOptionW(NULL, INTERNET_OPTION_PROXY_SETTINGS_CHANGED, NULL, 0);
		InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOptionW(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
		return std::nullopt;
	}

}  // namespace system_proxy
