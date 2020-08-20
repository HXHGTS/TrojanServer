# TrojanServer
搭建Trojan服务器

### 一句话执行(CentOS7):
```
sudo yum install -y gcc wget && wget https://github.com/HXHGTS/TrojanServer/raw/master/trojan.c -O trojan.c && sudo chmod +x trojan.c && gcc -o trojan trojan.c && sudo ./trojan
```

第一次点击安装后会自动升级系统内核并触发重启，重启后输入
```
sudo ./WireGuard
```
并选1继续安装

### 安装前准备：

1.一个域名，与服务器ip做好dns解析

2.该域名必须提前申请SSL证书用于加密（后续考虑加入自动申请证书）

3.脚本仅支持CentOS7系统
