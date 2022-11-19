# CPP_linux_server_notes

linux和git同步方法
1、安装git
  sudo apt-get install git
2、配置信息
  git config --global user.name "用户名"
  git config --global user.email "邮箱地址"
3、生成ssh密钥
　　ssh-keygen -C 'you email address@gmail.com' -t rsa
　　会在用户目录~/.ssh/下建立相应的密钥文件。
4、创建完公钥后，需要上传。
  使用命令cd ~/.ssh进入~/.ssh文件夹，输入
  cat id_rsa.pub
  打开id_rsa.pub文件，复制其中所有内容。
  接着访问github网页，新建SSH公钥，标题栏名称自定义，公钥栏把刚才复制的内容粘贴进去。
5、创建仓库
  在github创建一个新的仓库
6、初始化本地项目
  git init //把这个目录变成Git可以管理的仓库
7、添加文件
  git add . //不但可以跟单一文件，还可以跟通配符，
            //更可以跟目录。一个点就把当前目录下所有未追踪的文件全部add了 
8、提交
  git commit -m "first commit" //把文件提交到仓库
8、关联远程仓库
  git remote add origin git@github.com:wangjiax9/practice.git //关联远程仓库
9、推送
  git push -u origin master //把本地库的所有内容推送到远程库上
