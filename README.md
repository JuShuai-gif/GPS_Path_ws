本文使用ROS版本：noetic
Ubuntu版本：20.04
# 1. 软件安装
使用cutecom测试串口是否接收到消息以及验证GPS消息是否正确

### cutecom安装

```sh
sudo apt install cutecom
sudo cutecom //设置 波特率9600 可以看到反馈的数据
```

注：波特率别设置错

另外有时候串口读取无权限可使用

```sh
sudo chmod 777 /dev/ttyUSB0
```

### nmea_navsat_driver 安装

```
sudo apt-get install ros-noetic-nmea-navsat-driver libgps-dev
```
# 2. nmea-navsat-driver包的使用
它总共提供四个节点
### nmea_topic_driver
NMEA GPS主题驱动程序节点。从ROS主题读取NMEA语句，而不是直接从串行端口读取。

#### 订阅的主题
nmea_sentence（nmea_msgs / Sentence）
- NMEA句子，每条消息1条。header.stamp应该与从设备读取消息的时间相对应，以实现准确的time_reference输出。在输出消息上设置frame_id时，将使用与此主题有关的消息上的header.frame_id。在到达驾驶员之前，不必对这些句子进行校验和或以其他方式进行验证。

#### 发布的话题
[sensor_msgs / NavSatFix](http://docs.ros.org/api/sensor_msgs/html/msg/NavSatFix.html)
- 设备报告的GPS定位信息。即使设备没有有效的修补程序，它也会以可用的任何位置和状态数据发布。无效的字段可能包含NaN。

[geometry_msgs / TwistStamped](http://docs.ros.org/api/geometry_msgs/html/msg/TwistStamped.html)
- 从GPS设备输出的速度。仅在设备输出有效的速度信息时发布。驾驶员不会仅基于定位来计算速度。

[sensor_msgs / TimeReference](http://docs.ros.org/api/sensor_msgs/html/msg/TimeReference.html)
- 来自GPS设备的时间戳记用作time_ref。

### nmea_serial_driver
NMEA GPS串行驱动程序节点

#### 发表的话题

sensor_msgs / NavSatFix

- 设备报告的GPS定位信息。即使设备没有有效的修补程序，它也会以可用的任何位置和状态数据发布。无效的字段可能包含NaN。

geometry_msgs / TwistStamped

- 从GPS设备输出的速度。仅在设备输出有效的速度信息时发布。驾驶员不会仅基于定位来计算速度。

sensor_msgs / TimeReference

- 来自GPS设备的时间戳记用作time_ref。

**参数**

串口号（字符串，默认是：/ dev / ttyUSB0）

波特率（int，默认值：4800）接收NMEA数据的波特率。

frame_id（string，默认值：gps）

sensor_msgs / NavSatFix和geometry_msgs / TwistStamped输出消息的标头的frame_id。如果定义，将使用tf_prefix解决。

time_ref_source（字符串，默认值：<frame_id>）

sensor_msgs / TimeReference中用作源的值。
useRMC（bool，默认：False）

从GGA语句还是RMC语句生成位置定位。如果为True，则将从RMC生成修复程序。如果为False，则将根据GGA语句生成修复程序。当RMC提供速度信息时，使用GGA语句可以输出近似的协方差。

### nmea_topic_serial_reader
从指定的串行端口读取NMEA语句，并将其发布到ROS主题。这是与nmea_topic_driver.py一起使用的简单前端

发表的话题

nmea_sentence（nmea_msgs / Sentence）

从串行设备捕获的单个带时间戳的NMEA语句
参量

串口号（字符串，默认是：/dev/ttyUSB0）

波特率（int，默认值：4800）接收NMEA数据的波特率

frame_id（string，默认值：gps）

nmea_msgs / Sentence的标头和输出消息的frame_id。如果定义，将使用tf_prefix解决。

### 使用
打开一个新的终端

```
roscore
```

新打开一个终端

```
rosrun nmea_navsat_driver nmea_topic_serial_reader _port:=/dev/ttyUSB0 _baud:=115200
```
这是可以通过查看话题，查看输出

```
rostopic echo /nmea_sentence
```

**launch文件**

```
<launch>
 
    <!-- A simple launch file for the nmea_serial_driver node. -->
   
    <arg name="port" default="/dev/ttyUSB0" />
    <arg name="baud" default="115200" />
    <arg name="frame_id" default="gps" />
    <arg name="use_GNSS_time" default="False" />
    <arg name="time_ref_source" default="gps" />
    <arg name="useRMC" default="False" />
   
    <node name="nmea_serial_driver_node" pkg="nmea_navsat_driver" type="nmea_serial_driver" output="screen">
      <param name="port" value="$(arg port)"/>
      <param name="baud" value="$(arg baud)" />
      <param name="frame_id" value="$(arg frame_id)" />
      <param name="use_GNSS_time" value="$(arg use_GNSS_time)" />
      <param name="time_ref_source" value="$(arg time_ref_source)" />
      <param name="useRMC" value="$(arg useRMC)" />
    </node>

    <node name="gps_path_publisher_node" pkg="gps_path_puh" type="gps_path_publisher_node" output="screen"/> 

    
    <node name="rviz" pkg="rviz" type="rviz" args="-d $(find gps_path_puh)/launch/rviz.rviz" />
 <!--    -->
</launch>
```

### 参看链接
[ROS包nmea_navsat_driver读取GPS、北斗定位信息笔记](https://blog.csdn.net/p942005405/article/details/123002038)

[通过ROS包nmea_navsat_driver读取GPS、北斗定位信息（C/C++）](https://blog.csdn.net/m0_37857300/article/details/82391023?ops_request_misc=&request_id=&biz_id=102&utm_term=nmea_navsat_driver&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-2-82391023.nonecase&spm=1018.2226.3001.4187)

[北斗/GPS模块的使用-基于正点原子ATK-1218-BD](https://blog.csdn.net/weixin_46158019/article/details/130979900)
