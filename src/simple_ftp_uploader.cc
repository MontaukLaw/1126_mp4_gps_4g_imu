#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "simple_ftp_uploader.h"

#define MAX_BUF 512
#define IP_LENGTH 16

// 正常时服务器回复的响应码
#define ACK_USER_NUM "331"
#define ACK_PASS_NUM "230"
#define ACK_PASV_NUM "227"
#define ACK_CWD_NUM "250"
#define ACK_SIZE_NUM "213"
#define ACK_RETR_NUM "150"
#define ACK_REST_NUM "350"
#define ACK_QUIT_NUM "200"
#define ACK_LIST_NUM "125"
#define ACK_STOR_NUM "150"
#define ACK_CONNECT_NUM "220"
#define ACK_PORT_NUM "200"

/*ftp server info*/
typedef struct
{
    // char szUserName[16];
    // char szPassWd[32];
    char server_path[128];
    char server_filename[64];
    char new_filename[128];
    int data_sock;
    char data_ip[32];
    int data_port;
    int client_server_sock;
    int file_handle;
} FTP_DATA_INFO;

static int itoa(int value, char *str, int radix);
static int send_cmd(int ctrl_sock, eu_cmd_type typ, const char *val, const char *ack_num);
static int enter_passive_mode(int ctrl_sock, char *data_ip, int *data_port);
static int enter_active_mode(int ctrl_sock);

static int get_data_sock(const char *server_ip, const int port);
static int get_active_data_sock(int client_server_sock);

static int GetAddr(const char *ifname, char *addr, int flag);
static int close_st_info(FTP_DATA_INFO *info);

FTP_DATA_INFO server_info;

static int GetAddr(const char *ifname, char *addr, int flag)
{
    struct sockaddr_in *sin;
    struct ifreq ifr;
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket create error!\n");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, ifname, IFNAMSIZ);

    if (ioctl(sockfd, flag, &ifr) < 0)
    {
        close(sockfd);
        return -1;
    }

    close(sockfd);

    if (SIOCGIFHWADDR == flag)
    {
        memcpy((void *)addr, (const void *)&ifr.ifr_ifru.ifru_hwaddr.sa_data, 6);
    }
    else
    {
        sin = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr;
        snprintf((char *)addr, IP_LENGTH, "%s", inet_ntoa(sin->sin_addr));
    }

    return 0;
}

static int itoa(int value, char *str, int radix)
{
    char temp[33];
    char *tp = temp;
    int i;
    unsigned v;
    int sign;
    char *sp;
    int num = 0;
    if (radix > 36 || radix < 1)
        return 0;
    sign = (radix == 10 && value < 0); // 十进制负数
    if (sign)
        v = -value;
    else
        v = (unsigned)value;
    while (v || tp == temp) // 转化操作
    {
        i = v % radix;
        v = v / radix;
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'a' - 10;
    }
    if (str == 0)
        str = (char *)malloc((tp - temp) + sign + 1);
    sp = str;
    if (sign) // 是负数的话把负号先加入数组
        *sp++ = '-';
    while (tp > temp)
    {
        *sp++ = *--tp;
        num++;
    }
    *sp = 0;

    return num;
}

/*
 * @brief 连接fpt服务器
 * @param 无
 * @return -1/成功建立的套接字
 */
