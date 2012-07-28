/*
 *  Filename	: UserHooks.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "UserHooks.h"

#if EXITPROCESS_HOOK
uint32_t userHook_ExitProcess(struct emu_env *env, struct emu_env_hook *hook, ...) {
/*
VOID WINAPI ExitProcess(
  UINT uExitCode
);
*/
	va_list vl;
	va_start(vl, hook);
	int exitcode = va_arg(vl,  int);
	va_end(vl);

	printf("%s(%i)\n", hook->hook.win->fnname, exitcode);

	return 0;
}
#endif

#if EXITTHREAD_HOOK
uint32_t userHook_ExitThread(struct emu_env *env, struct emu_env_hook *hook, ...) {
/*
VOID ExitThread(
  DWORD dwExitCode
);
*/
	va_list vl;
	va_start(vl, hook);
	int exitcode = va_arg(vl,  int);
	va_end(vl);

	printf("%s(%i)\n", hook->hook.win->fnname, exitcode);

	return 0;

}
#endif

#if CREATEPROCESS_HOOK
uint32_t userHook_CreateProcess(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	/* char *pszImageName				  = */ (void)va_arg(vl, char *);
	char *pszCmdLine                      = va_arg(vl, char *);
	/* void *psaProcess, 				  = */ (void)va_arg(vl, void *);
	/* void *psaThread,  				  = */ (void)va_arg(vl, void *);
	/* bool fInheritHandles,              = */ (void)va_arg(vl, char *);
	/* uint32_t fdwCreate,                = */ (void)va_arg(vl, uint32_t);
	/* void *pvEnvironment             	  = */ (void)va_arg(vl, void *);
	/* char *pszCurDir                 	  = */ (void)va_arg(vl, char *);
	STARTUPINFO *psiStartInfo             = va_arg(vl, STARTUPINFO *);
	PROCESS_INFORMATION *pProcInfo        = va_arg(vl, PROCESS_INFORMATION *);

	va_end(vl);
	printf("CreateProcess(%s)\n",pszCmdLine);

	if ( pszCmdLine != NULL && strncasecmp(pszCmdLine, "cmd", 3) == 0 )
	{
		pid_t child;
		pid_t spy;



		if ( (spy = fork()) == 0 )
		{ // spy

			int in[2];
			int out[2];
			int err[2];

			socketpair( AF_UNIX, SOCK_STREAM, 0, in );
			socketpair( AF_UNIX, SOCK_STREAM, 0, out );
			socketpair( AF_UNIX, SOCK_STREAM, 0, err );

			if ( (child=fork()) == 0 )
			{ // child

				close(in[0]);
				close(out[1]);
				close(err[1]);

				dup2(in[1], fileno(stdin));
				dup2(out[0], fileno(stdout));
				dup2(err[0], fileno(stderr));

				int sys = -1;
				struct emu_hashtable_item *ehi = emu_hashtable_search(opts.override.commands.commands, "cmd");
				if ( ehi != NULL )
					sys = system((char *)ehi->value);
				else
					sys = system("/bin/sh -c \"cd ~/.wine/drive_c/; wine 'c:\\windows\\system32\\cmd_orig.exe' \"");

				close(in[1]);
				close(out[0]);
				close(err[0]);

				printf("process ended (%i)!\n", sys);
				exit(EXIT_SUCCESS);
			} else
			{
				struct emu_string *io = emu_string_new();
				close(in[1]);
				close(out[0]);
				close(err[0]);
				fd_set socks;

				fcntl(psiStartInfo->hStdInput,F_SETFL,O_NONBLOCK);
				fcntl(out[1],F_SETFL,O_NONBLOCK);
				fcntl(err[1],F_SETFL,O_NONBLOCK);

				char buf[1048];

				while ( 1 )
				{
					FD_ZERO(&socks);
					FD_SET(psiStartInfo->hStdInput,&socks);
					FD_SET(out[1],&socks);
					FD_SET(err[1],&socks);

					int highsock = MAX(psiStartInfo->hStdInput, MAX(out[1], err[1]));


					struct timeval timeout = { 10, 0};

					int action = select(highsock+1, &socks, NULL, NULL, &timeout);
					int written = -1;
					if ( action > 0 )
					{
						if ( FD_ISSET(psiStartInfo->hStdInput, &socks) )
						{
							int size = read(psiStartInfo->hStdInput, buf, 1024);

							if ( size > 0 )
								written = write(in[0], buf, size);
							else
								goto exit_now;
							append(io, "in  >", buf, size);
						}
						if ( FD_ISSET(out[1], &socks) )
						{
							int size = read(out[1], buf, 1024);
							if ( size > 0 )
								written = write(psiStartInfo->hStdOutput, buf, size);
							else
								goto exit_now;
							append(io, "out <", buf, size);
						}
						if ( FD_ISSET(err[1], &socks) )
						{
							int size = read(err[1], buf, 1024);
//							printf("read %i err '%.*s'\n",size,size,buf);
							if ( size > 0 )
								written = write(psiStartInfo->hStdOutput, buf, size);
							else
								goto exit_now;
							append(io, "err <", buf, size);
						}
						if( written == -1 )
							goto exit_now;

					} else
					{
						printf("timeout, killing cmd prompt\n");
						kill(child, SIGKILL);

exit_now:
						printf("spy dies\n");
						printf("session was\n%s\n", emu_string_char(io));
						emu_string_free(io);
						close(in[0]);
						close(out[1]);
						close(err[1]);
						exit(EXIT_SUCCESS);
					}

				}
			} // spy


		} else
		{ // parent
			pProcInfo->hProcess = spy;
		}
	}



	return 1;
}
#endif

