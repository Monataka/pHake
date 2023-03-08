#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <vector>
#include <math.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <Psapi.h> 

struct ProcessModule
{
	uintptr_t base, size;
};

class pProcess
{
public:
	DWORD		  pid_; // process id
	HANDLE		  handle_; // handle to process
	ProcessModule base_module_;

public:
	bool AttachProcess(const char* process_name);
	bool AttachWindow(const char* window_name);
	void Close();

public:
	ProcessModule GetModule(const char* module_name);
	LPVOID		  Allocate(size_t size_in_bytes);
	uintptr_t	  FindCodeCave(uint32_t length_in_bytes);
	uintptr_t     FindSignature(std::vector<uint8_t> signature);
	uintptr_t     FindSignature(ProcessModule target_module, std::vector<uint8_t> signature);

	template<class T>
	uintptr_t ReadOffsetFromSignature(std::vector<uint8_t> signature, uint8_t offset) // offset example: "FF 05 ->22628B01<-" offset is 2
	{
		uintptr_t pattern_address = this->FindSignature(signature);
		if (!pattern_address)
			return 0x0;
		
		T offset_value = this->read<T>(pattern_address + offset);
		return pattern_address + offset_value + offset + sizeof(T);
	}

	bool read_raw(uintptr_t address, void* buffer, size_t size)
	{
		SIZE_T bytesRead;
		if (ReadProcessMemory(this->handle_, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead))
		{
			return bytesRead == size;
		}
		return false;
	}

	template<class T>
	void write(uintptr_t address, T value)
	{
		WriteProcessMemory(handle_, (void*)address, &value, sizeof(T), 0);
	}

	template<class T>
	T read(uintptr_t address)
	{
		T buffer{};
		ReadProcessMemory(handle_, (void*)address, &buffer, sizeof(T), 0);
		return buffer;
	}

	void write_bytes(uintptr_t addr, std::vector<uint8_t> patch)
	{
		WriteProcessMemory(handle_, (void*)addr, &patch[0], patch.size(), 0);
	}

	uintptr_t read_multi_address(uintptr_t ptr, std::vector<uintptr_t> offsets)
	{
		uintptr_t buffer = ptr;
		for (int i = 0; i < offsets.size(); i++)
			buffer = this->read<uintptr_t>(buffer + offsets[i]);
		
		return buffer;
	}

	template <typename T>
	T read_multi(uintptr_t base, std::vector<uintptr_t> offsets) {
		uintptr_t buffer = base;
		for (int i = 0; i < offsets.size() - 1; i++)
		{
			buffer = this->read<uintptr_t>(buffer + offsets[i]);
		}
		return this->read<T>(buffer + offsets.back());
	}

private:
	uint32_t FindProcessIdByProcessName(const char* process_name);
	uint32_t FindProcessIdByWindowName(const char* window_name);
};
#endif