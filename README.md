# TrojanServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

手动搭建Trojan服务器，并部署网页页面

### 安装:

KVM/XEN机器执行:
`yum install -y gcc wget && wget https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan.c -O trojan.c && gcc -o trojan trojan.c && ./trojan`

OpenVZ机器执行:
`yum install -y gcc wget && wget https://raw.githubusercontent.com/HXHGTS/TrojanServer/master/trojan_no_bbr.c -O trojan.c && gcc -o trojan trojan.c && ./trojan`

第一次点击安装后会自动升级系统内核并触发重启，重启后输入

`./trojan`

并选1继续安装

### 安装前准备：

1.一个域名，与服务器ip做好dns解析

2.该域名必须提前申请SSL证书用于加密（后续考虑加入自动申请证书），将证书(.cer/.crt/.pem)文件命名为1.pem，将私钥(.key/*.pem)文件命名为2.pem，放在/root目录下(SSL证书申请:[腾讯云](https://console.cloud.tencent.com/ssl) [阿里云](https://common-buy.aliyun.com/?spm=5176.b5912525.0.0.3c07GExwGExwfv&commodityCode=cas) [FreeSSL](https://freessl.cn/))

3.脚本仅支持CentOS7系统

4.脚本执行需要root权限登录系统，AWS等特殊机型打开root权限方法[看这里](https://hxhgts.ml/AWSECSRoot/)

5.请在服务器后台防火墙放行tcp80(http)tcp2053(knetd)端口