#if WAITFOR_SIGNLEOBJECT_HOOK
uint32_t userHook_WaitForSingleObject(struct emu_env *env, struct emu_env_hook *hook, ...) {
	/*
	DWORD WINAPI WaitForSingleObject(
	  HANDLE hHandle,
	  DWORD dwMilliseconds
	);
	*/

	va_list vl;
	va_start(vl, hook);

	int32_t hHandle = va_arg(vl, int32_t);
	/*int32_t dwMilliseconds = */ (void)va_arg(vl, int32_t);
	va_end(vl);

	int status;
	while(1)
	{
		if (waitpid(hHandle, &status, WNOHANG) != 0)
			break;
		sleep(1);
	}

	printf("process exited with status %i\n",status);
	return 0;
}
#endif

#if EXIT_HOOK
uint32_t userHook_exit(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);
	int code = va_arg(vl,  int);
	va_end(vl);

	printf("exit(%i)\n",code);
	return 0;
}
#endif

#if ACCEPT_HOOK
uint32_t userHook_accept(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int s 					= va_arg(vl,  int);
	/*struct sockaddr* addr 	= */(void)va_arg(vl,  struct sockaddr *);
	/*socklen_t* addrlen 		= */(void)va_arg(vl,  socklen_t *);
	va_end(vl);

	struct sockaddr sa;
	socklen_t st = sizeof(struct sockaddr);

	return accept(s, &sa, &st);
}
#endif

#if BIND_HOOK
uint32_t userHook_bind(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int s 					= va_arg(vl,  int);
	struct sockaddr* addr 	= va_arg(vl,  struct sockaddr *);
	socklen_t addrlen = va_arg(vl,  socklen_t );

	if (opts.override.bind.host != NULL )
	{
		struct sockaddr_in *si = (struct sockaddr_in *)addr;
		si->sin_addr.s_addr = inet_addr(opts.override.bind.host);
	}

	if (opts.override.bind.port > 0)
	{
		struct sockaddr_in *si = (struct sockaddr_in *)addr;;
		si->sin_port = htons(opts.override.bind.port);
	}

	va_end(vl);

    return bind(s, addr, addrlen);
}
#endif

#if CLOSESOCKET_HOOK
uint32_t userHook_closesocket(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);
	int s 					= va_arg(vl,  int);
	va_end(vl);

    return close(s);
}
#endif

#if CONNECT_HOOK
uint32_t userHook_connect(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int s 					= va_arg(vl,  int);
	struct sockaddr* addr 	= va_arg(vl,  struct sockaddr *);

	if (opts.override.connect.host != NULL )
	{
		struct sockaddr_in *si = (struct sockaddr_in *)addr;
		si->sin_addr.s_addr = inet_addr(opts.override.connect.host);
	}

	if (opts.override.connect.port > 0)
	{
		struct sockaddr_in *si = (struct sockaddr_in *)addr;;
		si->sin_port = htons(opts.override.connect.port);
	}


	socklen_t addrlen = va_arg(vl,  socklen_t);

	if (addrlen != sizeof(struct sockaddr))
	{
		addrlen = sizeof(struct sockaddr);
	}

	va_end(vl);

    return connect(s, addr, addrlen);
}
#endif

