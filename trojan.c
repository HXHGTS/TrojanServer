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
        system("sudo vi /usr/local/etc/trojan/config.json");
        system("systemctl start trojan");
        goto Menu;
    }
    else if (mode == 6) {
        DNS_Change();
        goto Menu;
    }
    else if (mode == 7) {
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
    printf("安装前请先去阿里云或腾讯云申请域名与域名对应的SSL证书，否则会导致安装失败！\n");
    printf("-----------------------------------------------------------\n");
    printf("----------------------当前Kernel版本-----------------------\n");
    system("uname -sr");
    printf("-----------------------------------------------------------\n");
    printf("1.安装trojan\n2.运行trojan\n3.显示二维码与配置链接\n4.查看服务器配置\n5.修改服务器配置\n6.修改DNS服务器\n7.关闭trojan\n0.退出\n");
    printf("-----------------------------------------------------------\n");
    printf("请输入:");
    scanf("%d", &mode);
    return 0;
}

int install_trojan() {
    KernelUpdate();
    system("setenforce 0");
    system("yum install -y curl pwgen qrencode unzip epel-release nginx");
    system("wget https://raw.githubusercontent.com/trojan-gfw/trojan-quickstart/master/trojan-quickstart.sh -O /root/trojan-quickstart.sh");
    system("chmod +x trojan-quickstart.sh");
    system("bash trojan-quickstart.sh");
    printf("请输入已绑定此服务器ip的域名:");
    scanf("%s", sni);
    printf("请将pem或cer或crt格式的证书用记事本打开并将内容粘贴至弹出页. . .\n");
    printf("弹出页按i开始编辑，结束编辑先按ESC再按:wq以保存\n");
    system("sleep 5");
    system("sudo vi /usr/local/etc/trojan/certificate.crt");
    printf("请将key格式的私钥用记事本打开并将内容粘贴至弹出页. . .\n");
    printf("弹出页按i开始编辑，结束编辑先按ESC再按:wq以保存\n");
    system("sleep 5");
    system("sudo vi /usr/local/etc/trojan/private.key");
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
    system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/html.zip > /usr/share/nginx/html/html.zip");
    system("unzip -o /usr/share/nginx/html/html.zip -d /usr/share/nginx/html");
    system("rm -f /usr/share/nginx/html/html.zip");
    printf("正在启动nginx并将nginx写入开机引导项. . .\n");
    system("systemctl enable nginx && systemctl start nginx");
    config = fopen("/usr/local/etc/trojan/client.conf", "w");
    fprintf(config, "trojan://%s@%s:443", passwd,sni);
    fclose(config);
    printf("正在配置防火墙. . .\n");
    system("yum install -y firewalld");
    system("ufw disable");
    system("systemctl enable firewalld && systemctl start firewalld");
    system("firewall-cmd --permanent --add-service=https");
    system("firewall-cmd --permanent --add-service=http");
    system("firewall-cmd --reload");
    printf("正在启动trojan并将trojan写入开机引导项. . .\n");
    system("systemctl enable trojan && systemctl start trojan");
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

int DNS_Change() {
    int dns;
    char ns1[16], ns2[16],command[50];
    printf("请选择想要设定的DNS:\n\n1.Google\n\n2.Cloudflare\n\n3.Opendns\n\n4.Quad9\n\n5.Level3\n\n请输入:");
    scanf("%d", &dns);
    if (dns == 1) {
        sprintf(ns1, "8.8.8.8");
        sprintf(ns2, "8.8.4.4");
    }
    else if (dns == 2) {
        sprintf(ns1, "1.1.1.1");
        sprintf(ns2, "1.0.0.1");
    }
    else if (dns == 3) {
        sprintf(ns1, "208.67.222.222");
        sprintf(ns2, "208.67.220.220");
    }
    else if (dns == 4) {
        sprintf(ns1, "9.9.9.9");
        sprintf(ns2, "149.112.112.9");
    }
    else {
        sprintf(ns1, "4.2.2.1");
        sprintf(ns2, "4.2.2.2");
    }
    sprintf(command, "echo \"nameserver %s\" > /etc/resolv.conf", ns1);
    system(command);
    sprintf(command, "echo \"nameserver %s\" >> /etc/resolv.conf", ns2);
    system(command);
    system("sed -e '/DNS1=/d;/DNS2=/d;/DNS3=/d' /etc/sysconfig/network-scripts/ifcfg-eth0 > /etc/sysconfig/network-scripts/ifcfg-eth0.temp");
    system("cp -rf /etc/sysconfig/network-scripts/ifcfg-eth0.temp /etc/sysconfig/network-scripts/ifcfg-eth0");
    system("rm -rf /etc/sysconfig/network-scripts/ifcfg-eth0.temp");
    sprintf(command, "echo \"DNS1=%s\" >> /etc/sysconfig/network-scripts/ifcfg-eth0", ns1);
    system(command);
    sprintf(command, "echo \"DNS2=%s\" >> /etc/sysconfig/network-scripts/ifcfg-eth0", ns2);
    system(command);
    system("service network restart");
    printf("DNS服务器修改成功!\n");
    return 0;
}