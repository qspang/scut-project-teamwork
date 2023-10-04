import ctypes

# Load the DLL
#my_dll = ctypes.WinDLL('C:\\Users\\hubia\\source\\repos\\Dll_for_python_1\\x64\\Release\\Dll_for_python_1.dll')
my_dll = ctypes.WinDLL('Dll_for_python_1.dll')

# Declare the argument and return types of the function
my_dll.test()

# Call the function
