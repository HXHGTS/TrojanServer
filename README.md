# TrojanServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

手动搭建Trojan服务器，并部署网页页面

### 一句话执行(CentOS7):
```
yum install -y gcc wget && wget https://github.com/HXHGTS/TrojanServer/raw/master/trojan.c -O trojan.c && chmod +x trojan.c && gcc -o trojan trojan.c && ./trojan
```

第一次点击安装后会自动升级系统内核并触发重启，重启后输入
```
./trojan
```
并选1继续安装

### 安装前准备：

1.一个域名，与服务器ip做好dns解析

2.该域名必须提前申请SSL证书用于加密（后续考虑加入自动申请证书），将证书(.cer/.crt/.pem)文件命名为1.pem，将私钥(.key/*.pem)文件命名为2.pem，放在/root目录下

3.脚本仅支持CentOS7系统，vps必须为KVM或XEN架构

4.脚本执行需要root权限登录系统，AWS等特殊机型打开root权限方法[看这里](https://hxhgts.icu/AWSECSRoot/)

5.请在服务器后台防火墙放行tcp80(http)tcp443(https)端口
