import ctypes
from datetime import datetime

import numpy as np

# 加载DLL文件
mydll = ctypes.WinDLL('D:/Dll1/x64/Release/Dll1.dll')


def convert_datetime_to_str(datetime_str):
    datetime_obj = datetime.strptime(datetime_str, "%Y-%m-%d %H:%M:%S")
    converted_str = datetime_obj.strftime("%Y%m%d%H%M%S")
    return converted_str


def convert_str_to_datatime(datetime_str):
    datetime_obj =datetime.strptime(datetime_str, "%Y%m%d%H%M%S")
    converted_str = datetime_obj.strftime("%Y-%m-%d %H:%M:%S")
    return converted_str


def get_all_roads_by_id(res_id):  # 频度由res_id来体现。
    res_roads = {}
    for i in range(72183):
        # 在res_id数组中，下标index = id -1，id是路的id
        if (res_id[i] != -1):
            # print(i + 1, res_id[i])
            res = np.zeros((100, 2), dtype=np.double)
            mydll.get_roads_by_id(i + 1, res)
            one_road = []
            for j in range(100):
                if res[j][0] == 0:
                    break;
                one_road.append((res[j][0], res[j][1]))
            res_roads[i] = one_road
    return res_roads


import math

pi = 3.14159265358979324
a = 6378245.0
ee = 0.00669342162296594323
x_pi = 3.14159265358979324 * 3000.0 / 180.0


def wgs2bd(lat, lon):
    lat_, lon_ = wgs2gcj(lat, lon)
    pLat, pLon = gcj2bd(lat_, lon_)
    return pLon, pLat


def gcj2bd(lat, lon):
    x, y = lon, lat
    z = math.sqrt(x * x + y * y) + 0.00002 * math.sin(y * pi)
    theta = math.atan2(y, x) + 0.000003 * math.cos(x * pi)
    pLon = z * math.cos(theta) + 0.0065
    pLat = z * math.sin(theta) + 0.006
    return pLat, pLon


def bd2gcj(lat, lon):
    x, y = lon - 0.0065, lat - 0.006
    z = math.sqrt(x * x + y * y) - 0.00002 * math.sin(y * math.pi)
    theta = math.atan2(y, x) - 0.000003 * math.cos(x * math.pi)
    pLon = z * math.cos(theta)
    pLat = z * math.sin(theta)
    return pLat, pLon


def outOfChina(lat, lon):
    return (lon < 72.004 or lon > 137.8347 or lat < 0.8293 or lat > 55.8271 or False)


def transformLat(x, y):
    ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * math.sqrt(abs(x))
    ret += (20.0 * math.sin(6.0 * x * pi) + 20.0 * math.sin(2.0 * x * pi)) * 2.0 / 3.0
    ret += (20.0 * math.sin(y * pi) + 40.0 * math.sin(y / 3.0 * pi)) * 2.0 / 3.0
    ret += (160.0 * math.sin(y / 12.0 * pi) + 320 * math.sin(y * pi / 30.0)) * 2.0 / 3.0
    return ret


def transformLon(x, y):
    ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * math.sqrt(abs(x))
    ret += (20.0 * math.sin(6.0 * x * pi) + 20.0 * math.sin(2.0 * x * pi)) * 2.0 / 3.0
    ret += (20.0 * math.sin(x * pi) + 40.0 * math.sin(x / 3.0 * pi)) * 2.0 / 3.0
    ret += (150.0 * math.sin(x / 12.0 * pi) + 300.0 * math.sin(x / 30.0 * pi)) * 2.0 / 3.0
    return ret


def wgs2gcj(lat, lon):
    if outOfChina(lat, lon):
        return (lat, lon)
    dLat = transformLat(lon - 105.0, lat - 35.0)
    dLon = transformLon(lon - 105.0, lat - 35.0)
    radLat = lat / 180.0 * pi
    magic = math.sin(radLat)
    magic = 1 - ee * magic * magic
    sqrtMagic = math.sqrt(magic)
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi)
    dLon = (dLon * 180.0) / (a / sqrtMagic * math.cos(radLat) * pi)
    pLat = lat + dLat
    pLon = lon + dLon
    return pLat, pLon


def gcj2wgs(lat, lon, pLat, pLon):
    if outOfChina(lat, lon):
        pLat = lat
        pLon = lon
        return 0
    offset = OffSet(lat, lon)
    pLat = lat - offset[0]
    pLon = lon - offset[1]
    return 0


def gcj2wgs_exactly(gcjlat, gcjlon, wgs_lat, wgs_lon):
    if outOfChina(gcjlat, gcjlon):
        wgs_lat[0] = gcjlat
        wgs_lon[0] = gcjlon
        return 0
    init_delta = 0.01
    threshold = 0.000000001
    dLat = init_delta
    dLon = init_delta
    mLat = gcjlat - dLat
    mLon = gcjlon - dLon
    pLat = gcjlat + dLat
    pLon = gcjlon + dLon
    wgsLat = 0.0
    wgslon = 0.0
    i = 0.0
    newgcjlat = 0.0
    newgcjlon = 0.0

    while True:
        wgsLat = (mLat + pLat) / 2
        wgslon = (mLon + pLon) / 2
        wgs2gcj(wgsLat, wgslon, newgcjlat, newgcjlon)
        dLon = newgcjlon - gcjlon
        dLat = newgcjlat - gcjlat
        if abs(dLat) < threshold and abs(dLon) < threshold:
            break
        if dLat > 0:
            pLat = wgsLat
        else:
            mLat = wgsLat
        if dLon > 0:
            pLon = wgslon
        else:
            mLon = wgslon
        if i > 10000:
            break
        i += 1
    wgs_lat[0] = wgsLat
    wgs_lon[0] = wgslon
    return 0


# BD09=>WGS84 百度坐标系=>地球坐标系(粗略)
def bd2wgs(lat, lon, pLat, pLon):
    lat_, lon_ = bd2gcj(lat, lon)
    gcj2wgs(lat_, lon_, pLat, pLon)
    return 0


# BD09=>WGS84 百度坐标系=>地球坐标系(精确)
def bd2wgs_Exactly(lat, lon, pLat, pLon):
    lat_, lon_ = bd2gcj(lat, lon)
    gcj2wgs_exactly(lat_, lon_, pLat, pLon)
    return 0


def OffSet(lat, lon):
    Latlon = [0.0, 0.0]
    dLat = transformLat(lon - 105.0, lat - 35.0)
    dLon = transformLon(lon - 105.0, lat - 35.0)
    radLat = lat / 180.0 * pi
    magic = math.sin(radLat)
    magic = 1 - ee * magic * magic
    sqrtMagic = math.sqrt(magic)
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi)
    dLon = (dLon * 180.0) / (a / sqrtMagic * math.cos(radLat) * pi)
    Latlon[0] = dLat
    Latlon[1] = dLon
    return Latlon
