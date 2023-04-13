## gdb调试

man gdb

1.  b  --break  +文件位置:端点位置
2.  r  -- run  运行程序
3.  c  continue
4.  layout src  调试界面
5.  s --step  -- 进入某一函数
6.  n  --next  一步
7.  info  b  查看断点
8.  list  查看源代码
9.  quit  退出gdb模式
10.  p  -- print查看value

## gdb 小技巧

1.  shell ls  
2.  shell cat 
3.  shell  + 终端命令
4.  日志功能 ---set logging on
5.  watchpoint   
    1.  观察变量是否变化
    2.  info 查看watchpoint
6.  catchpoint