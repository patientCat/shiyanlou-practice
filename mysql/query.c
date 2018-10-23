#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";	//本地数据库
	char* user="root";	
	char* password="321";
	char* database="test25";//要访问的数据库名称
	char query[300]="select * from Person where ID=";
	sprintf(query,"%s%s",query,argv[1]);
	//strcpy(query,"select * from Person");
	puts(query);
	int t,r;
	conn=mysql_init(NULL);
	//连接数据库，失败返回0，成功返回1
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))	
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
	//向conn发送查询指令，
	t=mysql_query(conn,query);	//看查询是否成功， 
	//查询成功返回0，失败返回1
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
	//	printf("Query made...\n");
		//成功之后返回数据给res
		//如果执行语句之后没有找到相应的数据就返回NULL
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)
				{
						printf("%8s ",row[t]);
				}
				printf("\n");
			}
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}

//多个连接，用连接池，和线程池一样的思想，一开始就建立100个conn，放入队列，有请求来了就给他一个。

//把整个查询函数封装起来，把conn,res,query传进去，返回res。