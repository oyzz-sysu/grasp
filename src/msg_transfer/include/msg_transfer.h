#ifndef _MSG_TRANSFER_H
#define _MSG_TRANSFER_H

#include <ros/ros.h>
#include <iostream>
#include <vector>
#include <nav_msgs/Odometry.h>
#include <msg_transfer/PositionCommand.h>
#include <opti_msgs/Odom.h>
#include <arm_test/track.h>
#include <arm_test/position.h>
#include <msg_transfer/SetRefPoint.h>


namespace msg_transfer{

class msg_transferNode{
public:
    msg_transferNode(ros::NodeHandle &n);
    ~msg_transferNode(){};

    void InitPublishers(ros::NodeHandle &n);
    void InitSubscribers(ros::NodeHandle &n);
    

private:
    ros::Publisher OdometryPublisher_;
    ros::Publisher PositionCommandPublisher_;
    ros::Subscriber OptitrackSubscriber_;
    ros::Subscriber IstrackSubscriber_;
    ros::Subscriber ConsoleSubscriber_;
    ros::Subscriber OdomSubscriber_;


    ros::ServiceServer set_point_server_;
    bool setPoint(
        msg_transfer::SetRefPoint::Request& req_set_point, 
        msg_transfer::SetRefPoint::Response& res_set_point);

    ros::Timer publish_timer_;
    double publish_time_ = 0.01;
    nav_msgs::Odometry odom_msg_;
    msg_transfer::PositionCommand cmd_msg_;
    bool is_track_ = false;
    bool received_desired_pose_ = false;
    int id_ = 0;
    int target_rigidbody_id_ = 1;

    // 真实位置 = 相对位置 + 参考位置 + 偏移位置， 通过控制台调整相对位置来实现对接
    double x_rel_  = 0.0, y_rel_  = 0.0, z_rel_  = 0.0; // 相对位置
    double x_act_  = 0.0, y_act_  = 0.0, z_act_  = 0.0; // 真实位置
    double x_ref_  = 0.0, y_ref_  = 0.0, z_ref_  = 0.0; // 参考位置
    double x_bias_ = 0.0, y_bias_ = 0.0, z_bias_ = 0.0; // 与参考位置的偏移量

    void odomCallBack(const nav_msgs::Odometry::ConstPtr& opti_msg);
    void optitrack_msgCallBack(const opti_msgs::Odom::ConstPtr& opti_msg);
    void is_trackCallBack(const arm_test::track::ConstPtr& is_track);
    void consoleCallBack(const arm_test::position::ConstPtr& rel_msg);
    void odom_cmdPublisher(const ros::TimerEvent& time_event);
    
};



}
#endif