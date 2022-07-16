#include <bits/stdc++.h>
#include <opencv2/opencv.hpp> 
#include "ros/ros.h"
#include "IRIS/BSRX.h"
#include "IRIS/BSTX.h"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/Int32.h"


using namespace std;

class Base
{
    public:
        ros::Subscriber semua;
        int tujuan_x;
        int tujuan_y;
        int x;
        int y;

        Base(ros::NodeHandle *nh)
        {
            semua = nh->subscribe("bs2pc_telemetry", 100, &Base::all, this);
            // semua = nh->subscribe("pc2bs_telemetry", 100, &Base::stat, this);
        }

        void all(const IRIS::BSRX::ConstPtr& data)
        {
            tujuan_x = data->x_tujuan;
            tujuan_y = data->y_tujuan;
            ROS_INFO("Posisi x : [%ld]",data->x_tujuan);
            ROS_INFO("Posisi y : [%ld]",data->y_tujuan);
            ROS_INFO("Tujuan : [%ld]",data->status);
        }

        void stat(const IRIS::BSTX::ConstPtr& data)
        {
            x = data->pos_x;
            y = data->pos_y;
            while (x == tujuan_x && y == tujuan_y)
            {
                
            }
            
        }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node3");
    ros::NodeHandle nh;

    Base all_data = Base(&nh);

    ros::spin();

    return 0;
}