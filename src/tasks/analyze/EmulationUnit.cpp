/*
 *  Filename	: EmulationUnit.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 12-05-2012
 */

#include "EmulationUnit.h"
#include <utils/SystemLogger.h>
#if 0
#include <core/UserHooks.h>
#endif

EmulationUnit::EmulationUnit()
{
	prepareUnit();
}

EmulationUnit::~EmulationUnit()
{
	destroyUnit();
}

void EmulationUnit::prepareUnit()
{
	/* create emulation unit */
    m_emu = emu_new();
    m_cpu = emu_cpu_get(m_emu);
    m_mem = emu_memory_get(m_emu);

	/* create enviroment unit */
    m_env = emu_env_new(m_emu);
    m_env->profile = emu_profile_new();

	/* IAT for sqlslammer */
	prepareIATForSQLSlammer();

	/* export user hooks */
	exportWin32Hooks();
	exportLinuxHooks();
}

void EmulationUnit::destroyUnit()
{
    emu_env_free(m_env); // emu_profile destroyed inside emu_env_free
    emu_free(m_emu);
}

void EmulationUnit::resetUnit()
{
	destroyUnit();
	prepareUnit();
}

bool EmulationUnit::step()
{
	return true;
}

int32_t EmulationUnit::loadCode(byte_t *code, int32_t size)
{
	/* perform getPC test */
    m_codeOffset = getPcTest(code, size);
    LOG("m_codeOffset: [%d]\n", m_codeOffset);

	/* clear registers */
	for(int i = 0; i < 8; ++i)
        emu_cpu_reg32_set(m_cpu, (emu_reg32) i, 0);

	/* clear flags */
    emu_cpu_eflags_set(m_cpu, 0);

	/* FIXME: what is it? */
    emu_memory_write_dword(m_mem, 0xef787c3c, 4711);
    emu_memory_write_dword(m_mem, 0x00416f9a, 4711);
    emu_memory_write_dword(m_mem, 0x0044fcf7, 4711);
    emu_memory_write_dword(m_mem, 0x00001265, 4711);
    emu_memory_write_dword(m_mem, 0x00002583, 4711);
    emu_memory_write_dword(m_mem, 0x00e000de, 4711);
    emu_memory_write_dword(m_mem, 0x01001265, 4711);
    emu_memory_write_dword(m_mem, 0x8a000066, 4711);

	/* write the code to the offset */
    emu_memory_write_block(m_mem, STATIC_CODE_OFFSET, code, size);

	/* set eip to the code */
    emu_cpu_eip_set(m_cpu, STATIC_CODE_OFFSET + m_codeOffset);

	/* FIXME: what is it? */
    emu_memory_write_block(m_mem, 0x0012fe98, code, size);
    emu_cpu_reg32_set(m_cpu, esp, STATIC_CODE_OFFSET - 50); //0x0012fe98);
    emu_memory_write_dword(m_mem, 0x7df7b0bb, 0x00000000); //UrldownloadToFile

	LOG("SUCCESS\n\n");
    return m_codeOffset;
}

struct emu *EmulationUnit::emu()
{
    return m_emu;
}

struct emu_cpu *EmulationUnit::cpu()
{
    return m_cpu;
}

struct emu_memory *EmulationUnit::memory()
{
    return m_mem;
}

struct emu_env *EmulationUnit::env()
{
    return m_env;
}

int32_t EmulationUnit::getPcTest(byte_t *code, int32_t size)
{
    int32_t off = emu_shellcode_test(m_emu, code, size);
	resetUnit();

	return off;
}

void EmulationUnit::prepareIATForSQLSlammer()
{
    emu_memory_write_dword(m_mem, 0x42AE1018, 0x7c801D77);
    emu_memory_write_dword(m_mem, 0x42ae1010, 0x7c80ADA0);
    emu_memory_write_dword(m_mem, 0x7c80ADA0, 0x51EC8B55);
    emu_memory_write_byte(m_mem, 0x7c814eeb, 0xc3);
}

int32_t EmulationUnit::codeOffset()
{
    return m_codeOffset;
}

void EmulationUnit::exportWin32Hooks()
{
#if 0
    emu_env_w32_export_hook(m_env, "ExitProcess", userHook_ExitProcess, NULL);
    emu_env_w32_export_hook(m_env, "ExitThread", userHook_ExitThread, NULL);

	/* shdocvw.dll */
    emu_env_w32_load_dll(m_env->env.win,"shdocvw.dll");
    emu_env_w32_export_hook(m_env, "IEWinMain", userHook_IEWinMain, NULL);
    LOG("loaded: [shdocvw.dll]\n");

	/* msvcrt.dll */
    emu_env_w32_load_dll(m_env->env.win,"msvcrt.dll");
    emu_env_w32_export_hook(m_env, "fclose", userHook_fclose, na);
    emu_env_w32_export_hook(m_env, "fopen", userHook_fopen, na);
    emu_env_w32_export_hook(m_env, "fwrite", userHook_fwrite, na);

    emu_env_w32_export_hook(m_env, "CreateProcessA", userHook_CreateProcess, NULL);
    emu_env_w32_export_hook(m_env, "WaitForSingleObject", userHook_WaitForSingleObject, NULL);
    emu_env_w32_export_hook(m_env, "CreateFileA", userHook_CreateFile, na);
    emu_env_w32_export_hook(m_env, "WriteFile", userHook_WriteFile, na);
    emu_env_w32_export_hook(m_env, "CloseHandle", userHook_CloseHandle, na);
    LOG("loaded: [msvcrt.dll]\n");

	/* ws2_32.dll */
    emu_env_w32_load_dll(m_env->env.win,"ws2_32.dll");
    emu_env_w32_export_hook(m_env, "accept", userHook_accept, NULL);
    emu_env_w32_export_hook(m_env, "bind", userHook_bind, NULL);
    emu_env_w32_export_hook(m_env, "closesocket", userHook_closesocket, NULL);
    emu_env_w32_export_hook(m_env, "connect", userHook_connect, NULL);

    emu_env_w32_export_hook(m_env, "listen", userHook_listen, NULL);
    emu_env_w32_export_hook(m_env, "recv", userHook_recv, NULL);
    emu_env_w32_export_hook(m_env, "send", userHook_send, NULL);
    emu_env_w32_export_hook(m_env, "socket", userHook_socket, NULL);
    emu_env_w32_export_hook(m_env, "WSASocketA", userHook_WSASocket, NULL);
    LOG("loaded: [ws2_32.dll]\n");

	/* urlmon.dll */
    emu_env_w32_load_dll(m_env->env.win,"urlmon.dll");
    emu_env_w32_export_hook(m_env, "URLDownloadToFileA", userHook_URLDownloadToFile, NULL);
    LOG("loaded: [urlmon.dll]\n");
    LOG("Win32Hooks loaded\n");
	LOG("SUCCESS\n\n");
#endif
}

void EmulationUnit::exportLinuxHooks()
{
#if 0
    emu_env_linux_syscall_hook(m_env, "exit", userHook_exit, NULL);
    emu_env_linux_syscall_hook(m_env, "socket", userHook_socket, NULL);
    LOG("LinuxHooks loaded\n");
	LOG("SUCCESS\n\n");
#endif
}
