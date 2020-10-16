#include <stdio.h>
#include <stdlib.h>


FILE* config;
char passwd[30],sni[30];
int mode;

int main(){
Menu:UI();
    system("clear");
    if (mode == 1) {
        install_trojan();
        goto Menu;
    }
    else if (mode == 2) {
        system("systemctl stop trojan");
        system("systemctl start trojan");
        printf("正在验证trojan启动，不为空则启动成功. . .\n");
        system("ss -lp | grep trojan");
        goto Menu;
    }
    else if (mode == 3) {
        printf("手机trojan客户端请扫描二维码添加:\n\n");
        system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
        printf("\ntrojan链接:\n\n");
        system("cat /usr/local/etc/trojan/client.conf");
        printf("\nClash配置:\n\n");
        system("cat /usr/local/etc/trojan/clash.json");
        printf("\n\n");
        goto Menu;
    }
    else if (mode == 4) {
        system("systemctl stop trojan");
        system("vi /usr/local/etc/trojan/config.json");
        system("systemctl start trojan");
        goto Menu;
    }
    else if (mode == 5) {
        if (fopen("/root/1.pem", "r") == NULL || fopen("/root/2.pem", "r") == NULL) {
            printf("检测到证书与私钥文件未按照规定方式放置于根目录，强制退出！\n");
            exit(0);
        }
        printf("请输入已绑定此服务器ip的新域名:");
        scanf("%s", sni);
        config = fopen("/usr/local/etc/sni.conf", "w");
        fprintf(config, "%s", sni);
        fclose(config);
        system("cp -rf /root/1.pem /usr/local/etc/trojan/certificate.crt");
        system("cp -rf /root/2.pem /usr/local/etc/trojan/private.key");
        printf("正在生成配置文件. . .\n");
        system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.conf.1 > /usr/local/etc/trojan/config.json");
        printf("正在生成强密码. . .\n");
        system("pwgen -s 28 1 > /usr/local/etc/trojan/passwd.conf");
        config = fopen("/usr/local/etc/trojan/passwd.conf", "r");
        fscanf(config, "%s", passwd);
        fclose(config);
        config = fopen("/usr/local/etc/trojan/config.json", "a");
        fprintf(config, "        \"%s\"\n", passwd);
        fclose(config);
        system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.conf.2 >> /usr/local/etc/trojan/config.json");
        printf("正在配置html网页. . .\n");
        system("wget https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/html.zip -O /usr/share/nginx/html/html.zip");
        system("unzip -o /usr/share/nginx/html/html.zip -d /usr/share/nginx/html");
        system("rm -f /usr/share/nginx/html/html.zip");
        printf("正在重启nginx. . .\n");
        system("systemctl restart nginx");
        config = fopen("/usr/local/etc/trojan/client.conf", "w");
        fprintf(config, "trojan://%s@%s:443", passwd, sni);
        fclose(config);
        printf("正在重启trojan. . .\n");
        system("systemctl restart trojan");
        printf("正在验证trojan启动，不为空则启动成功. . .\n");
        system("ss -lp | grep trojan");
        printf("trojan部署完成！\n");
        config = fopen("/usr/local/etc/trojan/clash.json", "w");
        fprintf(config, "  - {name: %s, server: %s, port: 443, type: trojan, password: %s, udp: true}", sni, sni, passwd);
        fclose(config);
        printf("手机trojan客户端请扫描二维码添加:\n\n");
        system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
        printf("\ntrojan链接:\n\n");
        system("cat /usr/local/etc/trojan/client.conf");
        printf("\nClash配置:\n\n");
        system("cat /usr/local/etc/trojan/clash.json");
        printf("\n\n");
        goto Menu;
    }
    else if (mode == 6) {
        system("systemctl stop trojan");
        printf("正在更新trojan. . .\n");
        system("wget https://raw.githubusercontent.com/trojan-gfw/trojan-quickstart/master/trojan-quickstart.sh -O /root/trojan-quickstart.sh");
        system("echo \"n\nn\n\" | bash trojan-quickstart.sh");
        system("rm -f trojan-quickstart.sh");
        system("systemctl start trojan");
        printf("正在验证trojan启动，不为空则启动成功. . .\n");
        system("ss -lp | grep trojan");
        goto Menu;
    }
    else if (mode == 7) {
        system("systemctl stop trojan");
        system("systemctl disable trojan");
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
    printf("安装前或需要更新SSL证书，请将证书(.cer/.crt/.pem)与私钥(.key/.pem)分别命名为1.pem与2.pem，上传至服务器/root目录\n");
    printf("-----------------------------------------------------------\n");
    printf("----------------------当前Kernel版本-----------------------\n");
    system("uname -sr");
    printf("-----------------------------------------------------------\n");
    printf("1.安装trojan\n2.运行trojan\n3.显示二维码与配置链接\n4.修改服务器配置\n5.更新域名与SSL证书\n6.更新trojan\n7.关闭trojan\n0.退出\n");
    printf("-----------------------------------------------------------\n");
    printf("请输入:");
    scanf("%d", &mode);
    return 0;
}

int install_trojan() {
    KernelUpdate();
    config = fopen("/usr/local/etc/sni.conf", "r");
    fscanf(config, "%s", sni);
    fclose(config);
    system("setenforce 0");
    system("yum install -y curl pwgen qrencode unzip bind-utils epel-release nginx");
    system("wget https://raw.githubusercontent.com/trojan-gfw/trojan-quickstart/master/trojan-quickstart.sh -O /root/trojan-quickstart.sh");
    system("chmod +x trojan-quickstart.sh");
    system("bash trojan-quickstart.sh");
    system("sleep 3");
    system("rm -f trojan-quickstart.sh");
    system("rm -f TCPO.sh");
    system("cp -f /root/1.pem /usr/local/etc/trojan/certificate.crt");
    system("cp -f /root/2.pem /usr/local/etc/trojan/private.key");
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
    system("setsebool -P httpd_can_network_connect 1");
    config = fopen("/usr/local/etc/trojan/client.conf", "w");
    fprintf(config, "trojan://%s@%s:443", passwd,sni);
    fclose(config);
    printf("正在启动trojan并将trojan写入开机引导项. . .\n");
    system("systemctl enable trojan");
    system("systemctl start trojan");
    printf("正在验证trojan启动，不为空则启动成功. . .\n");
    system("ss -lp | grep trojan");
    printf("trojan部署完成！\n");
    config = fopen("/usr/local/etc/trojan/clash.json", "w");
    fprintf(config, "  - {name: %s, server: %s, port: 443, type: trojan, password: %s, udp: true}",sni,sni, passwd);
    fclose(config);
    printf("手机trojan客户端请扫描二维码添加:\n\n");
    system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
    printf("\ntrojan链接:\n\n");
    system("cat /usr/local/etc/trojan/client.conf");
    printf("\nClash配置:\n\n");
    system("cat /usr/local/etc/trojan/clash.json");
    printf("\n\n");
    return 0;
}

int KernelUpdate() {
    if ((fopen("KernelUpdate.sh", "r")) == NULL) {
        if (fopen("/root/1.pem", "r") == NULL || fopen("/root/2.pem", "r") == NULL) {
        printf("检测到证书与私钥文件未按照规定方式放置于根目录，强制退出！\n");
        exit(0);
    }
        printf("请输入已绑定此服务器ip的域名:");
        scanf("%s", sni);
        config = fopen("/usr/local/etc/sni.conf", "w");
        fprintf(config, "%s", sni);
        fclose(config);
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
