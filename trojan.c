#include <stdio.h>
#include <stdlib.h>

FILE* config;
char passwd[30],sni[30];
int mode;

int main(){
    Menu:UI();
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
        system("cat /usr/local/etc/trojan/config.json");
        goto Menu;
    }
    else if (mode == 4) {
        system("systemctl stop trojan");
        system("vi /usr/local/etc/trojan/config.json");
        system("systemctl start trojan");
        goto Menu;
    }
    else if (mode == 5) {
        printf("�ֻ�trojan�ͻ�����ɨ���ά�����:\n");
        system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
        printf("trojan���ӣ�����������Clash(R)�����ã�:\n");
        system("cat /usr/local/etc/trojan/client.conf");
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
    system("clear");
    printf("-----------------------------------------------------------\n");
    printf("----------------------trojan��װ����-----------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("��װǰ����ȥ�����ƻ���Ѷ������������������Ӧ��SSL֤�飬����ᵼ�°�װʧ�ܣ�\n");
    printf("-----------------------------------------------------------\n");
    printf("1.��װtrojan\n2.����trojan\n3.�鿴����������\n4.�޸ķ���������\n5.��ʾ��ά������������\n6.�ر�trojan\n0.�˳�\n");
    printf("-----------------------------------------------------------\n");
    printf("������:");
    scanf("%d", &mode);
    return 0;
}

int install_trojan() {
    KernelUpdate();
    system("setenforce 0");
    system("yum install -y curl pwgen qrencode epel-release nginx firewalld");
    system("wget https://raw.githubusercontent.com/trojan-gfw/trojan-quickstart/master/trojan-quickstart.sh -O /root/trojan-quickstart.sh");
    system("chmod +x trojan-quickstart.sh");
    system("bash trojan-quickstart.sh");
    printf("�������Ѱ󶨴˷�����ip������:");
    scanf("%s", sni);
    printf("�뽫pem��cer��crt��ʽ��֤���ü��±��򿪲�������ճ��������ҳ. . .\n");
    printf("����ҳ��i��ʼ�༭�������༭�Ȱ�ESC�ٰ�:wq�Ա���\n");
    system("sleep 5");
    system("vi /usr/local/etc/trojan/certificate.crt");
    printf("�뽫key��ʽ��˽Կ�ü��±��򿪲�������ճ��������ҳ. . .\n");
    printf("����ҳ��i��ʼ�༭�������༭�Ȱ�ESC�ٰ�:wq�Ա���\n");
    system("sleep 5");
    system("vi /usr/local/etc/trojan/private.key");
    printf("�������������ļ�. . .\n");
    system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.conf.1 > /usr/local/etc/trojan/config.json");
    printf("��������ǿ����. . .\n");
    system("pwgen -s 28 1 > /usr/local/etc/trojan/passwd.conf");
    config = fopen("/usr/local/etc/trojan/passwd.conf", "r");
    fscanf(config, "%s",passwd);
    fclose(config);
    config = fopen("/usr/local/etc/trojan/config.json", "a");
    fprintf(config, "        \"%s\"\n", passwd);
    fclose(config);
    system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.conf.2 >> /usr/local/etc/trojan/config.json");
    printf("��������trojan����trojanд�뿪��������. . .\n");
    system("systemctl enable trojan && systemctl start trojan");
    printf("������֤trojan��������Ϊ���������ɹ�. . .\n");
    system("ss -lp | grep trojan");
    printf("��������nginx����nginxд�뿪��������. . .\n");
    system("systemctl enable nginx && systemctl start nginx");
    printf("�������÷���ǽ. . .\n");
    system("systemctl enable firewalld && systemctl start firewalld");
    system("firewall-cmd --permanent --add-service=https");
    system("firewall-cmd --permanent --add-service=http");
    system("firewall-cmd --reload");
    printf("��������html��ҳ. . .\n");
    system("curl https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/index.html > /usr/share/nginx/html/index.html");
    config = fopen("/usr/local/etc/trojan/client.conf", "w");
    fprintf(config, "trojan://%s@%s:443", passwd,sni);
    fclose(config);
    printf("trojan������ɣ�\n");
    printf("�ֻ�trojan�ͻ�����ɨ���ά�����:\n");
    system("qrencode -t ansiutf8 < /usr/local/etc/trojan/client.conf");
    printf("trojan���ӣ�����������Clash(R)�����ã�:\n");
    system("cat /usr/local/etc/trojan/client.conf");
    return 0;
}

int KernelUpdate() {
    if ((fopen("KernelUpdate.sh", "r")) == NULL) {
        printf("�����������ں�. . .\n");
        system("wget https://github.com/HXHGTS/TCPOptimization/raw/master/KernelUpdate.sh");
        system("chmod +x KernelUpdate.sh");
        printf("�������������Զ�����������Ӧ������. . .\n");
        system("bash KernelUpdate.sh");
    }
    else {
        system("wget https://github.com/HXHGTS/TCPOptimization/raw/master/TCPO.sh");
        system("chmod +x TCPO.sh");
        system("bash TCPO.sh");
    }
    return 0;
}