int connect_ftp_server(const char *server_ip, const int port)
{
    int control_sock = -1;
    int ret = -1;
    struct sockaddr_in server;
    char read_buf[MAX_BUF] = {0};
    struct timeval tv_out;

    memset(&server, 0, sizeof(struct sockaddr_in));

    if (server_ip == NULL)
    {
        printf("argc is NULL\n");
        return -1;
    }

    control_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (control_sock < 0)
    {
        printf("socket failed\n");
        return -1;
    }

    /*设置sock fd 接收超时时间*/
    tv_out.tv_sec = 0;
    tv_out.tv_usec = 500 * 1000;
    setsockopt(control_sock, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    ret = connect(control_sock, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0)
    {
        printf("connect failed\n");
        return -1;
    }

    ret = 1;

    /*接收服务端的应答消息*/
    usleep(100 * 1000);
    ret = read(control_sock, read_buf, sizeof(read_buf));
    if (ret < 0)
    {
        printf("read error\n");
        return -1;
    }
    printf("%s ret=%d \n", read_buf, ret);

    if (strncmp(read_buf, ACK_CONNECT_NUM, 3) == 0) /*成功*/
    {
        printf("Connect ftp ok\n");
        return control_sock;
    }
    else
    {
        close(control_sock);
        return -1;
    }
}

/*
 * @brief 被动模式连接获取服务器的data_sock
 * @param 无
 * @return -1/成功建立的套接字
 */
static int get_data_sock(const char *server_ip, const int port)
{
    int data_sock = -1;
    int ret = -1;
    struct sockaddr_in server;
    char read_buf[MAX_BUF] = {0};

    memset(&server, 0, sizeof(struct sockaddr_in));

    if (server_ip == NULL)
    {
        printf("argc is NULL\n");
        return -1;
    }

    data_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (data_sock < 0)
    {
        printf("socket failed\n");
        return -1;
    }
    /*设置为非阻塞*/
    // int cflags = fcntl(data_sock,F_GETFL,0);
    // fcntl(data_sock,F_SETFL,cflags|O_NONBLOCK);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    ret = connect(data_sock, (struct sockaddr *)&server, sizeof(server));
    if (ret < 0)
    {
        printf("connect failed\n");
        return -1;
    }
    /*无应答*/

    return data_sock;
}

/*主动模式获取data sock
  必须要在LIST等下载上传命令发送后
  accept接受才能成功

*/
static int get_active_data_sock(int client_server_sock)
{
    int data_sock = -1;
    struct sockaddr_in client_name;
    socklen_t len; // int len;

    len = sizeof(client_name);
    data_sock = accept(client_server_sock, (struct sockaddr *)&client_name, &len);
    if (data_sock < 0)
    {
        printf("accept failed\n");
    }
    printf("data_sock = %d\n", data_sock);

    return data_sock;
}

/*
 * @brief 登陆fpt服务器
 * @param 无
 * @return -1/成功返回0
 */
int login_ftp_server(int ctrl_sock, const char *user_name, const char *passwd)
{
    int ret = -1;

    if ((user_name == NULL) || (passwd == NULL))
    {
        printf("argc is NULL\n");
        return -1;
    }

    ret = send_cmd(ctrl_sock, CMD_USER, user_name, ACK_USER_NUM);
    if (ret < 0)
    {
        printf("send_cmd  %d failed \n", CMD_USER);
        return -1;
    }

    ret = send_cmd(ctrl_sock, CMD_PASS, passwd, ACK_PASS_NUM);
    if (ret < 0)
    {
        printf("send_cmd  %d failed \n", CMD_PASS);
        return -1;
    }
    return 0;
}

/*
* @brief 给服务器发送指令
* @param
* @return 失败返回-1/成功返回0
    SIZE 返回等到的文件大小
*/

static int send_cmd(int ctrl_sock, eu_cmd_type typ, const char *val, const char *ack_num)
{
    int ret = -1;
    char send_buf[MAX_BUF] = {0};
    char read_buf[MAX_BUF] = {0};
    char *pos = NULL;
    char tmp[64] = {0};

    if ((typ == CMD_USER) || (typ == CMD_PASS) || (typ == CMD_CWD))
    {
        if ((val == NULL) || (ack_num == NULL))
        {
            printf("argc is NULL\n");
            return -1;
        }
    }

    switch (typ)
    {
    case CMD_USER:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "USER %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        ret = strncmp(read_buf, ack_num, strlen(ack_num));
        break;
    case CMD_PASS:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "PASS %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        ret = strncmp(read_buf, ack_num, strlen(ack_num));
        break;
    case CMD_PASV: /*只发送命令，函数外面接收提取信息*/
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "PASV\r\n");
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        break;
    case CMD_CWD:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "CWD %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        usleep(500 * 1000);
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        ret = strncmp(read_buf, ack_num, strlen(ack_num));
        break;
    case CMD_QUIT:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "QUIT\r\n");
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        usleep(500 * 1000);
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        //    ret = strncmp(read_buf,ack_num,strlen(ack_num));

    case CMD_LIST:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "LIST %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        memset(read_buf, 0, sizeof(read_buf));
        usleep(100 * 1000); /*等待一会把266 也接收回来*/
        ret = read(ctrl_sock, read_buf, sizeof(send_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        //    ret = strncmp(read_buf,ack_num,strlen(ack_num));

        break;

    case CMD_STOR:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "STOR %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        usleep(50 * 1000);
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        //    ret = strncmp(read_buf,ack_num,strlen(ack_num));

        break;

    case CMD_RETR:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "RETR %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        usleep(50 * 1000);
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        //    ret = strncmp(read_buf,ack_num,strlen(ack_num));

        break;

    case CMD_SIZE_FTP:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "SIZE %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        usleep(50 * 1000);
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        /* 客户端接收服务器的响应码和信息，正常为 ”213 <size>” */
        pos = strstr(read_buf, ack_num);
        if (pos != NULL)
        {
            pos += strlen(ack_num) + 1;
            strcpy(tmp, pos);
            ret = atoi(tmp);
        }
        else
        {
            ret = -1;
        }

        break;
    case CMD_PORT_FTP:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "PORT %s\r\n", val);
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        usleep(50 * 1000);
        memset(read_buf, 0, sizeof(read_buf));
        ret = read(ctrl_sock, read_buf, sizeof(read_buf));
        if (ret < 0)
        {
            printf("read failed\n");
            return -1;
        }
        ret = strncmp(read_buf, ack_num, strlen(ack_num));
        break;
    case CMD_MLSD:
        memset(send_buf, 0, sizeof(send_buf));
        sprintf(send_buf, "MLSD\r\n");
        ret = write(ctrl_sock, send_buf, strlen(send_buf));
        if (ret < 0)
        {
            printf("write failed\n");
            return -1;
        }
        break;
    default:
        break;
    }
    printf("FTP server ack= %s\n", read_buf);
    return ret;
}

