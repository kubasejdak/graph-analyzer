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

/* functions to define */
#define EXITPROCESS_HOOK		0
#define EXITTHREAD_HOOK			0
#define CREATEPROCESS_HOOK		0
#define WAITFOR_SIGNLEOBJECT_HOOK	0
#define EXIT_HOOK			0
#define ACCEPT_HOOK			0
#define BIND_HOOK			0
#define CLOSESOCKET_HOOK		0
#define CONNECT_HOOK			0
#define FCLOSE_HOOK			0
#define FOPEN_HOOK			0
#define FWRITE_HOOK			0
#define LISTEN_HOOK			0
#define RECV_HOOK			0
#define SEND_HOOK			0
#define SOCKET_HOOK			0
#define WSASOCKET_HOOK			0
#define CREATEFILE_HOOK			0
#define WRITEFILE_HOOK			0
#define CLOSEHANDLE_HOOK		0
#define URLDOWNLOADTOFILE_HOOK		0
#define IEWINMAIN_HOOK			0

#if EXIT_PROCESS_HOOK
uint32_t userHook_ExitProcess(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if EXIT_THREAD_HOOK
uint32_t userHook_ExitThread(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if CREATE_PROCESS_HOOK
uint32_t userHook_CreateProcess(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if WAIT_FOR_SIGNLE_OBJECT_HOOK
uint32_t userHook_WaitForSingleObject(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if EXIT_HOOK
uint32_t userHook_exit(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if ACCEPT_HOOK
uint32_t userHook_accept(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if BIND_HOOK
uint32_t userHook_bind(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if CLOSESOCKET_HOOK
uint32_t userHook_closesocket(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if CONNECT_HOOK
uint32_t userHook_connect(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if FCLOSE_HOOK
uint32_t userHook_fclose(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if FOPEN_HOOK
uint32_t userHook_fopen(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if FWRITE_HOOK
uint32_t userHook_fwrite(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if LISTEN_HOOK
uint32_t userHook_listen(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if RECV_HOOK
uint32_t userHook_recv(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if SEND_HOOK
uint32_t userHook_send(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if SOCKET_HOOK
uint32_t userHook_socket(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if WSASOCKET_HOOK
uint32_t userHook_WSASocket(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if CREATEFILE_HOOK
uint32_t userHook_CreateFile(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if WRITEFILE_HOOK
uint32_t userHook_WriteFile(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if CLOSEHANDLE_HOOK
uint32_t userHook_CloseHandle(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if URLDOWNLOADTOFILE_HOOK
uint32_t userHook_URLDownloadToFile(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif
#if IEWINMAIN_HOOK
uint32_t userHook_IEWinMain(struct emu_env *env, struct emu_env_hook *hook, ...);
#endif

/* utility */
void append(struct emu_string *to, const char *dir, char *data, int size);

#endif /* USERHOOKS_H_ */
