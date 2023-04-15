

# docker

- 镜像：相当于一个root文件系统。比如官方镜像ubuntu：16.04就包含了完整的一套ubuntu16.04最小系统的root文件系统。
- 容器：镜像和容器的关系就像是面向对象程序设计中的类和对象一样，镜像是静态的定义，容器是镜像运行的实体。容器可以被创建、启动、停止、删除、暂停等。
- 仓库：仓库可看成一个代码控制中心，用来保存镜像。



操作镜像和操作容器

## docker 服务相关命令



- 启动docker

  ```bash
  $ sudo systemctl start docker
  ```

- 停止

  ```bash
  $ sudo systemctl stop docker
  ```

- 重启

  ```bash
  $ sudo systemctl restart docker
  ```

- 查看服务状态	

  ```
  $ sudo systemctl status docker
  ```

- 设置开机启动

  ```bash
  $ sudo systemctl enable docker
  ```

  

​			

## docker 镜像相关命令

- 查看镜像

  ```bash
  $ docker images		
  #TAG 版本号
  #created  创建的时间
  ```

- 搜索镜像

  ```bash
  $ docker serch redis
  $ docker imahes -q
  #查看所有镜像id
  ```

- 拉取镜像

  ```bash
  $ docker pull redis 
  #不写版本号时， 默认最新
  $ docker pull redis:5.0
  ```

- 删除镜像

  ```bash 
  $ docker rmi 33e3db53b328
  # rmi  后＋ IMAGES ID
  $ docker rmi redis:5.0
  # docker rmi REPOSITORY:TAG
  $ docker rmi `docker imahes -q`
  #删除所有镜像			
  ```

## docker 容器相关命令

- 查看容器

  ```bash
  # 查看正在运行的容器
  $ docker ps
  #查看所有容器
  $ docker ps -a
  ```

- 创建容器

  ```bash
  $ docker run -i -t --name=c1 ubuntu:22.04 /bin/bash
  #i 一直运行 t分配终端
  #--name=自己想用的名字
  #ubuntu:22.04  打开的镜像  --基础镜像
  #/bin/bash   
  $ docker run -id --name=c2 ubuntu:22.04 
  # -d  后台运行  不会exit后立即关闭
  # 以守护模式运行容器
  ```

  ![image-20230414135639219](/home/ly/.config/Typora/typora-user-images/image-20230414135639219.png)

```bash
#退出容器
$ exit 
####注意
#当使用docker run -it --name=c1 ubuntu:22.04 /bin/bash  时
#exit 后 容器自动关闭

# -it  --创建的容器，一般称为交互式容器
# -id  -- 创建的容器，一般称为守护式容器
```

​									

- 进入容器

  ```bash
  $ docker exec -it c2 /bin/bash
  #c2  --容器名字
  
  ```

- 启动容器

  ```bash
  $ docker start 容器名称
  ```

  

- 停止容器

  ```bash
  $ docker stop c2
  ```

- 删除容器

  ```bash
  $ docker rm c1
  #删除所有容器，无法删除正在运行的容器
  $docker rm `docker ps -aq`
  ```

  

- 查看容器信息

  ```bash
  #查看所有容器id信息
  $ docker ps -aq
  
  $ docker inspect c2
  # c2  --容器名字
  
  ```

## Docker 容器的数据卷



![image-20230414142923926](/home/ly/.config/Typora/typora-user-images/image-20230414142923926.png)



![image-20230414143127727](/home/ly/.config/Typora/typora-user-images/image-20230414143127727.png)

**数据卷**

- 是宿主机中的一个目录或文件
- 当容器目录和数据卷目录绑定后，对方的修改会立即同步
- 一个数据卷可以被多个容器挂载

**作用**

- 容器数据持久化
- 外部机器和容器间的通信
- 容器之间数据交换



### 配置数据卷

- 创建启动容器时，使用-v参数，设置数据卷

  ```bash
  $ docker run ... -v 宿主机目录（文件）:容器内目录(文件) ...
  
  # 宿主机目录（文件）:容器内目录(文件)  ---冒号间不能有空格
  ```

- 注意事项：

  1. 目录必须是绝对路径
  2. 如果目录不存在，会自动创建
  3. 可以挂载多个数据卷

- example

  ```bash
  $ docker run -it --name=c2 -v /root/snap/docker/:/root/data_container ubuntu:22.04  /bin/bash
  ```

  1. 挂载多个目录

     ```bash
     $ docker run -it --name=c1 -v ~/data:/root/data -v ~/data2:/root/data2 ubuntu:22.04 /bin/bash
     ```

  2. 实现两个容器间通信

     ```bash
     #c2
      docker run -it --name=c2 -v ~/data:/root/data ubuntu:22.04 /bin/bash
     #c3
      docker run -it --name=c3 -v ~/data:/root/data ubuntu:22.04 /bin/bash
      
      ###公用卷--- ~/data
     ```

     ![image-20230414150441374](/home/ly/.config/Typora/typora-user-images/image-20230414150441374.png)