/*
 * @brief 进入主动模式，让服务器主动连接到客户端的端口
 * @param 无
 * @return -1/成功返回client_server_sock
 */
static int enter_active_mode(int ctrl_sock)
{
    int data_sock, server_sock;
    struct sockaddr_in name;
    struct sockaddr_in client_name, loc_addr;
    unsigned short server_port = 0;
    int ret = -1;
    socklen_t len; // int len = 0;
    char send_buf[64] = {0};
    char ip[20] = {0};
    unsigned short ip0, ip1, ip2, ip3, p1, p2;
    // char read_buf[128] ={0};

    struct timeval tv_out;

    /*设置sock fd 接收超时时间*/
    tv_out.tv_sec = 3;
    tv_out.tv_usec = 0;

    /*
      if(GetAddr("eth0", ip, SIOCGIFADDR) != 0)
      {
           printf("get local ip failed\n");
          return -1;
      }
      printf("local ip =%s\n",ip);
      */
    memset(&name, 0, sizeof(name));
    memset(&client_name, 0, sizeof(client_name));

    /*通过ctrl_sock获取到本机的ip*/
    len = sizeof(name);
    if (getsockname(ctrl_sock, (struct sockaddr *)&name, &len) == -1)
    {
        printf("get sock name failed\n");
        return -1;
    }

    sscanf(inet_ntoa(name.sin_addr), "%hu.%hu.%hu.%hu", &ip0, &ip1, &ip2, &ip3);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        printf("get sock failed\n");
        return -1;
    }
    /*设置接收超时*/
    setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

    name.sin_family = AF_INET;
    name.sin_port = 0;
    len = sizeof(name);
    ret = bind(server_sock, (struct sockaddr *)&name, len);
    if (ret < 0)
    {
        printf("bind error\n");
        goto err0;
    }

    /*通过ctrl_sock获取到系统分配到的端口*/
    len = sizeof(loc_addr);
    memset(&loc_addr, 0, len);
    ret = getsockname(server_sock, (struct sockaddr *)&loc_addr, &len);
    if (ret < 0)
    {
        printf("get sock name failed\n");
        goto err0;
    }
    server_port = ntohs(loc_addr.sin_port);

    p1 = server_port / 256;
    p2 = server_port % 256;

    ret = listen(server_sock, 10);
    if (ret < 0)
    {
        printf("listen error\n");
        goto err0;
    }

    /*给服务器 命令 “PORT       \r\n*/
    /*将ip中的.更换为，*/
