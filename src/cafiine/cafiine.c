#include <malloc.h>
#include <string.h>
#include "cafiine.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "kernel/kernel_functions.h"
#include "common/common.h"
#include "utils/logger.h"

static int recvwait(int sock, void *buffer, int len) {
	int ret;
	while (len > 0) {
		ret = recv(sock, buffer, len, 0);
		CHECK_ERROR(ret < 0);
		len -= ret;
		buffer += ret;
	}
	return 0;
error:
	return ret;
}

static int recvbyte(int sock) {
	unsigned char buffer[1];
	int ret;
	
	ret = recvwait(sock, buffer, 1);
	if (ret < 0) return ret;
	return buffer[0];
}

static int sendwait(int sock, const void *buffer, int len) {
	int ret;
	while (len > 0) {
		ret = send(sock, buffer, len, 0);
		CHECK_ERROR(ret < 0);
		len -= ret;
		buffer += ret;
	}
	return 0;
error:
	return ret;
}

static int sendbyte(int sock, unsigned char byte) {
	unsigned char buffer[1];
	
	buffer[0] = byte;
	return sendwait(sock, buffer, 1);
}

static int client_num_alloc(void *pClient) {
	int i;
	
	for (i = 0; i < MAX_CLIENT; i++)
		if (bss.pClient_fs[i] == 0) {
			bss.pClient_fs[i] = pClient;
			return i;
		}
	return -1;
}
static void client_num_free(int client) {
	bss.pClient_fs[client] = 0;
}

static int client_num(void *pClient) {
	int i;
	
	for (i = 0; i < MAX_CLIENT; i++)
		if (bss.pClient_fs[i] == pClient)
			return i;
	
	return -1;
}

void memset_bss() {
	// allocate memory for our stuff
	void *mem_ptr = memalign(0x40, sizeof(struct bss_t));
	if(!mem_ptr) {
		return;
	}
	
	// copy pointer
	bss_ptr = mem_ptr;
	memset(bss_ptr, 0, sizeof(struct bss_t));
}

