import test_ffi
import numba

print("=== Starting FFI test ===")

# Test that the shared library loads
print("Imported test_ffi successfully")


# Test construct
print("\nTesting construct()")

ptr = test_ffi.construct()

print("Construct returned:")
print(ptr)
print(type(ptr))

if ptr == 0:
    print("FAILED: construct returned null pointer")
else:
    print("PASSED: construct returned valid pointer")


# Test AddOne
print("\nTesting add_one()")

try:
    result = test_ffi.add_one(
        ptr,
        10,
        False,
        14
    )

    print("add_one result:")
    print(result)

    print("PASSED add_one")

except Exception as e:
    print("FAILED add_one")
    print(type(e))
    print(e)



# Test AddIt
print("\nTesting add_it()")

try:
    result = test_ffi.add_it(ptr)

    print("add_it result:")
    print(result)

    print("PASSED add_it")

except Exception as e:
    print("FAILED add_it")
    print(type(e))
    print(e)



# Test destructor
print("\nTesting destruct()")

try:
    result = test_ffi.destruct(ptr)

    print("destruct result:")
    print(result)

    print("PASSED destruct")

except Exception as e:
    print("FAILED destruct")
    print(type(e))
    print(e)


print("\n=== Finished ===")