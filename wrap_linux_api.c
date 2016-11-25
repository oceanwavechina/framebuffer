# include "common.h"

int Open(const char *pathname, int flags)
{
	int fd;
	fd = open(pathname, flags);
	if(fd < 0){
		fprintf(stderr, "open %s Error :<%s>\n", pathname, strerror(errno));
		exit(1);
	}

	return fd;
}

int Read(int fd, char *buf, int size)
{
	int n;
	n = read(fd, buf, size);
	if(n < 0){
		fprintf(stderr, "read Error: <%s>\n", strerror(errno));
		exit(1);
	}

	return n;
}

int Pthread_create(pthread_t *id, pthread_attr_t *attr, func *f, void *arg)
{
	int ret;

	ret = pthread_create(id, attr, f, arg);
	if(ret != 0){
		fprintf(stderr, "thread create Error: <%s>\n", strerror(errno));
		exit(1);
	}

	return 0;
}
