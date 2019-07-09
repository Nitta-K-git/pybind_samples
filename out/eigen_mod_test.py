import numpy as np
import eigen_mod as tes

x = np.arange(12).reshape(3, 4).astype(np.float64)
print("created with numpy")
print(x)

print("using pybind11 to print")
tes.print_array(x)

x2 = tes.modify_array(x, 2.0)
print("x2 newly created with pybind11")
print(x2)

tes.modify_array_inplace(x, 3.0) # 参照渡しで書き換える
print("x modified inplace")
print(x)
