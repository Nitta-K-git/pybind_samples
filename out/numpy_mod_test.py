import numpy_mod as tes
import numpy as np
a = np.array([1,3,5,6]).reshape(2,2) # 今回の関数は2次元のみ限定
print(a)
tes.print_array(a) # numpyデータをprint
b = tes.modify_array(a,3) # ブロードキャスト処理
print(a)
print(b)
tes.modify_array_inplace(a,4) # ブロードキャスト処理で引数自体を書き換え
print(a)
