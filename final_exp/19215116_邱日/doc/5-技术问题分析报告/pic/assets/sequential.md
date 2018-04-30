```flow
st=>start: 开始
e=>end:结束
op1=>operation: 初始化全局描述符表
op2=>operation: 初始化字符设备驱动.
op3=>operation: 完成定时器设置，把中断服务程序与异常处理设置好
op4=>operation: 终端简易shell建立
err=>operation: 提示硬盘1不存在,内核panic
cond1=>condition: 硬盘1存在？
op8=>operation: 初始化文件系统
op5=>operation: 初始化硬盘分区表
op6=>operation: 格式化全盘(硬盘1)
op7=>operation: 从硬盘加载文件系统
cond2=>condition: Magic number存在？
e=>end:结束
st->op1->op2->op3->op4->cond1->e
cond1(yes)->op5->cond2
cond1(no)->err
cond2(yes)->op7
cond2(no)->op6->op8->cond2
```

