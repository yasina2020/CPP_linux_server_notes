# CPP_linux_server_notes





## linux和git同步方法
### 1、安装git
  

    sudo apt-get install git

### 2、配置信息
  

    git config --global user.name "用户名"
      git config --global user.email "邮箱地址"

### 3、生成ssh密钥
　　

    ssh-keygen -C 'you email address@gmail.com' -t rsa

　　会在用户目录~/.ssh/下建立相应的密钥文件。
### 4、创建完公钥后，需要上传。
 

     打开~/.ssh/id_rsa.pub文件，复制其中所有内容。
      访问github网页，新建SSH公钥.

### 5、创建仓库
  

    在github创建一个新的仓库

### 6、初始化本地项目
 

     git init //把这个目录变成Git可以管理的仓库

### 7、添加文件
 

     git add . //不但可以跟单一文件，还可以跟通配符，更可以跟目录。一个点就把当前目录下所有未追踪的文件全部add了 

### 8、提交
 

     git commit -m "first commit" //把文件提交到仓库

### 8、关联远程仓库
 

     git remote add origin git@github.com:wangjiax9/practice.git //关联远程仓库

### 9、推送
  

    git push -u origin master //把本地库的所有内容推送到远程库上


## linux 安装完整的man手册

1)、安装 C语言 库函数基本帮助文档:

$ sudo apt-get install libc-dev

$ sudo apt-get install glibc-doc

$ sudo apt-get install manpages

$ sudo apt-get install manpages-de

$ sudo apt-get install manpages-de-dev

$ sudo apt-get install manpages-dev

2)、安装 POSIX 函数帮助文档：

$ sudo apt-get install manpages-posix

$ sudo apt-get install manpages-posix-dev

3)、安装内核函数文档：

$ sudo apt-get install linux-doc

$ sudo apt-get install libcorelinux-dev

4)、安装 C++ 帮助文档:

$ sudo apt-get install libstdc++-7-dev

$ sudo apt-get install libstdc++-7-doc



当然!对于 manpage 可以直接一条命令：

$ sudo apt-get install manpages*


4、关于 manpage 颜色
1)、在家目录下的 .bashrc 中加入下面代码来实现调色：

export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_mb=$'\E[01;32m'
export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_md=$'\E[01;36m'   # man description
export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_me=$'\E[0m'
export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_so=$'\E[01;32m'
export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_se=$'\E[0m' 
export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_us=$'\E[01;32m'  # underly select  
export <font face = "Courier" size = 5 color = "#a52a2a">LESS_TERMCAP_ue=$'\E[0m' 

目前只知道 b 为 bold，o 为 option

2)、重新启动终端


