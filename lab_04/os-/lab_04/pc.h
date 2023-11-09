#ifndef __PC_H__
#define __PC_H__

#define SE 0
#define SF 1
#define SB 2

int producer(char *addr, int semid);
int consumer(char *addr, int semid);
void sig_handler(int sig_num);

#endif