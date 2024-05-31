/**
 * @file gps_tool.h
 * @author GHR (208967048@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-31
 */
#pragma once
#include <GeographicLib/LocalCartesian.hpp>
#include <Eigen/Dense>

class GPSTool
{
public:
    double init_lon{},init_lat{};

public:
    GPSTool() = default;

    inline void SetInit(double lat, double lon)
    {
        geo_converter_.Reset(lat, lon);
    }

    // 经纬度转ENU
    inline Eigen::Vector3d ConvertLLAToENU(const Eigen::Vector3d &point_lla)
    {
        double enu_x{}, enu_y{}, enu_z{};

        geo_converter_.Forward(point_lla.x(), point_lla.y(), point_lla.z(),
                               enu_x, enu_y, enu_z);
        Eigen::Vector3d enugps{enu_x, enu_y, enu_z};
        return enugps;
    }

    // 经纬度转ENU
    inline Eigen::Vector3d ConvertLLAToENU(double lat,double lon)
    {
        double enu_x{}, enu_y{}, enu_z{};

        geo_converter_.Forward(lat, lon, 0.0,
                               enu_x, enu_y, enu_z);
        Eigen::Vector3d enugps{enu_x, enu_y, enu_z};
        return enugps;
    }

    // ENU转经纬度
    inline Eigen::Vector3d ConvertENUToLLA(const Eigen::Vector3d &enu_pos)
    {
        double lat, lon, hei;
        geo_converter_.Reverse(enu_pos.x(), enu_pos.y(), enu_pos.z(), lat, lon, hei);
        Eigen::Vector3d gps{lat, lon, hei};
        return gps;
    }

private:
    GeographicLib::LocalCartesian geo_converter_; // only support ENU
};
