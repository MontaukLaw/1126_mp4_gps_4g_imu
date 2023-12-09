20231026

1. CMakeLists.txt 会生成两个程序, 一个是app, 一个是opencv_demo

编译:
$ cmake --build build 

源码的入口查看CMakeLists.txt

2. 录制视频的方法跟海思是一样的.

3. 使用4G上网的方法就是, 插入网卡拧紧螺丝, 并插入nano sim卡到板子上的sim卡槽

$ mkdir /etc/ppp/ #如果已经存在，则不用重复创建
$ cd /demo/bin/4G/
使用ls指令可以看到此路径下有多个脚本文件
其中尾缀10000表示适用于电信卡，10010表示适用于联通卡，10086表示适用于移动卡。
这里以联通卡为例。执行下面ppp-on脚本，&的作用是后台运行
$ ./ppp-on-10010 &
使用ifconfig可查看4G模块的网络名称。
ping百度测试连通性，-I是指定网卡

$ ping baidu -I ppp0

4. 挂载imu6050 姿态传感器的方法是:

$ cd /lib/modules/4.19.111/
$ insmod industrialio-triggered-buffer.ko
$ insmod inv-mpu6050.ko
$ insmod inv-mpu6050-spi.ko

5. 在板子上运行app, 会在第60帧左右生成一个/userdata/raw.rgb的文件, 可以使用YUVViewer一类的软件打开, 或者使用这个目录下面的opencv_demo直接转成jpg

5. 由于原子的1126并没有多余的串口, 所以没法接GPS模块, 将来如果自制板子, 可以考虑多引出一个串口, GPS模块链接:
https://detail.tmall.com/item.htm?_u=q124ig721d2&id=624586693428&spm=a1z09.2.0.0.1d982e8d4hHSVS
PC上面测试过没问题.

