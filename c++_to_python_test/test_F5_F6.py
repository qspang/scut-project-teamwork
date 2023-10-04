import ctypes
import numpy as np
# 加载DLL文件
mydll = ctypes.WinDLL('D:/Dll1/x64/Release/Dll1.dll')

# 定义函数签名
mydll.READ_FILE.restype = None
mydll.READ_FILE()  # 调用READ_FILE函数

mydll.F5_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
mydll.F5_FUNCTION.restype = ctypes.c_double

mydll.F6_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double]
mydll.F6_FUNCTION.restype = ctypes.c_double

start_time = "20080203011934"
end_time = "20080203230546"

res = mydll.F5_FUNCTION(start_time.encode('utf-8'), end_time.encode('utf-8'), 116.65640, 40.00151, 116.93210, 39.93210, \
    116.15640, 40.83210, 117.65540, 39.93110)
print(res, '\n')
res = mydll.F6_FUNCTION(start_time.encode('utf-8'), end_time.encode('utf-8'), 116.65640, 40.00151, 116.93210, 39.93210)
print(res)