2g测试使用方法：

1.拷贝2gtester文件夹到dragonboard/src/testcase目录下。
2.编辑dragonboard/src/testcase/Makefile这个文件，在最后加入：
   make -C 2gtester
 
3.在配置的testconfig.fex中加入以下内容：
;-------------------------------------------------------------------------------
;   2g test
;-------------------------------------------------------------------------------
[2g]
activated   = 1
setup_delay = 30
display_name= "2g"
program     = "2tester"
device_node = "/dev/ttyUSB0"
call_number = "10010"
call_time   = 30
category    = 1
run_type    = 1
;----------------------------------------------------------

其中关键的字键意义如下：
setup_delay：是在开始这个测试之前的等待延时。
这个时间必须要足够长，以保证在测试之前模组已经搜索到2G
信号。
device_node ：需要操作的设备结点，通过这个结点来发送AT命令。
call_number: 要拨打的号码
call_time : 从拨通电话，到挂电话之间的时间

通过以上配置，重新编译打包既可。



