#ifndef __SIMPLE_FTP_UPLOADER_H__
#define __SIMPLE_FTP_UPLOADER_H__

#define FTP_SERVER_PORT 21

typedef enum
{
    CMD_USER = 0, /*用户名*/
    CMD_PASS,     /*密码*/
    CMD_PASV,     /*让服务器进入被动模式*/
    CMD_CWD,      /*切换工作目录*/
    CMD_SIZE_FTP, /*获取文件大小*/
    CMD_RETR,     /*下载文件*/
    CMD_REST,     /*指定下载文件的偏移量*/
    CMD_QUIT,     /*退出命令*/
    CMD_LIST,     /*列表*/
    CMD_STOR,     /*上传文件*/
    CMD_PORT_FTP, /*发送客户端端口给服务器*/
    CMD_MLSD,     /*列表*/

} eu_cmd_type;

void create_uploader_process(void);

int connect_ftp_server(const char *server_ip, const int port);

int login_ftp_server(int ctrl_sock, const char *user_name, const char *passwd);

int down_file_ftpserver(int ctrl_sock, char *server_filepath_name,
                        const char *newfilename, int connect_mode, int offset, eu_cmd_type typ);

int up_file_ftpserver(int ctrl_sock, char *server_filepath_name,
                      const char *srcfilename, int connect_mode, int offset);

int get_fsize_ftpserver(int ctrl_sock, char *server_filepath_name);

int quit_fpt_server(int ctrl_sock);
#endif // __SIMPLE_FTP_UPLOADER_H__

