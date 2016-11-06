#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
int main(int argc , char *argv[])
{
int socket_desc , new_socket , c;
struct sockaddr_in server , client;
char *message , s[4] , cmsg[2000];
int i , j , k, l , num , x=0 , y=0 ,Ans[10],destination[4],intput=0;
//Create socket
socket_desc = socket(AF_INET , SOCK_STREAM , 0);
if (socket_desc == -1)
{
	printf("Could not create socket");
}
//Prepare the sockaddr_in structure
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons( 8888 );
//Bind
if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
{
	puts("bind failed");
	return 1;
}
puts("bind done");
//Listen
listen(socket_desc , 3);
//Accept and incoming connection
puts("Waiting for incoming connections...");
c = sizeof(struct sockaddr_in);
new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

if (new_socket<0)
{
	perror("accept failed");
	return 1;
}
puts("Connection accepted");

for( i = 0 ; i < 10 ; i++)
	Ans[i] = i;
srand(time(NULL));
for ( i = 0 ; i < 4 ; i++ ) 
{
	k = rand() % 10;
	if( k == i )
		continue;
	l = Ans[i] ;
	Ans[i] = Ans[k] ;
	Ans[k] = l ;
}

for( i = 3 ; i >= 0 ; i-- )
	printf("%d",Ans[i]);
printf("\n");

//Reply
while(1)
{
	bzero(s,4);
	read(new_socket,s,4);
	intput=1000*(s[0]-'0')+100*(s[1]-'0')+10*(s[2]-'0')+(s[3]-'0');
	destination[3] = intput / 1000 % 10;
	destination[2] = intput / 100 % 10 ;
	destination[1] = intput / 10 % 10 ;
	destination[0] = intput / 1 % 10 ;
	for( i = 0 ; i < 4 ; i++ )
		for( j = 0 ; j < 4 ; j++ )
			if( Ans[i] == destination[j] )
				if( i == j )
					x++;
				else
					y++;
	if( x!=4 )
	{
		sprintf(cmsg ,"%dA%dB\n",x,y);
		write(new_socket,cmsg,strlen(cmsg));
		x=0,y=0;
	}
	else
	{
		message="Correct!!\n";
		write(new_socket , message , strlen(message));
		return 1;
	}
}
return 0;
}

