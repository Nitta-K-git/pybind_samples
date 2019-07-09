import numpy as np
import eigen_matrix as tes

x = np.arange(32).reshape(8, 4).astype(np.float64)
print(x)
tes.block_test(x,10)
x = x.reshape(2,4,4)
print(x)
