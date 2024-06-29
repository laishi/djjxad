#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080
#define WEB_ROOT "./website"

void handle_request(int client_socket) {
    char buffer[1024] = {0};
    char response_header[1024] = {0};
    int valread;

    valread = read(client_socket, buffer, 1024);
    printf("%s\n", buffer);

    // 解析HTTP请求，获取请求路径
    char method[10];
    char path[255];
    sscanf(buffer, "%s %s", method, path);

    // 构建完整的文件路径
    char file_path[265];
    sprintf(file_path, "%s%s", WEB_ROOT, path);

    // 默认首页处理
    if (strcmp(path, "/") == 0) {
        sprintf(file_path, "%s/index.html", WEB_ROOT);
    }

    // 打开文件
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        // 文件不存在，返回404
        sprintf(response_header, "HTTP/1.1 404 Not Found\r\n\r\n");
        send(client_socket, response_header, strlen(response_header), 0);
        printf("404 Not Found: %s\n", file_path);
    } else {
        // 根据文件扩展名设置Content-Type
        char *ext = strrchr(file_path, '.');
        char *content_type = "text/html"; // 默认为HTML

        if (ext != NULL) {
            if (strcmp(ext, ".js") == 0) {
                content_type = "text/javascript";
            } else if (strcmp(ext, ".css") == 0) {
                content_type = "text/css";
            } else if (strcmp(ext, ".json") == 0) {
                content_type = "application/json";
            } else if (strcmp(ext, ".png") == 0) {
                content_type = "image/png";
            } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
                content_type = "image/jpeg";
            } else if (strcmp(ext, ".gif") == 0) {
                content_type = "image/gif";
            }
        }

        sprintf(response_header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);
        send(client_socket, response_header, strlen(response_header), 0);

        // 发送文件内容
        int bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
        }

        fclose(file);
        printf("200 OK: %s\n", file_path);
    }

    // 关闭连接
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置socket选项，允许重用本地地址和端口
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on http://8.137.120.53:%d\n", PORT);

    // 接受和处理连接
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // 处理请求
        handle_request(new_socket);
    }

    return 0;
}
