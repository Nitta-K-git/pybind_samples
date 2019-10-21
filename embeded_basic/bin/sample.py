import sys
print(sys.version)

from pip import _internal
_internal.main(['list'])

# from pip._internal.main import main as _main
# _main(['list'])

import _cc11binds as bnd

print("done")

# def add(x):
#     print(x)
#     x.append(55)
#     print(x)