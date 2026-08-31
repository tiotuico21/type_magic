import test_ffi
import numba



ctx = test_ffi.construct()
print(test_ffi.stopping_time(ctx, 27))
ptr = test_ffi.makeInstance(numba.types.int32)

test_ffi.test_print(ptr, ctx, 3)