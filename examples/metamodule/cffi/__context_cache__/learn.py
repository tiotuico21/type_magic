import numba
from llvmlite import binding

binding.initialize()
binding.initialize_native_target()
binding.initialize_native_asmprinter()
@numba.njit (nopython=True)
def add(a: int, b: int) -> int:
    return a + b

result = add(2, 3)

llvm_ir_dict = add.inspect_llvm()
print("keys (signatures):", llvm_ir_dict.keys())

llvm_ir = llvm_ir_dict[(numba.int64, numba.int64)]
print("LLVM IR for add(2,3):\n", llvm_ir[:500])

llvm_mod = binding.parse_assembly(llvm_ir)

llvm_mod.verify()

pm = binding.create_module_pass_manager()

pm.add_instruction_combining_pass()
pm.add_gvn_pass()
pm.add_cfg_simplification_pass()

pm.run(llvm_mod)

print(llvm_mod)