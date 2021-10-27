//
// Created by fox on 2021/10/22.
//

#include "SingleInstance.h"

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/stat.h>

/**
 * lockfile 对文件加锁
 * @param fd
 * @return
 */
static int lockfile(int fd)
{
    flock fl;
    fl.l_type   = F_WRLCK;
    fl.l_start  = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len    = 0;
    return(fcntl(fd, F_SETLK, &fl));
}

/**
 * proc_is_exist 判断系统中是否存在该进程
 * @param procname 进程名
 * @return 返回1表示系统中已经存在该进程了；返回0表示系统中不存在该进程
 */
int proc_is_exist(const char *procname)
{
    int  fd;
    char buf[16];
    char filename[100];

    sprintf(filename, "%s.pid", procname);

    fd = open(filename, O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    if (fd < 0) {
        printf("open file \"%s\" failed!!!\n", filename);
        return 1;
    }

    /* 尝试对文件进行加锁,此处加锁完后无需对文件进行close，而是进程退出后由系统来释放；否则无法起到保护的作用 */
    if (lockfile(fd) == -1) {
        close(fd);
        return 1;
    } else {
        ftruncate(fd, 0);
        sprintf(buf, "%ld", (long)getpid());
        write(fd, buf, strlen(buf) + 1);/* 写入运行实例的pid */
        return 0;
    }
}