#ifndef _CAFIINE_H_
#define _CAFIINE_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#define MAX_CLIENT 32
	
#define CHECK_ERROR(cond) if (cond) { goto error; }
	
#define BYTE_NORMAL 0xff
#define BYTE_SPECIAL 0xfe
#define BYTE_OPEN 0x00
#define BYTE_READ 0x01
#define BYTE_CLOSE 0x02
#define BYTE_OK 0x03
#define BYTE_SETPOS 0x04
#define BYTE_STATFILE 0x05
#define BYTE_EOF 0x06
#define BYTE_GETPOS 0x07
	
struct bss_t {
	int socket_fsa[MAX_CLIENT];
	void *pClient_fs[MAX_CLIENT];
	int socket_fs[MAX_CLIENT];
	volatile int lock;
};
	
#define bss_ptr (*(struct bss_t **)0x100000e4)
#define bss (*bss_ptr)

void memset_bss();
void cafiine_connect(void *pClient, int clientId, int isFSA);
void cafiine_disconnect(void *pClient, int clientId, int isFSA);
int cafiine_handshake(int sock);
int cafiine_fopen(void* pClient, int clientId, int *result, const char *path, const char *mode, int *handle, int isFSA);
int cafiine_fread(void* pClient, int *result, void *buffer, int size, int count, int fd);
int cafiine_fclose(void* pClient, int *result, int fd);
int cafiine_fsetpos(void* pClient, int *result, int fd, int set);
int cafiine_fgetpos(void* pClient, int *result, int fd, int *pos);
int cafiine_fstat(void* pClient, int *result, int fd, void *ptr);
int cafiine_feof(void* pClient, int *result, int fd);

#ifdef __cplusplus
}
#endif

#endif // _CAFIINE_H_
