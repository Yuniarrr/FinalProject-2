#include <bits/stdc++.h>
#include <opencv2/opencv.hpp> 
#include "ros/ros.h"
#include "IRIS/BSTX.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/Int32.h"

using namespace std;
using namespace cv;

class Foto
{
    public:
        Mat img;
        ros::Publisher foto_pub;
        IRIS::BSTX pesan;
        Mat foto_resize,foto_color,tresh,foto_flip,foto_edit;

        Foto(ros::NodeHandle *nh)
        {
            foto_pub = nh->advertise<IRIS::BSTX>("pc2bs_telemetry", 1000);
        }

        void cl_foto(Mat foto)
        {
            ros::Rate frekuensi_loop(1);
            Point2f center;
            // center.x=100;
            // center.y=50;
            float radius;
            resize(foto,foto_resize,Size(900, 600));
            // resize(foto,foto_resize,Size(),0.2,0.2);
            cvtColor(foto_resize, foto_color, COLOR_BGR2GRAY);
            // cvtColor(foto_resize, foto_color, COLOR_BGR2HSV);
            int L_H = 0;
            int U_H = 85;
            int L_S = 0;
            int U_S = 255;
            int L_V = 0;
            int U_V = 255;
            inRange(foto_color, Scalar (L_H, L_S, L_V), Scalar(U_H, U_S, U_V), tresh);

            vector<vector<Point>> contours;
            vector<Vec4i> hierarchy;
            findContours(tresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
            for (int i=0;i<contours.size();i++)
            {
                vector<Point>contours_lagi;
                contours_lagi = contours[i];
                minEnclosingCircle(contours_lagi,center,radius);
                if (radius>=45 && radius <=80)
                {
                    circle(foto_resize,center,radius,Scalar(255,0,0),3);
                    printf ("x= %.2f, y= %.2f, rad= %.2f\n\n", center.x, center.y, radius);
                    pesan.bola_x = center.y;
                    pesan.bola_y = center.x;
                }
            }

            while (ros::ok())
            {
                foto_pub.publish(pesan);
                ROS_INFO("BOLA X = %ld",pesan.bola_x);
                ROS_INFO("BOLA Y = %ld",pesan.bola_y);

                ros::spinOnce();
                frekuensi_loop.sleep();
            }
        }

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "foto");
    ros::NodeHandle nh;
    Foto pos_gam = Foto(&nh);

    Mat foto = imread("/home/mny/workspace/src/IRIS/src/gambar/gam1.jpg");

    pos_gam.cl_foto(foto);

    return 0;
}