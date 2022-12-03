#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"

#define MAX_FD 65536   // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000  // 监听的最大的事件数量

// 添加文件描述符
extern void addfd( int epollfd, int fd, bool one_shot );
extern void removefd( int epollfd, int fd );



// int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
//     为指定信号改变或检查处理动作
//     - signum:要捕捉的信号   注：SIGKILL和SIGSTOP无法被捕获或者忽略
//     - act:捕捉到信号后的处理动作
//     - oldact：传出参数，上一次信号捕捉时设置的动作，一般为NULL
//     返回值 成功0 失败-1
void addsig(int sig, void( handler )(int)){
    struct sigaction sa;
    // memset( &sa, '\0', sizeof( sa ) );
    bzero(&sa,sizeof(sa));
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    // 阻塞信号集中所有位置1
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

int main( int argc, char* argv[] ) {
    
    if( argc <= 1 ) {
        printf( "usage: %s port_number\n", basename(argv[0]));
        return 1;
    }

    // 把字符串转化为整数
    int port = atoi( argv[1] );
// 这里需要捕获SIGPIPE并对其做SIG_IGN处理的原因是：https://www.cnblogs.com/myyan/p/4703887.html
    addsig( SIGPIPE, SIG_IGN );

// 创建一个线程池，线程池的类型是任务的类型。
    threadpool<http_conn> * pool = nullptr;
    try {
        pool = new threadpool<http_conn>(4,10000);
    } catch( ... ) {//catch(…)能够捕获多种数据类型的异常对象
        printf("pool error\n");
        return 1;
    }

// 一个装满任务的数组，
    http_conn* users = new http_conn[ MAX_FD ];

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );

    int ret = 0;
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET,"127.0.0.1",&address.sin_addr.s_addr);
    address.sin_family = AF_INET;
    address.sin_port = htons( port );

    // 端口复用
    int reuse = 1;
    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );

    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    ret = listen( listenfd, 5 );

    // 创建epoll对象，和事件数组，添加
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 1 );
    // 将listenfd添加到epoll对象中
    addfd( epollfd, listenfd, false );
    http_conn::m_epollfd = epollfd;

    while(true) {
        
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        
        if ( ( number < 0 ) && ( errno != EINTR ) ) {
            printf( "epoll failure\n" );
            break;
        }

        for ( int i = 0; i < number; i++ ) {
            
            int sockfd = events[i].data.fd;
            
            if( sockfd == listenfd ) {
                
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                
                if ( connfd < 0 ) {
                    printf( "errno is: %d\n", errno );
                    continue;
                } 

                if( http_conn::m_user_count >= MAX_FD ) {
                    //已连接的客户数量以达到最大值
                    close(connfd);
                    continue;
                }
                users[connfd].init( connfd, client_address);

            } else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) ) {

                users[sockfd].close_conn();

            } else if(events[i].events & EPOLLIN) {

                if(users[sockfd].read()) {
                    pool->append(users[sockfd]);
                } else {
                    users[sockfd].close_conn();
                }

            }  else if( events[i].events & EPOLLOUT ) {

                if( !users[sockfd].write() ) {
                    users[sockfd].close_conn();
                }

            }
        }
    }
    
    close( epollfd );
    close( listenfd );
    delete [] users;
    delete pool;
    return 0;
}