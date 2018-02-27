install
=======

## 用源安装

### 引入mosquitto仓库并更新

```
$sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
$sudo apt update
```

### 安装mosquitto包

```
$sudo apt install mosquitto
```

### 安装mosquitto开发包

```
$sudo apt install libmosquitto-dev
```

### 安装mosquitto客户端

```
$sudo apt install mosquitto-clients
```

### 查询mosquitto是否正确运行

```
$sudo service mosquitto status
```

## 用源码安装

### 下载地址

```
http://mosquitto.org/download/
https://github.com/eclipse/mosquitto/tree/master
```
> note: 我这里使用的是github上的源码

### 编译

* 下载源码

```
git clone https://github.com/eclipse/mosquitto/tree/master
cd mosquitto ; make
```

* 安装相关库

```
sudo apt install libc-ares-dev
sudo apt install uuid-dev
```

* 安装相关工具

```
sudo apt install xsltproc
sudo apt install docbook-xsl
```

## 交叉编译










## 测试

### 本地测试

* 订阅主题"mqtt"

```
./mosquitto_pub -h localhost -t "mqtt" -m "hello mqtt"
```
* 发布消息到主题"mqtt"

```
./mosquitto_sub -h localhost -t "mqtt" -v
```

### 网络测试

从`test.mosquitto.org`订阅主题

* 订阅所有主题

```
mosquitto_sub -h test.mosquitto.org -t "#" -v
```

* 订阅特定主题

```
./mosquitto_sub -h test.mosquitto.org -t "gnsyxiang-msg" -v
./mosquitto_pub -h test.mosquitto.org -t "gnsyxiang-msg" -m "hello mqtt"
```

> note: `test.mosquitto.org`支持加密和不加密MQTT消息模式，也支持用TCP或者Websocket作为承载，可以通过wireshark抓包来观察不同的包格式.





