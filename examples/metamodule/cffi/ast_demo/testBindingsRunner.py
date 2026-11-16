# import testBindings
# import sys

# print("Before:", "testBindings" in sys.modules)

# sys.modules.pop("testBindings", None)

# import toycode

# print(toycode.add_one(1.0, True, 2.0))

#import testBindings
import sys
import numba

print("Before:", "testBindings" in sys.modules)

#sys.modules.pop("testBindings", None)

import toycode

print("\n========== TESTING NESTED NUMBA COMPILATION ==========")


compiled = numba.cfunc(
    numba.types.float32(
        numba.types.float32,
        numba.types.boolean,
        numba.types.float32
    )
)(toycode.add_one)

print("\n========== GENERATED LLVM IR ==========")
print(compiled.inspect_llvm())

print(toycode.add_one(1.0, True, 2.0))