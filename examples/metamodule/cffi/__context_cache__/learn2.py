import numba
from numba import types
from numba import cfunc
from llvmlite import binding


binding.initialize()

#to say use the machine i am running on
binding.initialize_native_target()
binding.initialize_native_asmprinter()

@numba.cfunc("float64(float64,float64)")
def multiply(x, y):
    return x * y

llvm_ir = multiply.inspect_llvm()

'''
with open("multiply.ll", "w") as f:
    f.write(llvm_ir)
'''



#converts it into a LLVM internal object
module = binding.parse_assembly(llvm_ir)
#check if all types and instructions are valid etc
module.verify()

#need a machine in between
#need to tell compiler what architecture os etc we are using
#that info is in target and creates the taegwt machine to be what we emit from module
#interprets whast in module by target machine to know how to do instructions
#its specialized to the machine
target = binding.Target.from_default_triple()
target_machine = target.create_target_machine()

obj = target_machine.emit_object(module)

with open("multiply.o", "wb") as f:
    f.write(obj)



def multiply(x: float, y: float) -> float:
    return x * y

sig = (types.float64, types.float64)

compiled = numba.njit(sig)(multiply)

# Force compilation
compiled(2.0, 3.0)

llvm_ir = compiled.inspect_llvm(sig)

print("LLVM IR for multiply:\n", llvm_ir[:500])
