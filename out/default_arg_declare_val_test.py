import default_arg_declare_val as tes

print(tes.add(1,2)) # 通常の使い方
print(tes.add1(j=2,i=5)) # 引数名指定なので、順不同
print(tes.add2(j=4,i=5))
print(tes.add_def()) # デフォルト値あり
print(tes.add_def1(i=3)) # デフォルトありなので、片方だけ指定可能
print(tes.add_def2(j=100))
# モジュール変数
print(tes.the_answer)
print(tes.what)