
import re
import sys
import inspect
import subprocess
import numba as nb

@nb.njit(cache=False)
def add_one(arg1: float, arg2: bool, arg3: float) -> float:
    return arg1 + arg3
    