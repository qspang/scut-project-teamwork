import ctypes

# 加载DLL
mydll = ctypes.WinDLL('Dll_for_python_testing.dll')

# 设置函数参数和返回类型
mydll.READ_FILE.restype = None
mydll.TEST_PRINT.restype = None
#mydll.F3_FUNCTION.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_char * 15, ctypes.c_char * 15]
mydll.F3_FUNCTION.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_char_p, ctypes.c_char_p]
mydll.F3_FUNCTION.restype = ctypes.c_int

# 调用函数示例
mydll.READ_FILE()  # 调用READ_FILE函数
#mydll.TEST_PRINT()
# 定义参数变量
#start_time = ctypes.create_string_buffer(b"20080203011934", 15)
#end_time = ctypes.create_string_buffer(b"20080203230546", 15)
start_time = "20080203011934"
end_time = "20080203230546"


result = mydll.F3_FUNCTION(116.65640, 40.00151, 116.93210, 39.93210, start_time.encode('utf-8'), end_time.encode('utf-8'))  # 调用F3_FUNCTION函数
print("Result:", result)

# 进行其他函数的调用和测试...
