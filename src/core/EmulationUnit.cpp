/*
 *  Filename	: EmulationUnit.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 12-05-2012
 */

#include "EmulationUnit.h"

EmulationUnit::EmulationUnit() {
	prepareUnit();
}

EmulationUnit::~EmulationUnit() {
	destroyUnit();
}

void EmulationUnit::prepareUnit() {
	/* create emulation unit */
	emu = emu_new();
	cpu = emu_cpu_get(emu);
	mem = emu_memory_get(emu);

	/* create enviroment unit */
	env = emu_env_new(emu);
	env->profile = emu_profile_new();

	/* IAT for sqlslammer */
	prepareIATForSQLSlammer();

	/* export user hooks */
	exportWin32Hooks();
	exportLinuxHooks();
}

void EmulationUnit::destroyUnit() {
	emu_env_free(env);		// emu_profile destroyed inside emu_env_free
	emu_free(emu);
}

void EmulationUnit::resetUnit() {
	destroyUnit();
	prepareUnit();
}

bool EmulationUnit::step() {
	return true;
}

int32_t EmulationUnit::loadCode(byte_t *code, int32_t size) {
	/* perform getPC test */
		codeOffset = getPcTest(code, size);

	/* clear registers */
	for(int i = 0; i < 8; ++i)
		emu_cpu_reg32_set(cpu, (emu_reg32) i, 0);

	/* clear flags */
	emu_cpu_eflags_set(cpu, 0);

	/* FIXME: what is it? */
	emu_memory_write_dword(mem, 0xef787c3c,  4711);
	emu_memory_write_dword(mem, 0x00416f9a,  4711);
	emu_memory_write_dword(mem, 0x0044fcf7, 4711);
	emu_memory_write_dword(mem, 0x00001265, 4711);
	emu_memory_write_dword(mem, 0x00002583, 4711);
	emu_memory_write_dword(mem, 0x00e000de, 4711);
	emu_memory_write_dword(mem, 0x01001265, 4711);
	emu_memory_write_dword(mem, 0x8a000066, 4711);

	/* write the code to the offset */
	emu_memory_write_block(mem, STATIC_CODE_OFFSET, code, size);

	/* set eip to the code */
	emu_cpu_eip_set(cpu, STATIC_CODE_OFFSET + codeOffset);

	/* FIXME: what is it? */
	emu_memory_write_block(mem, 0x0012fe98, code, size);
	emu_cpu_reg32_set(cpu, esp, STATIC_CODE_OFFSET - 50); //0x0012fe98);
	emu_memory_write_dword(mem, 0x7df7b0bb, 0x00000000); //UrldownloadToFile

	return codeOffset;
}

struct emu *EmulationUnit::getEmu() {
	return emu;
}

struct emu_cpu *EmulationUnit::getCpu() {
	return emu_cpu_get(emu);
}

struct emu_memory *EmulationUnit::getMemory() {
	return emu_memory_get(emu);
}

struct emu_env *EmulationUnit::getEnv() {
	return env;
}

int32_t EmulationUnit::getPcTest(byte_t *code, int32_t size) {
	int32_t off = emu_shellcode_test(emu, code, size);
	resetUnit();

	return off;
}

void EmulationUnit::prepareIATForSQLSlammer() {
	emu_memory_write_dword(mem, 0x42AE1018, 0x7c801D77);
	emu_memory_write_dword(mem, 0x42ae1010, 0x7c80ADA0);
	emu_memory_write_dword(mem, 0x7c80ADA0, 0x51EC8B55);
	emu_memory_write_byte(mem,  0x7c814eeb, 0xc3);
}

int32_t EmulationUnit::getCodeOffset() {
	return codeOffset;
}

void EmulationUnit::exportWin32Hooks() {
#if 0
	emu_env_w32_export_hook(env, "ExitProcess", userHook_ExitProcess, NULL);
	emu_env_w32_export_hook(env, "ExitThread", userHook_ExitThread, NULL);

	/* shdocvw.dll */
	emu_env_w32_load_dll(env->env.win,"shdocvw.dll");
	emu_env_w32_export_hook(env, "IEWinMain", userHook_IEWinMain, NULL);

	/* msvcrt.dll */
	emu_env_w32_load_dll(env->env.win,"msvcrt.dll");
	emu_env_w32_export_hook(env, "fclose", userHook_fclose, na);
	emu_env_w32_export_hook(env, "fopen", userHook_fopen, na);
	emu_env_w32_export_hook(env, "fwrite", userHook_fwrite, na);

	emu_env_w32_export_hook(env, "CreateProcessA", userHook_CreateProcess, NULL);
	emu_env_w32_export_hook(env, "WaitForSingleObject", userHook_WaitForSingleObject, NULL);
	emu_env_w32_export_hook(env, "CreateFileA", userHook_CreateFile, na);
	emu_env_w32_export_hook(env, "WriteFile", userHook_WriteFile, na);
	emu_env_w32_export_hook(env, "CloseHandle", userHook_CloseHandle, na);

	/* ws2_32.dll */
	emu_env_w32_load_dll(env->env.win,"ws2_32.dll");
	emu_env_w32_export_hook(env, "accept", userHook_accept, NULL);
	emu_env_w32_export_hook(env, "bind", userHook_bind, NULL);
	emu_env_w32_export_hook(env, "closesocket", userHook_closesocket, NULL);
	emu_env_w32_export_hook(env, "connect", userHook_connect, NULL);

	emu_env_w32_export_hook(env, "listen", userHook_listen, NULL);
	emu_env_w32_export_hook(env, "recv", userHook_recv, NULL);
	emu_env_w32_export_hook(env, "send", userHook_send, NULL);
	emu_env_w32_export_hook(env, "socket", userHook_socket, NULL);
	emu_env_w32_export_hook(env, "WSASocketA", userHook_WSASocket, NULL);

	/* urlmon.dll */
	emu_env_w32_load_dll(env->env.win,"urlmon.dll");
	emu_env_w32_export_hook(env, "URLDownloadToFileA", userHook_URLDownloadToFile, NULL);
#endif
}

void EmulationUnit::exportLinuxHooks() {
#if 0
	emu_env_linux_syscall_hook(env, "exit", userHook_exit, NULL);
	emu_env_linux_syscall_hook(env, "socket", userHook_socket, NULL);
#endif
}
