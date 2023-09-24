#include "wrappers.h"



int main(int argc, char **argv)
{
	int	sockfd;
	struct sockaddr_in	servaddr, cli_addr;
    socklen_t clilen;


	if (argc != 2) {
		printf("usage: ./tcpclient <IPaddress>");
		return 1;
	}

    sockfd = Socket();

    //server addr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(SERV_TCP_PORT);	

    //client
	bzero(&cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port   = htons(SERV_TCP_PORT+1);	

	bind(sockfd, (SA*)&cli_addr,sizeof(cli_addr));



    //Bind(sockfd,(SA *) &cli_addr,sizeof(cli_addr)); //bind the client address to the socket



    Connect(sockfd,(SA*)&servaddr,sizeof(servaddr)); //connect to the server

	PeerName(sockfd,(SA*)&servaddr,sizeof(servaddr),&servaddr);
	SockName(sockfd,(SA*)&cli_addr,sizeof(cli_addr),&cli_addr);
		
	str_cli(stdin, sockfd);



	exit(0);
}
