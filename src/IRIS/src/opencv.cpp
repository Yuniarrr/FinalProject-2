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
using namespace ros;

class Foto
{
    public:
        Mat foto,foto_resize,foto_color,tresh,foto_flip,foto_edit;
        ros::Publisher pub_foto;
        IRIS::BSTX pesan;

        Foto(ros::NodeHandle *nh)
        {
            // pub_foto = nh->advertise<IRIS::BSTX>("foto", 1000);
            image_transport::ImageTransport it(*nh);
            image_transport::Publisher pub = it.advertise("data_gambar",1000);
        }

        void publish(const ros::TimerEvent &e)
        {
            ros::Rate frekuensi_loop(1);
                foto = imread("gambar/gam1.jpg");
                Point2f center;
                // center.x=100;
                // center.y=50;
                float radius;
                resize(foto,foto_resize,Size(),0.2,0.2);
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
                    if (radius>=45 && radius <=80){
                        circle(foto_resize,center,radius,Scalar(255,0,0),3);
                        printf ("x= %.2f, y= %.2f, rad= %.2f\n\n", center.x, center.y, radius);
                        pesan.bola_x = center.y;
                        pesan.bola_y = center.x;
                    }
                }
                // flip(foto_resize,foto_flip,-1);
                // rotate(foto_resize,foto_edit,ROTATE_90_CLOCKWISE);
                // imshow("WINDOW", foto_resize);
                // waitKey(0);

            while(ros::ok())
            {

                // pesan.bola_x = pos_mesin_xnya;
                pub_foto.publish(pesan);
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
    ros::Timer timer;
    ros::MultiThreadedSpinner spn;

    Foto foto = Foto(&nh);

    timer = nh.createTimer(ros::Duration(1), &Foto::publish, &foto);
    
    spn.spin();

    // ros::Publisher foto_2;
    // image_transport::ImageTransport it(nh);
    // image_transport::Publisher pub = it.advertise("data_gambar",1000);
    // ros::Publisher foto_2 = nh.advertise<IRIS::BSTX>("data_gambar",1000);
    // ros::Rate frekuensi_loop(1);
    // IRIS::BSTX pesan;

    // while (ros::ok())
    // {
    // Mat foto,foto_resize,foto_color,tresh,foto_flip,foto_edit;
    // foto = imread("gambar/gam1.jpg");
    // Point2f center;
    // // center.x=100;
    // // center.y=50;
    // float radius;
    // resize(foto,foto_resize,Size(),0.2,0.2);
    // // resize(foto,foto_resize,Size(),0.2,0.2);
    // cvtColor(foto_resize, foto_color, COLOR_BGR2GRAY);
    // // cvtColor(foto_resize, foto_color, COLOR_BGR2HSV);
    // int L_H = 0;
    // int U_H = 85;
    // int L_S = 0;
    // int U_S = 255;
    // int L_V = 0;
    // int U_V = 255;
    // inRange(foto_color, Scalar (L_H, L_S, L_V), Scalar(U_H, U_S, U_V), tresh);

    // vector<vector<Point>> contours;
    // vector<Vec4i> hierarchy;
    // findContours(tresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    // for (int i=0;i<contours.size();i++)
    // {
    //     vector<Point>contours_lagi;
    //     contours_lagi = contours[i];
    //     minEnclosingCircle(contours_lagi,center,radius);
    //     if (radius>=45 && radius <=80){
    //         circle(foto_resize,center,radius,Scalar(255,0,0),3);
    //         printf ("x= %.2f, y= %.2f, rad= %.2f\n\n", center.x, center.y, radius);
    //         pesan.bola_x = center.y;
    //         pesan.bola_y = center.x;
    //     }
    // }
    // // flip(foto_resize,foto_flip,-1);
    // // rotate(foto_resize,foto_edit,ROTATE_90_CLOCKWISE);
    // // imshow("WINDOW", foto_resize);
    // // waitKey(0);


    //     // pesan.bola_x = pos_mesin_xnya;
    //     foto_2.publish(pesan);
    //     ROS_INFO("BOLA X = %ld",pesan.bola_x);
    //     ROS_INFO("BOLA Y = %ld",pesan.bola_y);

    //     ros::spinOnce();
    //     frekuensi_loop.sleep();
    // }

    return 0;
}