#if FCLOSE_HOOK
uint32_t userHook_fclose(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);
	FILE *f = va_arg(vl, FILE *);
	va_end(vl);

	struct nanny_file *nf = nanny_get_file(hook->hook.win->userdata, (uint32_t)(uintptr_t)f);

	if (nf != NULL)
	{
		FILE *ef = nf->real_file;
		nanny_del_file(hook->hook.win->userdata, (uint32_t)(uintptr_t)f);
    	return fclose(ef);
	}
	else
		return 0;

}
#endif

#if FOPEN_HOOK
uint32_t userHook_fopen(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	char *filename			= va_arg(vl,  char *);
	/*char *mode 				= */(void)va_arg(vl,  char *);
	va_end(vl);


	char *localfile;

	if ( asprintf(&localfile, "/tmp/%s-XXXXXX",filename) == -1)
		exit(-1);

	int fd = mkstemp(localfile);
	close(fd);

	FILE *f = fopen(localfile,"w");

	uint32_t file;
	nanny_add_file(hook->hook.win->userdata, localfile, &file, f);

	return file;
}
#endif

#if FWRITE_HOOK
/* size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream); */
uint32_t userHook_fwrite(struct emu_env *env, struct emu_env_hook *hook, ...)
{
	va_list vl;
	va_start(vl, hook);
	void *data = va_arg(vl, void *);
	size_t size = va_arg(vl, size_t);
	size_t nmemb = va_arg(vl, size_t);
	FILE *f = va_arg(vl, FILE *);
	va_end(vl);

	struct nanny_file *nf = nanny_get_file(hook->hook.win->userdata, (uint32_t)(uintptr_t)f);

	if (nf != NULL)
		return fwrite(data, size, nmemb, nf->real_file);
	else
		return size*nmemb;

}
#endif

#if LISTEN_HOOK
uint32_t userHook_listen(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int s 					= va_arg(vl,  int);
	int backlog			 	= va_arg(vl,  int);
	va_end(vl);

	return listen(s, backlog);
}
#endif

#if RECV_HOOK
uint32_t userHook_recv(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int s = va_arg(vl,  int);
	char* buf = va_arg(vl,  char *);
	int len = va_arg(vl,  int);
	int flags = va_arg(vl,  int);
	va_end(vl);

	return recv(s, buf, len,  flags);
}
#endif

#if SEND_HOOK
uint32_t userHook_send(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int s = va_arg(vl,  int);
	char* buf = va_arg(vl,  char *);
	int len = va_arg(vl,  int);
	int flags = va_arg(vl,  int);
	va_end(vl);

	return send(s, buf, len,  flags);
}
#endif

#if SOCKET_HOOK
/* int socket(int domain, int type, int protocol); */
uint32_t userHook_socket(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int domain = va_arg(vl,  int);
	int type = va_arg(vl,  int);
	int protocol = va_arg(vl, int);
	va_end(vl);

	printf("socket(%i, %i, %i)\n",domain, type, protocol);

	return socket(domain, type, protocol);
}
#endif

#if WSASOCKET_HOOK
/* int socket(int domain, int type, int protocol); */
uint32_t userHook_WSASocket(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	int domain = va_arg(vl,  int);
	int type = va_arg(vl,  int);
	int protocol = va_arg(vl, int);
	(void)va_arg(vl, int);
	(void)va_arg(vl, int);
	(void)va_arg(vl, int);

	va_end(vl);

	printf("WSASocket(%i, %i, %i)\n",domain, type, protocol);

	return socket(domain, type, protocol);
}
#endif

#if CREATEFILE_HOOK
uint32_t userHook_CreateFile(struct emu_env *env, struct emu_env_hook *hook, ...) {
/*
HANDLE CreateFile(
  LPCTSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  HANDLE hTemplateFile
);
*/

	va_list vl;
	va_start(vl, hook);
	char *lpFileName			= va_arg(vl, char *);
	/*int dwDesiredAccess		=*/(void)va_arg(vl, int);
	/*int dwShareMode			=*/(void)va_arg(vl, int);
	/*int lpSecurityAttributes	=*/(void)va_arg(vl, int);
	/*int dwCreationDisposition	=*/(void)va_arg(vl, int);
	/*int dwFlagsAndAttributes	=*/(void)va_arg(vl, int);
	/*int hTemplateFile			=*/(void)va_arg(vl, int);
	va_end(vl);

	char *localfile;

	if ( asprintf(&localfile, "/tmp/%s-XXXXXX",lpFileName) == -1)
		exit(-1);

	int fd = mkstemp(localfile);
	close(fd);

	FILE *f = fopen(localfile,"w");

	uint32_t handle;
	nanny_add_file(hook->hook.win->userdata, localfile, &handle, f);

	return (uint32_t)handle;
}
#endif

