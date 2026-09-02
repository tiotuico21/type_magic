import test_ffi
import numba



ctx = test_ffi.construct()
#print(test_ffi.stopping_time(ctx, 27))
ptr = test_ffi.meta_print.makeInstance(numba.types.int32)

test_ffi.meta_print.test__meta_print(ptr, ctx, 3)

log_ptr = test_ffi.meta_log.makeInstance(numba.types.int32)
test_ffi.meta_log.test__meta_log(log_ptr, ctx, 3)

test_ffi.destruct(ctx)