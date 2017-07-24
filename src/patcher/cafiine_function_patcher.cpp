#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "cafiine_function_patcher.h"
#include "utils/logger.h"
#include "cafiine/cafiine.h"
#include "retain_vars.h"

DECL(int, FSAAddClient, void *r3) {
	int res = real_FSAAddClient(r3);
    if(gAppStatus == 2) return res;

	if (res < MAX_CLIENT && res >= 0) {
		cafiine_connect((void*)0, res, 1);
	}

	return res;
}

DECL(int, FSADelClient, int client) {
    if(gAppStatus == 2) return real_FSADelClient(client);
	if (client < MAX_CLIENT && client >= 0) {
		cafiine_disconnect((void*)0, client, 1);
	}

	return real_FSADelClient(client);
}

DECL(int, FSAOpenFile, int client, const char *path, const char *mode, int *handle) {
    if(gAppStatus == 2) return real_FSAOpenFile(client, path, mode, handle);
	if (client < MAX_CLIENT && client >= 0) {
		int ret;
		if (cafiine_fopen((void*)0, client, &ret, path, mode, handle, 1) == 0)
			return ret;
	}

	return real_FSAOpenFile(client, path, mode, handle);
}


DECL(int, FSAddClientEx, void *r3, void *r4, void *r5) {
	int res = real_FSAddClientEx(r3, r4, r5);
    if(gAppStatus == 2) return res;

	if (res >= 0) {
		cafiine_connect(r3, 0, 0);
	}

	return res;
}

DECL(int, FSDelClient, void *pClient) {
    if(gAppStatus == 2) return real_FSDelClient(pClient);
    cafiine_disconnect(pClient, 0, 0);

	return real_FSDelClient(pClient);
}

DECL(int, FSOpenFile, void *pClient, void *pCmd, const char *path, const char *mode, int *handle, int error) {
    if(gAppStatus == 2) return real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
    int ret;
    if (cafiine_fopen(pClient, 0, &ret, path, mode, handle, 0) == 0) {
        return ret;
    }

	return real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
}

DECL(int, FSReadFile, void *pClient, void *pCmd, void *buffer, int size, int count, int fd, int flag, int error) {
    if(gAppStatus == 2) return real_FSReadFile(pClient, pCmd, buffer, size, count, fd, flag, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_fread(pClient, &ret, buffer, size, count, fd) == 0) {
			return ret;
		}
	}

	return real_FSReadFile(pClient, pCmd, buffer, size, count, fd, flag, error);
}

DECL(int, FSReadFileWithPos, void *pClient, void *pCmd, void *buffer, int size, int count, int pos, int fd, int flag, int error) {
    if(gAppStatus == 2) return real_FSReadFileWithPos(pClient, pCmd, buffer, size, count, pos, fd, flag, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_fsetpos(pClient, &ret, fd, pos) == 0) {
			if (cafiine_fread(pClient, &ret, buffer, size, count, fd) == 0) {
				return ret;
			}
		}
	}

	return real_FSReadFileWithPos(pClient, pCmd, buffer, size, count, pos, fd, flag, error);
}

DECL(int, FSCloseFile, void *pClient, void *pCmd, int fd, int error) {
    if(gAppStatus == 2) return real_FSCloseFile(pClient, pCmd, fd, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_fclose(pClient, &ret, fd) == 0) {
			return ret;
		}
	}

	return real_FSCloseFile(pClient, pCmd, fd, error);
}

DECL(int, FSSetPosFile, void *pClient, void *pCmd, int fd, int pos, int error) {
    if(gAppStatus == 2) return real_FSSetPosFile(pClient, pCmd, fd, pos, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_fsetpos(pClient, &ret, fd, pos) == 0) {
			return ret;
		}

	}

	return real_FSSetPosFile(pClient, pCmd, fd, pos, error);
}

DECL(int, FSGetPosFile, void *pClient, void *pCmd, int fd, int *pos, int error) {
    if(gAppStatus == 2) return real_FSGetPosFile(pClient, pCmd, fd, pos, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_fgetpos(pClient, &ret, fd, pos) == 0) {
			return ret;
		}
	}

	return real_FSGetPosFile(pClient, pCmd, fd, pos, error);
}

DECL(int, FSGetStatFile, void *pClient, void *pCmd, int fd, void *buffer, int error) {
    if(gAppStatus == 2) return real_FSGetStatFile(pClient, pCmd, fd, buffer, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_fstat(pClient, &ret, fd, buffer) == 0) {
			return ret;
		}
	}

	return real_FSGetStatFile(pClient, pCmd, fd, buffer, error);
}

DECL(int, FSIsEof, void *pClient, void *pCmd, int fd, int error) {
    if(gAppStatus == 2) return real_FSIsEof(pClient, pCmd, fd, error);
	if ((fd & MASK_FD) == MASK_FD) {
		int ret;
		if (cafiine_feof(pClient, &ret, fd) == 0) {
			return ret;
		}
	}

	return real_FSIsEof(pClient, pCmd, fd, error);
}

hooks_magic_t method_hooks_cafiine[] __attribute__((section(".data"))) = {
	MAKE_MAGIC(FSAAddClient, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSADelClient, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSAOpenFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSAddClientEx, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSDelClient, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSOpenFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSCloseFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSReadFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSReadFileWithPos, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSSetPosFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSGetStatFile, LIB_FS, STATIC_FUNCTION),
	MAKE_MAGIC(FSIsEof, LIB_FS, STATIC_FUNCTION),
};
u32 method_hooks_size_cafiine __attribute__((section(".data"))) = sizeof(method_hooks_cafiine) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_cafiine[sizeof(method_hooks_cafiine) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));