#if 0    
    &ip0 = strtok(ip,".");
    &ip1 = strtok(NULL,".");
    &ip2 = strtok(NULL,".");
    &ip3 = strtok(NULL,".");
#endif

    sprintf(send_buf, "%hu,%hu,%hu,%hu,%hu,%hu", ip0, ip1, ip2, ip3, p1, p2);
    printf("send_buf =%s server_port=%d\n", send_buf, server_port);
    ret = send_cmd(ctrl_sock, CMD_PORT_FTP, send_buf, ACK_PORT_NUM);
    if (ret < 0)
    {
        printf("Send PORT failed\n");
        goto err0;
    }

    return server_sock;
err0:
    close(server_sock);
    return -1;
}

/*
 * @brief 进入被动模式，让服务器在数据端口监听数据
 * @param 无
 * @return -1/成功返回0
 */

static int enter_passive_mode(int ctrl_sock, char *data_ip, int *data_port)
{
    int ret = -1;
    char read_buf[MAX_BUF] = {0};
    char tmp_buf[64] = {0};
    unsigned char ip1, ip2, ip3, ip4, port1, port2;
    // char *tmp;

    if ((data_ip == NULL) || (data_port == NULL))
    {
        printf("argc is NULL\n");
        return -1;
    }
    ret = send_cmd(ctrl_sock, CMD_PASV, NULL, NULL);
    if (ret < 0)
    {
        printf("send_cmd  %d failed \n", CMD_PASV);
        return -1;
    }

    usleep(100 * 1000);
    ret = read(ctrl_sock, read_buf, sizeof(read_buf));
    if (ret < 0)
    {
        printf("read failed\n");
        return -1;
    }
    printf("rev =%d: %s\n", ret, read_buf);
    if (strstr(read_buf, ACK_PASV_NUM) != NULL)
    {

        sscanf(strchr(read_buf, '(') + 1, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
        // printf("ip1=%d,ip2=%d,ip3=%d,ip4=%d,port1 =%d ,port2 = %d\n",ip1,ip2,ip3,ip4,port1,port2);

        // snprintf(data_ip,sizeof(data_ip),"%hhu,%hhu,%hhu,%hhu",ip1,ip2,ip3,ip4);

        // memset(data_ip,0,sizeof(data_ip));
        // snprintf(data_ip,sizeof(data_ip),"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
        // printf("data_ip = %s\n",data_ip);

        memset(data_ip, 0, sizeof(data_ip));

        memset(tmp_buf, 0, sizeof(tmp_buf));
        itoa(ip1, tmp_buf, 10);
        strcat(data_ip, tmp_buf);
        strcat(data_ip, ".");

        memset(tmp_buf, 0, sizeof(tmp_buf));
        itoa(ip2, tmp_buf, 10);
        strcat(data_ip, tmp_buf);
        strcat(data_ip, ".");

        memset(tmp_buf, 0, sizeof(tmp_buf));
        itoa(ip3, tmp_buf, 10);
        strcat(data_ip, tmp_buf);
        strcat(data_ip, ".");

        memset(tmp_buf, 0, sizeof(tmp_buf));
        itoa(ip4, tmp_buf, 10);
        strcat(data_ip, tmp_buf);
        // printf("data_ip1 = %s\n",data_ip);

        *data_port = port1 * 256 + port2;
    }
    return 0;
}

/*
* @brief 从ftp服务器上下载文件
* @param
  ctrl_sock 控制服务器sock
  connect_mode= 0 设置服务器被动模式下载，非0服务器主动模式下载
  server_filepath_name 要下载文件的路径
  newfilename 下载到本地的路径和文件名字
  offset 设置下载文件偏移的位置，不偏移写0 ，可用于错误时续传。
* @return -1/成功返回未接收到的字节数
*/

int down_file_ftpserver(int ctrl_sock, char *server_filepath_name,
                        const char *newfilename, int connect_mode, int offset, eu_cmd_type typ)
{

    int ret = -1, file_size = 0;
    char rec_buf[2048] = {0};
    char stri[128] = {0};
    int read_size = 0;
    char *pos = NULL;
    char *tmp = NULL;
    int flags = O_CREAT | O_RDWR | O_TRUNC;
    ;

    close_st_info(&server_info);

    if (server_filepath_name == NULL || newfilename == NULL)
    {
        printf("argc is NULL\n");
        return -1;
    }

    /*提取出server_path file name*/
    tmp = server_filepath_name;

    if (tmp != NULL)
    {
        while (tmp != NULL)
        {
            tmp = strstr(tmp, "/");
            //    printf("tmp =%x :%s\n",tmp,tmp);
            if (tmp != NULL)
            {
                pos = tmp;
                tmp++; /*越过找到的"/"*/
            }
        }
        if (pos != NULL)
        {
            strncpy(server_info.server_path, server_filepath_name, pos - server_filepath_name + 1);
            strcpy(server_info.server_filename, pos + 1);
        }
        else
        {
            strcpy(server_info.server_filename, server_filepath_name);
        }
    }

    printf("server path = %s ;file name =%s\n", server_info.server_path, server_info.server_filename);

    if ((typ != CMD_RETR) && (typ != CMD_LIST))
    {
        printf("typ value is not CMD_RETR or CMD_LIST\n");
        return -1;
    }

    if (connect_mode)
    { /*主动模式*/

        server_info.client_server_sock = enter_active_mode(ctrl_sock);
        if (server_info.client_server_sock < 0)
        {
            printf("get data_scok failed\n");
            return -1;
        }
    }
    else
    { /*被动模式*/
        ret = enter_passive_mode(ctrl_sock, server_info.data_ip, &server_info.data_port);
        if (ret < 0)
        {
            printf("set passive mode failed\n");
            return -1;
        }
        printf("server_info.data_ip =%s, data_port =%d \n", server_info.data_ip, server_info.data_port);
        server_info.data_sock = get_data_sock(server_info.data_ip, server_info.data_port);
        if (server_info.data_sock < 0)
        {
            printf("get data sock failed\n");
            return -1;
        }
    }

    /*更改目录*/
    if (strlen(server_info.server_path) != 0)
    {
        ret = send_cmd(ctrl_sock, CMD_CWD, server_info.server_path, ACK_CWD_NUM);
        if (ret < 0)
        {
            printf("set passive mode failed\n");
            goto err0;
        }
    }

    if (typ == CMD_RETR)
    { /*发送下载文件命令*/

        /*设置下载文件偏移的位置*/
        if (offset > 0)
        {

            flags = O_CREAT | O_RDWR | O_APPEND;
            itoa(offset, stri, 10);
            ret = send_cmd(ctrl_sock, CMD_REST, stri, ACK_REST_NUM);
            if (ret < 0)
            {
                printf("set file offsize failed\n");
                goto err0;
            }
        }
        ret = send_cmd(ctrl_sock, CMD_RETR, server_info.server_filename, ACK_RETR_NUM);
        if (ret < 0)
        {
            printf("send RETR failed\n");
            goto err0;
        }
    }
    else if (typ == CMD_LIST)
    {
        ret = send_cmd(ctrl_sock, CMD_LIST, server_filepath_name, ACK_LIST_NUM);
        if (ret < 0)
        {
            printf("send LIST failed\n");
            goto err0;
        }
    }

    if (connect_mode)
    {
        server_info.data_sock = get_active_data_sock(server_info.client_server_sock);
        if (server_info.data_sock < 0)
        {
            printf("accept failed\n");
            goto err0;
        }
    }

    server_info.file_handle = open(newfilename, flags, 0766);
    if (server_info.file_handle < 0)
    {
        printf("open file failed\n");
        goto err0;
    }
    if (offset > 0)
    {
        lseek(server_info.file_handle, offset, SEEK_SET);
        read_size += offset;
    }

    for (;;)
    {
        memset(rec_buf, 0, sizeof(rec_buf));

        ret = recv(server_info.data_sock, rec_buf, sizeof(rec_buf), 0);
        if (ret < 0)
        {
            printf("Read error\n");
            goto err1;
        }
        else if (ret == 0)
        {
            ret = read_size;
            goto err1;
        }
        else if (ret > 0)
        {
            read_size += ret;
            ret = write(server_info.file_handle, rec_buf, ret);
            if (ret < 0)
            {
                printf("Write error\n");
                goto err1;
            }
            // printf("read_buf =%s\n",rec_buf);
        }
    }

err1:
    if (server_info.file_handle > 0)
        close(server_info.file_handle);
err0:
    if (server_info.client_server_sock > 0)
        close(server_info.client_server_sock);

    if (server_info.data_sock > 0)
        close(server_info.data_sock);
    memset(rec_buf, 0, sizeof(rec_buf));
    /* 客户端接收服务器的响应码和信息，正常为 ”226 Transfer complete.” */
    read(ctrl_sock, rec_buf, sizeof(rec_buf)); /*有时消息再发送完命令后里面就能收到，导致会阻塞在这个上面*/
    printf("%s \n Download file end!!\n", rec_buf);

    return ret;
}

/*
* @brief 上传文件从ftp服务器
* @param
  ctrl_sock 控制服务器sock
  connect_mode = 0 设置服务器被动模式，非0 服务器主动模式
  server_filepath_name  上传到服务器上的文件路径和名称
  srcfilename 本地要上传的路径和文件名字
  offset 设置下载文件偏移的位置，不偏移写0 ，可用于错误时续传。
* @return -1/成功返回上传完成的字节数
*/

int up_file_ftpserver(int ctrl_sock, char *server_filepath_name,
                      const char *srcfilename, int connect_mode, int offset)

{

    int ret = -1, file_size = 0;
    int file_handle = 0;
    char rec_buf[2048] = {0};
    int read_size = 0;
    char stri[128] = {0};
    char *tmp = NULL;
    char *pos = NULL;

    close_st_info(&server_info);
    if ((server_filepath_name == NULL) || (srcfilename == NULL))
    {
        printf("argc is NULL\n");
        return -1;
    }

    /*提取出server_path file name*/
    tmp = server_filepath_name;

    if (tmp != NULL)
    {
        while (tmp != NULL)
        {
            tmp = strstr(tmp, "/");
            //    printf("tmp =%x :%s\n",tmp,tmp);
            if (tmp != NULL)
            {
                pos = tmp;
                tmp++; /*越过找到的"/"*/
            }
        }
        if (pos != NULL)
        {
            strncpy(server_info.server_path, server_filepath_name, pos - server_filepath_name + 1);
            strcpy(server_info.server_filename, pos + 1);
        }
        else
        {
            strcpy(server_info.server_filename, server_filepath_name);
        }
    }

    printf("server path = %s ;file name =%s\n", server_info.server_path, server_info.server_filename);

    if (connect_mode)
    { /*主动模式*/

        server_info.client_server_sock = enter_active_mode(ctrl_sock);
        if (server_info.client_server_sock <= 0)
        {
            printf("get data_scok failed\n");
            return -1;
        }
    }
    else
    { /*被动模式*/
        ret = enter_passive_mode(ctrl_sock, server_info.data_ip, &server_info.data_port);
        if (ret < 0)
        {
            printf("set passive mode failed\n");
            return -1;
        }
        printf("server_info.data_ip =%s, data_port =%d\n", server_info.data_ip, server_info.data_port);
        server_info.data_sock = get_data_sock(server_info.data_ip, server_info.data_port);
        if (server_info.data_sock < 0)
        {
            printf("get data sock failed\n");
            return -1;
        }
    }
    /*更改目录*/
    if (strlen(server_info.server_path) != 0)
    {
        ret = send_cmd(ctrl_sock, CMD_CWD, server_info.server_path, ACK_CWD_NUM);
        if (ret < 0)
        {
            printf("set passive mode failed\n");
            goto err0;
        }
    }

    /*设置上传文件偏移的位置*/
    if (offset > 0)
    {
        itoa(offset, stri, 10);
        ret = send_cmd(ctrl_sock, CMD_REST, stri, ACK_REST_NUM);
        if (ret < 0)
        {
            printf("set file offsize failed\n");
            goto err0;
        }
    }

    /*发送上传文件命令*/
    ret = send_cmd(ctrl_sock, CMD_STOR, server_info.server_filename, ACK_STOR_NUM);
    if (ret < 0)
    {
        printf("send STOR failed\n");
        goto err0;
    }

    if (connect_mode)
    {
        server_info.data_sock = get_active_data_sock(server_info.client_server_sock);
        if (server_info.data_sock < 0)
        {
            printf("accept failed\n");
            goto err0;
        }
    }

    server_info.file_handle = open(srcfilename, O_RDONLY);
    if (server_info.file_handle < 0)
    {
        printf("open file failed\n");
        goto err0;
    }
    if (offset > 0)
    {
        lseek(server_info.file_handle, offset, SEEK_SET);
        read_size += offset;
    }

    for (;;)
    {
        memset(rec_buf, 0, sizeof(rec_buf));
        ret = read(server_info.file_handle, rec_buf, sizeof(rec_buf));
        if (ret < 0)
        {
            printf("read file error\n");
            goto err1;
        }
        else if (ret == 0)
        {
            ret = read_size;
            goto err1;
        }
        else if (ret > 0)
        {
            // printf("read_buf =%s\n",rec_buf);

            ret = write(server_info.data_sock, rec_buf, ret);
            if (ret < 0)
            {
                printf("Write failed\n");
                goto err1;
            }
            else
            {
                read_size += ret;
                // lseek(file_handle,read_size, SEEK_SET);
            }
        }
    }

err1:
    if (server_info.file_handle > 0)
        close(server_info.file_handle);
err0:
    if (server_info.client_server_sock > 0)
        close(server_info.client_server_sock);

    if (server_info.data_sock > 0)
        close(server_info.data_sock);
    memset(rec_buf, 0, sizeof(rec_buf));
    /* 客户端接收服务器的响应码和信息，正常为 ”226 Transfer complete.” */
    read(ctrl_sock, rec_buf, sizeof(rec_buf));
    printf("%s\n Up file end!!!\n", rec_buf);

    return ret;
}

static int close_st_info(FTP_DATA_INFO *info)
{
    if (info == NULL)
        return -1;

    if (info->file_handle > 0)
        close(info->file_handle);

    if (info->client_server_sock > 0)
        close(info->file_handle);

    if (info->data_sock > 0)
        close(info->file_handle);

    memset(info, 0, sizeof(FTP_DATA_INFO));
    return 0;
}

int get_fsize_ftpserver(int ctrl_sock, char *server_filepath_name)
{
    int ret = -1;

    if (server_filepath_name == NULL)
    {
        printf("argc is null\n");
        return -1;
    }
    /*发送上传文件命令*/
    ret = send_cmd(ctrl_sock, CMD_SIZE_FTP, server_filepath_name, ACK_SIZE_NUM);
    if (ret < 0)
    {
        printf("send SZIE failed\n");
        return -1;
    }
    // printf("file size =%d\n",ret);
    return ret;
}

/*
* @brief 从ftp服务器退出
* @param
  ctrl_sock 控制服务器sock
* @return -1/成功返回0
*/
int quit_fpt_server(int ctrl_sock)
{
    int ret = -1;
    close_st_info(&server_info);

    ret = send_cmd(ctrl_sock, CMD_QUIT, NULL, ACK_QUIT_NUM);
    if (ret < 0)
        printf("quit fpt server error\n");

    close(ctrl_sock);

    return ret;
}
