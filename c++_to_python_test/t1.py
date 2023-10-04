import ctypes

import numpy as np
from matplotlib import pyplot as plt, cm

TIME_L = "20080202011934"
TIME_R = "20080208231934"
array_size = 2000
array = (ctypes.c_int * array_size)()
arr = np.zeros((2000, 4), dtype=np.double)

# 加载DLL
mydll = ctypes.WinDLL('D:/Dll1/x64/Release/Dll1.dll')

# 设置函数参数和返回类型
mydll.READ_FILE.restype = None
mydll.TEST_PRINT.restype = None
mydll.F3_FUNCTION.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_char_p,
                              ctypes.c_char_p]
mydll.F3_FUNCTION.restype = ctypes.c_int
mydll.F4_FUNCTION.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_double, ctypes.POINTER(ctypes.c_int),
                              ctypes.POINTER(ctypes.c_double)]
mydll.F4_FUNCTION.restype = None
# 调用函数示例
mydll.READ_FILE()  # 调用READ_FILE函数
mydll.F4_FUNCTION(TIME_L.encode('utf-8'), TIME_R.encode('utf-8'), 20000.0,
                  array, arr.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))  # 调用F3_FUNCTION函数

t1 = list(TIME_L)
t2 = list(TIME_R)
L2 = int(t2[7]) - int(t1[7])
length = 0
print("`11111111111111111111")
print(L2)
for i in range(len(array) - 1):
    if array[i] == -1:
        break
    length = length + 1
print(length)

z1 = np.zeros((100, 7), dtype=np.double)
t3 = TIME_L
t2[7] = t1[7]
t4 = ''.join(t2)
for i in range(7):

    mydll.F4_FUNCTION(t3.encode('utf-8'), t4.encode('utf-8'), 20000.0,
                      array, arr.ctypes.data_as(ctypes.POINTER(ctypes.c_double)))  # 调用F3_FUNCTION函数
    for j in range(100):
        if array[j] == -1:
            break
        z1[j][i] = array[j]
    t3 = list(t3)
    t4 = list(t4)
    t3[7] = f'{int(int(t3[7]) + 1)}'
    t4[7] = f'{int(int(t4[7]) + 1)}'
    t3=''.join(t3)
    t4=''.join(t4)
    print("dddddddddddddddddddddd")
x = np.repeat([20080202, 20080203, 20080204, 20080205, 20080206, 20080207, 20080208], 100)
y = np.tile(np.arange(1, 101), 7)
z1 = z1.flatten()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.bar3d(x, y, np.zeros_like(z1), 1, 1, z1, color=cm.ScalarMappable().to_rgba(x))

plt.xlabel('time')
plt.ylabel('brides')
plt.title('3维直方图')
plt.show()

