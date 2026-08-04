#include <Perplex/pch.h>
#include <Perplex/Platform/SystemUtils.h>
#include <Perplex/Parsing/Parser.h>

#include <Windows.h>
#include <shlobj.h>

#include <filesystem>
#include <string>
#include <sstream>
#include <cstdlib>
#include <optional>

#include <GLFW/glfw3.h>

namespace fs = std::filesystem;

namespace Perplex
{
	fs::path NativePath(const fs::path& path)
	{
		std::string nativePathStr{ path.string() };
		ReplaceAll(nativePathStr, "/", "\\");
		return fs::path{ nativePathStr };
	}

	void OpenFilesystemGui(const fs::path& path)
	{
		std::stringstream commandStream;
		commandStream << "explorer " << path.string();

		std::string commandString = commandStream.str();
		std::system(commandString.c_str());
	}

    // For user-specific data
    fs::path PerplexAppUserPath()
    {
        TCHAR szPath[MAX_PATH];

        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath)))
            return fs::path{ szPath } / "Perplex";

        HW_CORE_ERROR("Failed find or write to the user's AppData folder!");
        return fs::path{};
    }

    // For general application data
	fs::path PerplexAppDataPath()
	{
		TCHAR szPath[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath)))
			return fs::path{ szPath } / "Perplex";

		HW_CORE_ERROR("Failed find or write to the user's AppData folder!");
		return fs::path{};
	}

    fs::path FileDialogue()
	{
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
            COINIT_DISABLE_OLE1DDE);
        fs::path returnPath{};

        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                // Show the Open dialog box.
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            returnPath = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }

        return returnPath;
	}

    float GetTime()
    {
        return (float)glfwGetTime();
    }
}