#if WRITEFILE_HOOK
uint32_t userHook_WriteFile(struct emu_env *env, struct emu_env_hook *hook, ...) {
/*
BOOL WriteFile(
  HANDLE hFile,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  LPOVERLAPPED lpOverlapped
);
*/

	va_list vl;
	va_start(vl, hook);
	FILE *hFile 					= va_arg(vl, FILE *);
	void *lpBuffer 					= va_arg(vl, void *);
	int   nNumberOfBytesToWrite 	= va_arg(vl, int);
	/* int *lpNumberOfBytesWritten  =*/(void)va_arg(vl, int*);
	/* int *lpOverlapped 		    =*/(void)va_arg(vl, int*);
	va_end(vl);

	struct nanny_file *nf = nanny_get_file(hook->hook.win->userdata, (uint32_t)(uintptr_t)hFile);

	if (nf != NULL)
	{
		if( fwrite(lpBuffer, nNumberOfBytesToWrite, 1, nf->real_file) != 1 )
			return 0;
	}
	else
		printf("shellcode tried to write data to not existing handle\n");

	return 1;

}
#endif

#if CLOSEHANDLE_HOOK
uint32_t userHook_CloseHandle(struct emu_env *env, struct emu_env_hook *hook, ...) {
/*
BOOL CloseHandle(
  HANDLE hObject
);
*/

	va_list vl;
	va_start(vl, hook);
	FILE *hObject = va_arg(vl, FILE *);
	va_end(vl);

	struct nanny_file *nf = nanny_get_file(hook->hook.win->userdata, (uint32_t)(uintptr_t)hObject);

	if (nf != NULL)
	{
		FILE *f = nf->real_file;
		nanny_del_file(hook->hook.win->userdata, (uint32_t)(uintptr_t)hObject);
		fclose(f);
	}
	else
	{
		printf("shellcode tried to close not existing handle (maybe closed it already?)\n");
	}


	return 0;
}
#endif

#if URLDOWNLOADTOFILE_HOOK
uint32_t userHook_URLDownloadToFile(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);

	/*void * pCaller    = */(void)va_arg(vl, void *);
	char * szURL      = va_arg(vl, char *);
	char * szFileName = va_arg(vl, char *);
	/*int    dwReserved = */(void)va_arg(vl, int   );
	/*void * lpfnCB     = */(void)va_arg(vl, void *);

	va_end(vl);


	printf("download %s -> %s\n", szURL, szFileName);

	return 0;
}
#endif

#if IEWINMAIN_HOOK
uint32_t userHook_IEWinMain(struct emu_env *env, struct emu_env_hook *hook, ...) {
	va_list vl;
	va_start(vl, hook);
	/*char *CommandLine =*/ (void)va_arg(vl, char *);
	/*int nShowWindow   =*/ (void)va_arg(vl, int);
	va_end(vl);

	opts.steps = 0;
	return 0;
}
#endif

/* utility */
void append(struct emu_string *to, const char *dir, char *data, int size)
{
	char *saveptr = data;
	struct emu_string *sanestr = emu_string_new();

	int i;
	for (i=0;i<size;i++) {
		if (data[i] == '\r') {
		}

		else if(isprint(data[i]))
			emu_string_append_format(sanestr, "%c", data[i]);

		else if (data[i] == '\n')
			emu_string_append_char(sanestr, "\n");

		else if (data[i] == '\t')
			emu_string_append_char(sanestr, "\t");

		else
			emu_string_append_format(sanestr, "\\x%02x", (unsigned char)data[i]);
	}

	saveptr = NULL;

	char *tok;
	tok  = strtok_r(sanestr->data, "\n", &saveptr);
	if(tok != NULL) {
		emu_string_append_format(to, "%s %s\n", dir, tok);
		while ( (tok = strtok_r(NULL,"\n",&saveptr)) != NULL )
			emu_string_append_format(to, "%s %s\n", dir, tok);
	}
	emu_string_free(sanestr);
}