### 数据卷容器

多容器进行数据交换 

1. 多个容器挂载同一个数据卷
2. 数据卷容器

![image-20230414151410094](/home/ly/.config/Typora/typora-user-images/image-20230414151410094.png)

#### 配置数据卷容器

1. 创建启动c3数据卷容器，使用-v 参数 设置数据卷

   ```bash
   docker run -it -name=c3 -v /volume ubuntu:22.04 /bin/bash
   ```

2. 创建启动c1  c2 容器 使用 --volumes-from 参数 设置数据卷

   ```bash
   docker run -it -name=c1  --volume-from c3 ubuntu:22.04 /bin/bash
   
   docker run -it -name=c2  --volume-from c3 ubuntu:22.04 /bin/bash
   ```

   

## docker 应用部署

![image-20230414190646775](/home/ly/.config/Typora/typora-user-images/image-20230414190646775.png)

**端口映射**

部署MySQL

1. 搜索镜像
2. 拉取镜像
3. 创建荣区，设置端口映射、目录映射

```bash
#在/root 目录下创建MySQL目录用于存储mysql信息
mkdir ~/mysql
cd ~/mysql
```

```bash
docker run -id \
-p 3307:3306 \
--name=c_mysql \
-v $pwd/conf:/ect/mysql/conf.d \
-v $pwd/logs:/logs \
-v $pwd/data:/var/lib/mysql \
-e MYSQL_ROOT_PASSWORD=123456 \
mysql:5.6


##docker: Error response from daemon: error while creating mount source path '/logs': mkdir /logs: read-only file system.

##使用  $pwd 时 会报错  应该使用绝对路径

#ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (2)

```

- 参数说明
  - -p 3307:3306 ：将容器的3306端口映射到宿主机的3307端口
  - -v $pwd/conf:/ect/mysql/conf.d ： 将主机当前目录下的conf/my.cnf挂载到容器的/ect/mysql/conf.d 
  - -v $pwd/logs:/logs  : 将主机当前目录下的logs目录挂载到容器的/logs. 日志目录
  - -v $pwd/data:/var/lib/mysql : 将主机当前目录下的data目录挂载到容器的 /var/lib/mysql。数据目录
  - -e MYSQL_ROOT_PASSWORD=123456 ： 初始化root用户的密码

## Dockerfile

### Docker镜像原理

![image-20230415151735620](/home/ly/.config/Typora/typora-user-images/image-20230415151735620.png)

1. 一个分层文件系统
2. Cento的ios镜像包含bootfs和rootfs，而docker的centos镜像服用操作系统的bootfs，只有roots和其他镜像层
3. 由于docker中镜像是分层的，tomcat虽然只有70多MB，但他需要依赖于父镜像和基础镜像，所以整个对外暴露的Tomcat镜像大小500多MB。



![image-20230415152007269](/home/ly/.config/Typora/typora-user-images/image-20230415152007269.png)

- Docker镜像特殊的文件系统叠加而成
- 最低端是bootfs,并使用宿主机的bootfs
- 第二层是root文件系统rootfs，称为base image
- 然后再往上可以叠加其他镜像文件
- 统一文件系统技术能够将不同层整合成一个文件系统，为这些层提供一个统一的视角，这样就隐藏了多层的存在，在用户角度看来，只存在一个文件系统。
- 一个镜像可以放在另一个镜像上面，位于下面的镜像称为父镜像，最底层的镜像称为基础镜像。
- 当从一个镜像启动容器时，Docker会在最顶层加载一个读写文件系统作为容器
- ![image-20230415152646112](/home/ly/.config/Typora/typora-user-images/image-20230415152646112.png)

### 镜像制作

#### 容器转为镜像

```bash
docker commit 容器id 镜像名称:版本号
```

​		

```bash
docker save -o 压缩文件名称 镜像名称:版本号
docker load -i 压缩文件名称

#使用压缩的镜像
docker load -i 镜像的压缩文件
docker run ...
```

![image-20230415154520452](/home/ly/.config/Typora/typora-user-images/image-20230415154520452.png)

#### Dockerfile

- Dockerfile是一个文本文件
- 包含了一条条指令
- 每一条指令构建一层，基于基础镜像，最终构建出一个新的镜像。

```bash
docker build -f 
```

