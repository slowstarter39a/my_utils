/*
 * =====================================================================================
 *
 *       Filename:  thread.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/10/2018 10:30:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *        Company:
 *
 * =====================================================================================
 */
#include <thread>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "ring_buffer.h"
#include <stdlib.h>
#include <ctime>

using namespace std;
int fd[2];
ring_buff_t rb;
U8 send_data[1024 * 1024] = {0,};
U8 read_data[1024 * 1024] = {0,};

void thread_func_1(int val)
{
	U32 i = 0;
	U32 rand_num = 0;
	U8 send_idx = 0;
	char buf[255] = {0,};


	while(1)
	{
		srand((unsigned int)time(NULL));
		rand_num = rand() % (1024 * 1024 - 1);

		if(ring_buff_is_full(&rb))
		{
            continue;
		}

		for(i = 0; i < rand_num; i++)
		{
			send_data[i] = send_idx;
			send_idx++;
		}
		if(ring_buff_is_full(&rb))
		{
			sleep(1);
			std::cout<<"ring_buff_is_full : wait 1s"<<std::endl;

		}

		ring_buff_put_data(&rb, (byte *)send_data, rand_num);

		sprintf(buf, "1\n");
		write(fd[1], buf, strlen(buf));
		usleep(10000);

	}
}

void thread_func_2(int val)
{
	U8 i = 0;
	U32 read_idx = 0;
	int n = 0;
	char buf[255] = {0,};
	U32 read_count = 0;
	U64 total_read_count = 0;

	while(1)
	{
		memset(buf, 0x0, 255);
		n = read(fd[0], buf, 255);

		read_count = ring_buff_get_data(&rb, (byte *)read_data, 1024 * 1024 - 1);
		std::cout<<"read_count "<<read_count<<std::endl;

		for(read_idx = 0; read_idx < read_count; read_idx++)
		{
			if(read_data[read_idx] == i )
			{
	//			std::cout<<(int)i<<std::endl;
				i++;
			}
			else
			{
				std::cout<<read_data[read_idx]<<"	"<<i<<std::endl;
				exit(0);
				break;
			}
		}
		total_read_count += read_count;
		std::cout<<"read_count "<<read_count<<std::endl;
		std::cout<<"total_read_count "<<total_read_count<<std::endl;
	}
}

int main(void)
{
	if(pipe(fd) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}

	ring_buff_init(&rb, 1024 * 1024 + 3);

	std::thread th1(thread_func_1, 100);
	std::thread th2(thread_func_2, 500);

	th1.join();
	th2.join();
}

