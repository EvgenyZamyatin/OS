#include "helpers.h"

#define INIT_BUF_LEN 4096
#define MAX_FDS 64


ssize_t read_(int fd, void* buf, size_t count)
{
	int have = 0;
	int rad;
	while ((rad = read(fd, (char*)buf + have, count)) > 0) 
	{
		have += rad;
		count -= rad;
	}
	return have;
}

ssize_t write_(int fd, void* buf, size_t count)
{
	int have = 0;
	int writed;
	while ((writed = write(fd, (char*)buf + have, count)) > 0) 
	{
		have += writed;
		count -= writed;
	}
	return have;
}

typedef struct buffer
{
	int fd;
	int len;
	int size;
	char* data;
} buffer;

void ensure_capacity(buffer* buf) 
{
	int newsize = buf->size * 2;
	char* newdata = malloc(newsize);
	memcpy(newdata, buf->data, buf->len);
	free(buf->data);
	buf->size = newsize;
	buf->data = newdata;
}

ssize_t read_until(int fd, void* out_buf, size_t count, char delimiter) 
{
	static buffer* buf_table = NULL;
	if (buf_table == NULL) 
		buf_table = calloc(MAX_FDS, sizeof(buffer));	
	static int cnt_fd = 0;
	buffer *buf = NULL;
	for (int i = 0; i < cnt_fd; ++i) 
	{
		if (buf_table[i].fd == fd) 
		{
			buf = &buf_table[i];
			break;
		}
	}
	if (buf == NULL) 
	{
		assert(cnt_fd != MAX_FDS);
		buf_table[cnt_fd] = (buffer){.fd = fd, .len = 0, .size = INIT_BUF_LEN, .data = malloc(INIT_BUF_LEN)};
		buf = &buf_table[cnt_fd++];
	}
	while (buf->size < count)
		ensure_capacity(buf);
	int copied_bytes = 0;
	int i = 0;
	count -= buf->len;
	while (1) 
	{
		int cnt = -1;
		if (i == buf->len) 
		{	
			cnt = read(fd, buf->data + buf->len, count);
			if (cnt == -1)
				cnt = 0;
			buf->len += cnt;
			count -= cnt;
		}
		if (buf->data[i] == delimiter || cnt == 0)
		{
			if (cnt == 0) 
				copied_bytes = buf->len;
			else 
				copied_bytes = i + 1;
			memcpy(out_buf, buf->data, copied_bytes);
			for (int j = copied_bytes; j < buf->len; ++j)
				buf->data[j - copied_bytes] = buf->data[j];
			buf->len -= copied_bytes;
			break;
		}
		++i;
	}	
	return copied_bytes;
}

//First argument must be name of executable file.
int spawn(const char* file, char* const argv[]) {
	int np = fork();
	if (np == -1)
		return -1;
	if (np) {
		int res;
 		waitpid(np, &res, 0);
 		return res;
	} else {
		//int fd = open("/dev/null",  O_WRONLY);
        //dup2(fd, STDOUT_FILENO);              
        //dup2(fd, STDOUT_FILENO);              
        //dup2(fd, STDERR_FILENO);              
        //close(fd);
		execvp(file, argv);
        exit(0);
	}
}

execargs_t* execargs(char *file, char** args) {
    execargs_t* ans = malloc(sizeof(execargs_t));
    *ans = (execargs_t){
        .file = file,
        .args = args,
        .outfd = -1,
        .infd = -1
    };
    return ans;
}

static void sigint_ignore(int sig) {};
static void sigint_exit(int sig) {
    char buf[256];
    sprintf(buf, "Process %d killed\n", getpid());
    write(STDERR_FILENO, buf, strlen(buf));
    exit(0);
}

int exec(execargs_t *args) {
    int np = fork();
	if (np == -1)
		return -1;
	if (np) {
 		return np;
	} else {
        //close(cls);
        signal(SIGINT, sigint_exit);
        if (((args->outfd > 0 && dup2(args->outfd, STDOUT_FILENO) < 0) ||
                (args->infd > 0 && dup2(args->infd, STDIN_FILENO) < 0))) {
            return -1;
        }
        execvp(args->file, args->args);
        exit(0);
	};
    return np;
}

int runpiped(execargs_t** p, size_t n) {
    if (signal(SIGINT, sigint_ignore) < 0) {
        perror("Cant bind signal handler");
        return -1;
    }
    int ans = 0;
    int last = -1;
    int lastproc = -1;
    for (int i = 0; i < n; ++i) {
        int pipefd[2];
        if (pipe(pipefd) < 0) {
            ans = -1;
            break;
        }
        if (i > 0) 
            p[i]->infd = last;
        else 
            p[i]->infd = -1;

        if (i != n - 1) 
            p[i]->outfd = pipefd[1];
        else 
            p[i]->outfd = -1;
        if ((lastproc = exec(p[i])) < 0) {
            ans = -1;
            break;
        }
        if (i > 0)
            close(last);
        close(pipefd[1]);
        last = pipefd[0];
    }
    close(last);
    if (ans == -1)
        perror("Error at runepiped");
    int res;
    waitpid(lastproc, NULL, 0);
    kill(-getpid(), SIGINT);
    //pause();
    signal(SIGINT, sigint_exit); 
    return ans;
}

int main() {
    return 0;
}














