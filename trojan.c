﻿#include <stdio.h>
#include <stdlib.h>


FILE* config;
char passwd[30],sni[30];
int mode;

int main(){
    system("clear");
Menu:UI();
    system("clear");
    if (mode == 1) {
        install_trojan();
        goto Menu;
    }
    else if (mode == 2) {
        system("systemctl stop trojan");
        system("systemctl start trojan");
        system("ss -lp | grep trojan");
        system("sleep 3");
        goto Menu;
    }
    else if (mode == 3) {
        printf("手机trojan客户端请扫描二维码添加:\n\n");
        system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
        system("sleep 5");
        printf("\ntrojan链接（可用于生成Clash(R)的配置）:\n\n");
        system("cat /usr/local/etc/trojan/client.conf");
        printf("\n\n");
        goto Menu;
    }
    else if (mode == 4) {
        system("cat /usr/local/etc/trojan/config.json");
        goto Menu;
    }
    else if (mode == 5) {
        system("systemctl stop trojan");
        system("vi /usr/local/etc/trojan/config.json");
        system("systemctl start trojan");
        goto Menu;
    }
    else if (mode == 6) {
        system("systemctl stop trojan");
        goto Menu;
    }
    else {
        exit(0);
    }
    return 0;
}

int UI() {
    printf("-----------------------------------------------------------\n");
    printf("----------------------trojan安装工具-----------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("安装前或需要更新SSL证书，请将证书(.cer/.crt/.pem)与私钥(.key/.pem)分别命名为1.pem与2.pem，上传至服务器/root目录");
    printf("-----------------------------------------------------------\n");
    printf("----------------------当前Kernel版本-----------------------\n");
    system("uname -sr");
    printf("-----------------------------------------------------------\n");
    printf("1.安装trojan\n2.运行trojan\n3.显示二维码与配置链接\n4.查看服务器配置\n5.修改服务器配置\n6.关闭trojan\n0.退出\n");
    printf("-----------------------------------------------------------\n");
    printf("请输入:");
    scanf("%d", &mode);
    return 0;
}

int install_trojan() {
    KernelUpdate();
    printf("请输入已绑定此服务器ip的域名:");
    scanf("%s", sni);
    system("setenforce 0");
    system("yum install -y curl pwgen qrencode unzip epel-release nginx");
    system("wget https://raw.githubusercontent.com/trojan-gfw/trojan-quickstart/master/trojan-quickstart.sh -O /root/trojan-quickstart.sh");
    system("chmod +x trojan-quickstart.sh");
    system("bash trojan-quickstart.sh");
    system("sleep 3");
    system("rm -rf trojan-quickstart.sh");
    system("rm -rf TCPO.sh");
    system("rm -rf KernelUpdate.sh");
    system("cp -rf /root/1.pem /usr/local/etc/trojan/certificate.crt");
    system("cp -rf /root/2.pem /usr/local/etc/trojan/private.key");
    system("rm -rf /root/1.pem");
    system("rm -rf /root/2.pem");
    printf("正在生成配置文件. . .\n");
    system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.conf.1 > /usr/local/etc/trojan/config.json");
    printf("正在生成强密码. . .\n");
    system("pwgen -s 28 1 > /usr/local/etc/trojan/passwd.conf");
    config = fopen("/usr/local/etc/trojan/passwd.conf", "r");
    fscanf(config, "%s",passwd);
    fclose(config);
    config = fopen("/usr/local/etc/trojan/config.json", "a");
    fprintf(config, "        \"%s\"\n", passwd);
    fclose(config);
    system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.conf.2 >> /usr/local/etc/trojan/config.json");
    printf("正在配置html网页. . .\n");
    system("wget https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/html.zip -O /usr/share/nginx/html/html.zip");
    system("unzip -o /usr/share/nginx/html/html.zip -d /usr/share/nginx/html");
    system("rm -f /usr/share/nginx/html/html.zip");
    printf("正在启动nginx并将nginx写入开机引导项. . .\n");
    system("systemctl enable nginx && systemctl start nginx");
    config = fopen("/usr/local/etc/trojan/client.conf", "w");
    fprintf(config, "trojan://%s@%s:443", passwd,sni);
    fclose(config);
    printf("正在启动trojan并将trojan写入开机引导项. . .\n");
    system("systemctl enable trojan");
    system("systemctl start trojan");
    printf("正在验证trojan启动，不为空则启动成功. . .\n");
    system("ss -lp | grep trojan");
    printf("trojan部署完成！\n");
    printf("手机trojan客户端请扫描二维码添加:\n\n");
    system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
    system("sleep 5");
    printf("\ntrojan链接（可用于生成Clash(R)的配置）:\n\n");
    system("cat /usr/local/etc/trojan/client.conf");
    printf("\n\n");
    return 0;
}

int KernelUpdate() {
    if ((fopen("KernelUpdate.sh", "r")) == NULL) {
        printf("正在升级新内核. . .\n");
        system("wget https://github.com/HXHGTS/TCPOptimization/raw/master/KernelUpdate.sh");
        system("chmod +x KernelUpdate.sh");
        printf("正在升级，将自动触发重启以应用配置. . .\n");
        system("bash KernelUpdate.sh");
    }
    else {
        system("wget https://github.com/HXHGTS/TCPOptimization/raw/master/TCPO.sh");
        system("chmod +x TCPO.sh");
        system("bash TCPO.sh");
    }
    return 0;
}
