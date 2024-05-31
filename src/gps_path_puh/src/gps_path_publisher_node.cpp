/**
 * @file gps_path_publisher_node.cpp
 * @author GHR (208967048@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-31
 */

#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include "gps_tool.h"
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>

class GPSPathPublisher
{
public:
    GPSPathPublisher()
    {
        path_pub_ = nh_.advertise<nav_msgs::Path>("/gps_path", 10);
        gps_sub_ = nh_.subscribe("/fix", 10, &GPSPathPublisher::gpsCallback, this);
        path_.header.frame_id = "map";
    }
    // GPS回调函数
    void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr &msg)
    {
        if (!init_position_set_)
        {
            gpstool.SetInit(msg->latitude, msg->longitude);
            init_position_set_ = true;
            ROS_INFO("Initial position set to: [%f, %f]", msg->latitude, msg->longitude);
            std::cout << "init succful" << std::endl;
        }

        addToPath(msg);
    }

private:
    void addToPath(const sensor_msgs::NavSatFix::ConstPtr &msg)
    {
        Eigen::Vector3d lla(msg->latitude, msg->longitude,0.0);
        ROS_INFO("GPS position set to: [%.6f, %.6f]", msg->latitude, msg->longitude);

        geometry_msgs::PoseStamped pose_stamped;
        pose_stamped.header.stamp = ros::Time::now();
        pose_stamped.header.frame_id = "map";

        // Convert GPS (latitude, longitude) to UTM (x, y)
        Eigen::Vector3d enu_pos = gpstool.ConvertLLAToENU(lla);

        static double xx = 0.0;
        static double yy = 0.0;

        ROS_INFO("Position set to: [%.6f, %.6f]", enu_pos.x(), enu_pos.y());
        
        pose_stamped.pose.position.x = enu_pos.x();
        pose_stamped.pose.position.y = enu_pos.y();
        //pose_stamped.pose.position.x = xx;
        //pose_stamped.pose.position.y = yy;
        //xx += 0.5;
        //yy += 0.5;
        pose_stamped.pose.position.z = 0;


        // Orientation is not set (use default quaternion)
        pose_stamped.pose.orientation.w = 1.0;

        path_.poses.push_back(pose_stamped);
        path_.header.stamp = ros::Time::now();

        path_pub_.publish(path_);
    }

    ros::NodeHandle nh_;
    ros::Publisher path_pub_;
    ros::Subscriber gps_sub_;

    nav_msgs::Path path_;

    bool init_position_set_ = false;
    GPSTool gpstool;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gps_path_publisher");
    GPSPathPublisher gps_path_publisher;
    ros::spin();
    return 0;
}
