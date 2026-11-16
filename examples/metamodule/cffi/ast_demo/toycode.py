
from numpy import typename
from harmonize import bindings 


def add_one(arg1: float, arg2: bool, arg3: float) -> float:
    return bindings.toy_python_bindings.add_one(arg1, arg2, arg3)


python_file = open("toy_python_bindings.py", "w")

string_code = r'''
import re
import sys
import inspect
import subprocess
import numba as nb

@nb.njit(cache=False)
def add_one(arg1: float, arg2: bool, arg3: float) -> float:
    return arg1 + arg3
    '''

python_file.write(string_code)
python_file.close()