void cafiine_connect(void *pClient, int clientId, int isFSA) {
	struct sockaddr_in addr;
	int sock, ret;
	
	int client = (isFSA == 1) ? clientId : client_num_alloc(pClient);
	
	socket_lib_init();
	
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	CHECK_ERROR(sock == -1);
	
	addr.sin_family = AF_INET;
	addr.sin_port = 7332;
	addr.sin_addr.s_addr = ((192<<24) | (168<<16) | (2<<8) | (15<<0));
	
	ret = connect(sock, (void *)&addr, sizeof(addr));
	CHECK_ERROR(ret < 0);
	ret = cafiine_handshake(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	
	if (isFSA == 1)
		bss.socket_fsa[client] = sock;
	else
		bss.socket_fs[client] = sock;
	
	
	return;
error:
	if (sock != -1)
		socketclose(sock);
	bss.socket_fs[client] = -1;
}


void cafiine_disconnect(void *pClient, int clientId, int isFSA) {
	int sock;
	if (isFSA == 1)
		sock = bss.socket_fsa[clientId];
	else
		sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	socketclose(sock);
	
	client_num_free(clientId);
error:
	return;
}

/*void cafiine_disconnect(int clientId) {
	int sock = bss.socket_fs[clientId];
	
	CHECK_ERROR(sock == -1);
	socketclose(sock);
	
	client_num_free(clientId);
error:
	return;
}*/

int cafiine_handshake(int sock) {
	int ret;
	unsigned char buffer[16];
	
	void* title_id = (void*)0x0;
	if (OS_FIRMWARE == 550) {
		title_id = (void*)0x10013C10;
	} else if (OS_FIRMWARE < 550 && OS_FIRMWARE >= 532) {
		title_id = (void*)0x100136D0;
	} else if (OS_FIRMWARE < 532 && OS_FIRMWARE >= 500) {
		title_id = (void*)0x10013010;
	} else if (OS_FIRMWARE == 410) {
		title_id = (void*)0x1000ECB0;
	} else {
		OSFatal("Sorry, this firmware version is unsupported.");
	}
	
	memcpy(buffer, title_id, 16);
	
	ret = sendwait(sock, buffer, sizeof(buffer));
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	return ret;
error:
	return ret;
}

int cafiine_fopen(void* pClient, int clientId, int *result, const char *path, const char *mode, int *handle, int isFSA) {
	while (bss.lock) GX2WaitForVsync();
	
	bss.lock = 1;
	
	int sock;
	if (isFSA == 1)
		sock = bss.socket_fsa[clientId];
	else
		sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	ret = sendbyte(sock, BYTE_OPEN);
	CHECK_ERROR(ret < 0);
	
	int len_path = 0;
	while (path[len_path++]);
	int len_mode = 0;
	while (mode[len_mode++]);
	{
		char buffer[8 + len_path + len_mode];
		*(int *)(buffer) = len_path;
		*(int *)(buffer + 4) = len_mode;
		for (ret = 0; ret < len_path; ret++)
			buffer[8 + ret] = path[ret];
		for (ret = 0; ret < len_mode; ret++)
			buffer[8 + len_path + ret] = mode[ret];
		
		ret = sendwait(sock, buffer, 8 + len_path + len_mode);
	}
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	ret = recvwait(sock, handle, 4);
	CHECK_ERROR(ret < 0);
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}

int cafiine_fread(void* pClient, int *result, void *ptr, int size, int count, int fd) {
	while (bss.lock) GX2WaitForVsync();
	bss.lock = 1;
	
	int sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	char buffer[1 + 12];
	buffer[0] = BYTE_READ;
	*(int *)(buffer + 1) = size;
	*(int *)(buffer + 5) = count;
	*(int *)(buffer + 9) = fd;
	ret = sendwait(sock, buffer, 1 + 12);
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	int sz;
	ret = recvwait(sock, &sz, 4);
	CHECK_ERROR(ret < 0);
	ret = recvwait(sock, ptr, sz);
	CHECK_ERROR(ret < 0);
	ret = sendbyte(sock, BYTE_OK);
	CHECK_ERROR(ret < 0);
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}

int cafiine_fclose(void* pClient, int *result, int fd) {
	while (bss.lock) GX2WaitForVsync();
	bss.lock = 1;
	
	int sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	char buffer[1 + 4];
	buffer[0] = BYTE_CLOSE;
	*(int *)(buffer + 1) = fd;
	ret = sendwait(sock, buffer, 1 + 4);
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}

int cafiine_fsetpos(void *pClient, int *result, int fd, int set) {
	while (bss.lock) GX2WaitForVsync();
	bss.lock = 1;
	
	int sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	char buffer[1 + 8];
	buffer[0] = BYTE_SETPOS;
	*(int *)(buffer + 1) = fd;
	*(int *)(buffer + 5) = set;
	ret = sendwait(sock, buffer, 1 + 8);
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}

int cafiine_fgetpos(void* pClient, int *result, int fd, int *pos) {
	while (bss.lock) GX2WaitForVsync();
	bss.lock = 1;
	
	int sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	char buffer[1 + 4];
	buffer[0] = BYTE_GETPOS;
	*(int *)(buffer + 1) = fd;
	ret = sendwait(sock, buffer, 1 + 4);
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	ret = recvwait(sock, pos, 4);
	CHECK_ERROR(ret < 0);
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}

int cafiine_fstat(void* pClient, int *result, int fd, void *ptr) {
	while (bss.lock) GX2WaitForVsync();
	bss.lock = 1;
	
	int sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	char buffer[1 + 4];
	buffer[0] = BYTE_STATFILE;
	*(int *)(buffer + 1) = fd;
	ret = sendwait(sock, buffer, 1 + 4);
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	int sz;
	ret = recvwait(sock, &sz, 4);
	CHECK_ERROR(ret < 0);
	if (ptr) {
		ret = recvwait(sock, ptr, sz);
		CHECK_ERROR(ret < 0);
	}
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}

int cafiine_feof(void* pClient, int *result, int fd) {
	while (bss.lock) GX2WaitForVsync();
	bss.lock = 1;
	
	int sock = bss.socket_fs[client_num(pClient)];
	
	CHECK_ERROR(sock == -1);
	
	int ret;
	char buffer[1 + 4];
	buffer[0] = BYTE_EOF;
	*(int *)(buffer + 1) = fd;
	ret = sendwait(sock, buffer, 1 + 4);
	CHECK_ERROR(ret < 0);
	ret = recvbyte(sock);
	CHECK_ERROR(ret < 0);
	CHECK_ERROR(ret == BYTE_NORMAL);
	ret = recvwait(sock, result, 4);
	CHECK_ERROR(ret < 0);
	
	bss.lock = 0;
	return 0;
error:
	bss.lock = 0;
	return -1;
}
