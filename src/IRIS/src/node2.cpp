#include <bits/stdc++.h>
#include <opencv2/opencv.hpp> 
#include "ros/ros.h"
#include "IRIS/BSTX.h"
#include "IRIS/BSRX.h"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

// untuk status 1, 2, 3, 4
int tujuan_x;
int x_gambar;
int tujuan_y;
int y_gambar;
int stat;

class Data
{
    public:
        // setting nilai
        int posisi_x;
        int posisi_y;
        int posisi_theta;
        int kec_x;
        int kec_y;
        int kec_theta;

        // gambar
        Mat img;
        Mat foto_resize,foto_color,tresh,foto_flip,foto_edit;

        Data(int _posisi_x, int _posisi_y, int _posisi_theta, int _kec_x, int _kec_y, int _kec_theta)
        {
            posisi_x = _posisi_x;
            posisi_y = _posisi_y;
            posisi_theta = _posisi_theta;
            kec_x = _kec_x;
            kec_y = _kec_y;
            kec_theta = _kec_theta;
        }

        void set_pos_x(int _posisi_x)
        {
            posisi_x = _posisi_x;
        }

        void set_pos_y(int _posisi_y)
        {
            posisi_y = _posisi_y;
        }

        void set_pos_theta(int _posisi_theta)
        {
            posisi_theta = _posisi_theta;
        }

        void set_kec_x(int _kec_x)
        {
            kec_x = _kec_x;
        }

        void set_kec_y(int _kec_y)
        {
            kec_y = _kec_y;
        }

        void set_kec_theta(int _kec_theta)
        {
            kec_theta = _kec_theta;
        }

        int get_pos_x()
        {
            return posisi_x;
        }

        int get_pos_y()
        {
            return posisi_y;
        }

        int get_pos_theta()
        {
            if (posisi_theta >= 0)
            {
                if (posisi_theta > 180)
                {
                    posisi_theta = -180 - posisi_theta;
                }
                posisi_theta = posisi_theta;
            }
            else if (posisi_theta <= -180)
            {
                posisi_theta = (posisi_theta + 180) * -1;
            }
            return posisi_theta;
        }

        int get_kec_x()
        {
            return kec_x;
        }

        int get_kec_y()
        {
            return kec_y;
        }

        int get_kec_theta()
        {
            return kec_theta;
        }

        int get_status()
        {
            return stat;
        }
        
        // simpan nilai dari basestation
        void all(const IRIS::BSRX::ConstPtr& data)
        {
            tujuan_x = data->x_tujuan;
            tujuan_y = data->y_tujuan;
            stat = data->status;
            // ROS_INFO("Posisi x : [%ld]",data->x_tujuan);
            // ROS_INFO("Posisi y : [%ld]",data->y_tujuan);
            // ROS_INFO("Tujuan : [%ld]",data->status);
        }

        // nilai center dari x dan y
        void gambar(Mat foto)
        {
            ros::Rate frekuensi_loop(1);
            Point2f center;
            // center.x=100;
            // center.y=50;
            float radius;
            resize(foto,foto_resize,Size(900, 600));
            // resize(foto,foto_resize,Size(),0.2,0.2);
            // cvtColor(foto_resize, foto_color, COLOR_BGR2GRAY);
            cvtColor(foto_resize, foto_color, COLOR_BGR2HSV);
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
                if (radius>=30 && radius <=110)
                {
                    circle(foto_resize,center,radius,Scalar(255,0,0),3);
                    // printf ("x= %.2f, y= %.2f, rad= %.2f\n\n", center.x, center.y, radius);
                    x_gambar = center.y;
                    y_gambar = center.x;
                    // printf("x= %d, y= %d", x_gambar, y_gambar);
                }
            }
            // imshow("WINDOW", foto_resize);
            // waitKey(0);
        }

};

// publisher
ros::Publisher kirim;

// set data awal = 0
Data kirim_data = Data(0, 0, 0, 0, 0, 0);

