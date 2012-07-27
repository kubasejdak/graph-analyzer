/*
 *  Filename	: UserHooks.h
 *  Author	: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#ifndef USERHOOKS_H_
#define USERHOOKS_H_

/* standard headers */
#include <cstdarg>
#include <stdint.h>
#include <cstdio>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <ctype.h>
#include <signal.h>

uint32_t userHook_ExitProcess(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_ExitThread(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_CreateProcess(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_WaitForSingleObject(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_exit(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_accept(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_bind(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_closesocket(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_connect(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_fclose(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_fopen(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_fwrite(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_listen(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_recv(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_send(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_socket(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_WSASocket(struct emu_env *env, struct emu_env_hook *hook, ...);

uint32_t userHook_CreateFile(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_WriteFile(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_CloseHandle(struct emu_env *env, struct emu_env_hook *hook, ...);

uint32_t userHook_URLDownloadToFile(struct emu_env *env, struct emu_env_hook *hook, ...);
uint32_t userHook_IEWinMain(struct emu_env *env, struct emu_env_hook *hook, ...);

#endif /* USERHOOKS_H_ */
