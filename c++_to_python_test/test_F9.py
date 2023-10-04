import ctypes
import numpy as np

# 加载DLL文件
mydll = ctypes.WinDLL('D:/Dll1/x64/Release/Dll1.dll')

# 定义函数签名
mydll.READ_FILE.restype = None
mydll.READ_FILE()  # 调用READ_FILE函数
mydll.F9_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                              ctypes.c_double,
                              ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                              ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double)]

# mydll.F9_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
# ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.POINTER(ctypes.c_double), np.ctypeslib.ndpointer(dtype=np.double, shape=(10000, 2))]


Time = ctypes.c_double()
# res = (ctypes.c_double * 10000)()
res = np.zeros((10000, 2), dtype=np.double)
start_time = "20080203011934"
end_time = "20080203230546"
mydll.F9_FUNCTION(start_time.encode('utf-8'), end_time.encode('utf-8'), 116.22940, 39.93751, 116.34910, 39.76210,
                  116.39740, 39.93210, 116.45640, 39.76810, ctypes.byref(Time),
                  res.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))
# mydll.F9_FUNCTION(start_time.encode('utf-8'), end_time.encode('utf-8'), 116.65640, 40.00151, 116.93210, 39.93210, \
#     116.15640, 40.83210, 117.65540, 39.93110, ctypes.byref(Time), res)

print(f"Time: {Time.value}")
# res = np.unique(res, axis=0)
# res = res[1:]  # 删除第一行
print(res)
for i in range(10000):
    if res[i][0] == 0.0:
        res = res[0:i]
        break  #

import numpy as np

# 打开原有HTML文件，读取内容
with open('t1.html', 'r', encoding='utf-8') as f:
    html_content = f.read()
shape = res.shape

line_total = 2



import math
pi = 3.14159265358979324
a = 6378245.0
ee = 0.00669342162296594323
x_pi = 3.14159265358979324 * 3000.0 / 180.0

def wgs2bd(lat, lon):
    lat_, lon_ = wgs2gcj(lat, lon)
    pLat, pLon = gcj2bd(lat_, lon_)
    return pLat, pLon


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









# 打开新HTML文件，写入内容
with open('new.html', 'w', encoding='utf-8') as k:
    # 将原有HTML文件中的内容写入新HTML文件中
    k.write(html_content)

    # 在新HTML文件中添加新的JavaScript代码
    k.write('<script type="text/javascript">\n')
    k.write(f'var marker1 = new BMapGL.Marker(new BMapGL.Point({res[0][0]}, {res[0][1]})); ')
    k.write(f'var marker2 = new BMapGL.Marker(new BMapGL.Point({res[shape[0] - 1][0]}, {res[shape[0] - 1][1]})); ')
    k.write('var polyline = new BMapGL.Polyline([')
    for i in range(shape[0]):
        p1,p2=wgs2bd(res[i][0],res[i][0])
        k.write(f'new BMapGL.Point({res[i][0]},{res[i][1]}),')

    k.write('], {strokeColor:"blue", strokeWeight:2, strokeOpacity:0.5}); '
            'function add_overlay()'
            '    {'
            'map.addOverlay(marker1);           '
            'map.addOverlay(marker2);           '
            '    map.addOverlay(polyline); '
            '    }'
            ' add_overlay();')
    k.write('</script>')
    print("写入完成")