void semua_data (const ros::TimerEvent &e)
{
    ros::Rate frekuensi_loop(1);
    while (ros::ok())
    {
        IRIS::BSTX ayok;

        // status 1
        if (stat == 1)
        {
            // menentukan gerak dari robot
            char jalan;
            cin >> jalan;
            // a ke kiri
            if (jalan == 'a')
            {
                kirim_data.set_pos_y(kirim_data.get_pos_y() - kirim_data.get_kec_y());
            }
            // d ke kanan
            else if (jalan == 'd')
            {
                kirim_data.set_pos_y(kirim_data.get_pos_y() + kirim_data.get_kec_y());
            }
            // s ke bawah
            else if( jalan == 's' )
            {
                kirim_data.set_pos_x(kirim_data.get_pos_x() + kirim_data.get_kec_x());
            }
            // w ke atas
            else if ( jalan == 'w' )
            {
                kirim_data.set_pos_x(kirim_data.get_pos_x() - kirim_data.get_kec_x());
            }
            // q sudut --
            else if ( jalan == 'q')
            {
                kirim_data.set_pos_theta(kirim_data.get_pos_theta() - kirim_data.get_kec_theta());
            }
            // e sudut ++
            else if ( jalan == 'e')
            {
                kirim_data.set_pos_theta(kirim_data.get_pos_theta() + kirim_data.get_kec_theta());           
            }
            // mematikan
            else
            {
                ros::shutdown();
            } 
        }

        // status 2
        else if ( stat == 2 )
        {
            // publish x dan y dari gambar
            ayok.bola_x = x_gambar;
            ayok.bola_y = y_gambar;

            // looping sampai x dan y dari gambar
            while (kirim_data.get_pos_x() < x_gambar)
            {
                kirim_data.set_pos_x(kirim_data.get_pos_x() + kirim_data.get_kec_x());
            }
            while (kirim_data.get_pos_y() < y_gambar)
            {
                kirim_data.set_pos_y(kirim_data.get_pos_y() + kirim_data.get_kec_y());
                kirim_data.set_pos_theta(kirim_data.get_pos_theta() + kirim_data.get_kec_theta());
            }
        }

        // status 3
        else if ( stat == 3 )
        {
            // x < titik tujuan
            if (kirim_data.get_pos_x() < tujuan_x)
            {
                while (kirim_data.get_pos_x() < tujuan_x)
                {
                    kirim_data.set_pos_x(kirim_data.get_pos_x() + kirim_data.get_kec_x());
                }
            }
            // x > titik tujuan
            else if (kirim_data.get_pos_x() > tujuan_x)
            {
                while (kirim_data.get_pos_x() > tujuan_x)
                {
                    kirim_data.set_pos_x(kirim_data.get_pos_x() - kirim_data.get_kec_x());
                }
            }
            // y < titik tujuan
            else if (kirim_data.get_pos_y() < tujuan_y)
            {
                while (kirim_data.get_pos_y() < tujuan_y)
                {
                    kirim_data.set_pos_y(kirim_data.get_pos_y() + kirim_data.get_kec_y());
                    kirim_data.set_pos_theta(kirim_data.get_pos_theta() + kirim_data.get_kec_theta());
                }
            }
            // y > titik tujuan
            else if(kirim_data.get_pos_y() > tujuan_y)
            {
                while (kirim_data.get_pos_y() > tujuan_y)
                {
                    kirim_data.set_pos_y(kirim_data.get_pos_y() - kirim_data.get_kec_y());
                    kirim_data.set_pos_theta(kirim_data.get_pos_theta() - kirim_data.get_kec_theta());
                }
            }
            
        }

        // status 4
        else if ( stat == 4 )
        {            
            if (kirim_data.get_pos_x() < x_gambar - 80)
            {
                if (kirim_data.get_pos_y() < y_gambar - 80)
                {
                    /* code */
                }
                
            }
            
            // publish x dan y dari gambar
            ayok.bola_x = x_gambar;
            ayok.bola_y = y_gambar;
            // masih bingung bedanya

            // looping sampai x dan y dari gambar
            while (kirim_data.get_pos_x() < x_gambar)
            {
                while (kirim_data.get_pos_y() < y_gambar)
                {
                    kirim_data.set_pos_y(kirim_data.get_pos_y() + kirim_data.get_kec_y());
                    kirim_data.set_pos_theta(kirim_data.get_pos_theta() + kirim_data.get_kec_theta());
                }
                kirim_data.set_pos_x(kirim_data.get_pos_x() + kirim_data.get_kec_x());
            }
        }
        
        // data publish
        ayok.pos_x = kirim_data.get_pos_x();
        ayok.pos_y = kirim_data.get_pos_y();
        ayok.pos_theta = kirim_data.get_pos_theta();
        ayok.v_x = kirim_data.get_kec_x();
        ayok.v_y = kirim_data.get_kec_y();
        ayok.v_theta = kirim_data.get_kec_theta();

        kirim.publish(ayok);

        ROS_INFO("posisi x: %d", kirim_data.get_pos_x());
        ROS_INFO("posisi y: %d", kirim_data.get_pos_y());
        ROS_INFO("posisi theta: %d", kirim_data.get_pos_theta());
        ROS_INFO("kec x: %d", kirim_data.get_kec_x());
        ROS_INFO("kec y: %d", kirim_data.get_kec_y());
        ROS_INFO("kec theta: %d", kirim_data.get_kec_theta());

        ros::spinOnce();
        frekuensi_loop.sleep();
    }
    
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "kirimkan");
    ros::NodeHandle nh;
    ros::MultiThreadedSpinner spn;

    // publish ke pc2bs_telemetry
    kirim = nh.advertise<IRIS::BSTX>("pc2bs_telemetry",1000);

    // subscribe dari bs2pc_telemetry
    ros::Subscriber semua = nh.subscribe("bs2pc_telemetry", 100, &Data::all, &kirim_data);

    // status 2 : ambil data gambar
    Mat foto = imread("/home/mny/workspace/src/IRIS/src/gambar/gam2.jpg");
    kirim_data.gambar(foto);

    ros::Timer timer;

    // setting nilai kec x, y, theta
    kirim_data.set_kec_x(100);
    kirim_data.set_kec_y(120);
    kirim_data.set_kec_theta(10);
    

    // callback semua data
    timer = nh.createTimer(ros::Duration(1), semua_data);

    spn.spin();
    
    return 0;
}