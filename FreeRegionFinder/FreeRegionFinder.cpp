#include <iostream>
#include <vector>
#include <Windows.h>
#include <memoryapi.h>

std::vector<MEMORY_BASIC_INFORMATION> GetMemoryInfo(DWORD processId) {
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle == nullptr) {
        std::cerr << "Failed to open process with ID: " << processId << std::endl;
        return {};
    }

    std::vector<MEMORY_BASIC_INFORMATION> memoryRegions;
    MEMORY_BASIC_INFORMATION memInfo;
    LPVOID address = nullptr;

    while (VirtualQueryEx(processHandle, address, &memInfo, sizeof(memInfo)) != 0) {
        memoryRegions.push_back(memInfo);
        address = static_cast<LPBYTE>(memInfo.BaseAddress) + memInfo.RegionSize;
    }

    CloseHandle(processHandle);
    return memoryRegions;
}

int main() {
    DWORD processId;
    std::cout << "Enter ProcessID: ";
    std::cin >> processId;

    auto memoryRegions = GetMemoryInfo(processId);

    if (!memoryRegions.empty()) {
        std::cout << "Free memory areas of the process with ID " << processId << ":\n";
        for (const auto& memRegion : memoryRegions) {
            if (memRegion.State == MEM_FREE) {
                std::cout << "Adress: " << memRegion.BaseAddress
                    << ", Size: " << memRegion.RegionSize << " byte's\n";
            }
        }
    }

    return 0;
}
