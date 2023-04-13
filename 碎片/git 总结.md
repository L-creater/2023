# git 总结

1. git init

2. 生成ssh

   1. ```bash
      $ ssh-keygen -t ed25519 -C "1814666859@qq.com"
      #如果您使用的是不支持 Ed25519 算法的旧系统，请使用：
      
      $ ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
      ```

      

3. // 配置user.name
   git config --global user.name "your user name"
   // 配置user.email
   git config --global user.email "your user email"
   // 查看配置
   git config --list

   ```
   git config --global user.name "L-creater"
   git config --global user.email "1814666859@qq.com"
   
   
   git push -u origin "master"
   ```

   

4. git remote 

   1. `git remote add origin https://gitee.com/L-creater/cv.git`

      你可以在命令行中使用字符串 origin 来代替整个 URL。

5. git clone

6. git fetch

   1. 这个命令会访问远程仓库，从中拉取所有你还没有的数据。 执行完成后，你将会拥有那个远程仓库中所有分支的引用，可以随时合并或查看。

   2. 注意 git fetch 命令只会将数据下载到你的本地仓库——它并不会自动合并或修改你当前的工作。 当准备好时你必须手动将其合并入你的工作。

7. git pull

   1. 通常会从最初克隆的服务器上抓取数据并自动尝试合并到当前所在的分支。

   2. 在大多数情况下它的含义是一个 git fetch 紧接着一个git merge 命令

8. git add 

9. .gitignore

   1. 将要忽略的文件

   2. • 所有空行或者以注释符号 ＃ 开头的行都会被 Git 忽略。
      • 可以使用标准的 glob 模式匹配。
      • 匹配模式最后跟反斜杠（ / ）说明要忽略的是目录。
      • 要忽略指定模式以外的文件或目录，可以在模式前加上惊叹号（ ! ）取反。

   3. 

      ```c
      #此为注释 – 将被 Git 忽略
      
      *.a # 忽略所有 .a 结尾的文件
      !lib.a # 但 lib.a 除外
      /TODO # 仅仅忽略项目根目录下的 TODO 文件，不包括 subdir/TODO
      build/ # 忽略 build/ 目录下的所有文件
      doc/*.txt # 会忽略 doc/notes.txt 但不包括 doc/server/arch.txt
      ```

      

10. git commit -m 

   1. 提交更新

11. git commit -a

   12. 跳过  git add 
   13. 自动把所有已经跟踪过的文件暂存起来一并提交

14. git status

15. gi

16. git checkout  A分支 

   17. --切换到对应A分支

18. git branch  A分支 
    1. -- 新建A分支

19. git checkout -b  A分支
    1. 新建A分支，并切换到A分支

20. git branch -d  A分支
    1. 删除A分支

21. git branch -v 
    1. 查看各个分支最后一次commit信息

22. git branch --merged
    1. 查看那些分支已被并入当前分支

23. git branch --no-merged
    1. 查看尚未合并的工作

24. git diff

    1. 查看尚未保存的文件更新了那部分

25. git diff --cached
    git diff --staged ，

    1. 要看已经暂存起来的文件和上次提交时的快照之间的差异

26. git mv

27. git log

28. 

29. git push origin  本地分支
    1. origin 远程分支 

30. git push origin  :A
    1. 删除远程分支 A\

31. git push origin --delete serverfix

    1. 基本上这个命令做的只是从服务器上移除这个指针。 Git 服务器通常会保留数据一段时间直到垃圾回收运行，所以如果不小心删除掉了，通常是很容易恢复的。

32. git remove

    1. 移除文件操作
    2. 如果删除之前修改过并且已经放到暂存区域的话，则必须
       要用强制删除选项-f

33. git remove --cached

    1. 把文件从 Git 仓库中删除（亦即从暂存区域移除），但仍然希望保留在当前工作目录中。

34. git show

## 变基

---整合分支

变基的基本操作

![image-20230411091016025](/home/ly/.config/Typora/typora-user-images/image-20230411091016025.png)

1.  整合分支最容易的方法是 merge 命令。 它会把两个分支的最新快照（C3 和 C4）以及二者最近的共同祖先（C2）进行三方合并，合并的结果是生成一个新的快照（并提交）。

2. 你可以提取在 C4 中引入的补丁和修改，然后在 C3 的基础上应用一次。 在 Git 中，这种操作就叫做 变基（rebase）。 你可以使用 rebase 命令将提交到某一分支上的所有修改都移至另一分支上，就好像“重新播放”一样。

   ```
   //你可以检出 experiment 分支，然后将它变基到 master 分支上：
   $ git checkout experiment
   $ git rebase master
   
   ```

   ![image-20230411091517729](/home/ly/.config/Typora/typora-user-images/image-20230411091517729.png)

3. 它的原理是首先找到这两个分支（即当前分支 experiment、变基操作的目标基底分支 master） 的最近共同
   祖先 C2，然后对比当前分支相对于该祖先的历次提交，提取相应的修改并存为临时文件， 然后将当前分支指向
   目标基底 C3, 最后以此将之前另存为临时文件的修改依序应用。

4. 最后---将 C4 中的修改变基到 C3 上现在回到 master 分支，进行一次快进合并。

5. ```
   $ git checkout master
   $ git merge experiment
   ```

   ![image-20230411091531521](/home/ly/.config/Typora/typora-user-images/image-20230411091531521.png)



1. git rebase --onto master server client
   1. 取出 client 分支，找出它从 server 分支分歧之后的补丁， 然后把这些补丁在master 分支上重放一遍，让 client 看起来像直接基于 master 修改一样”。这理解起来有一点复杂，不过效果非常酷。

## 个人令牌

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

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

# 错误

```
>>>git push 2023 master 

Username for 'https://github.com': L-creater
Password for 'https://L-creater@github.com': 
remote: Support for password authentication was removed on August 13, 2021.
remote: Please see https://docs.github.com/en/get-started/getting-started-with-git/about-remote-repositories#cloning-with-https-urls for information on currently recommended modes of authentication.
fatal: 'https://github.com/L-creater/2023.git/' 鉴权失败
```

-- 个人访问令牌

- 只需创建一个 ssh 密钥并将它们添加到您的 github acc
- help: [Create ssh key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)在此页面上，首先选择您的操作系统，然后按照以下步骤操作
- [将新的 SSH 密钥添加到您的 GitHub 帐户](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)
- 最后，使用 ssh 链接而不是 http 克隆 repos
