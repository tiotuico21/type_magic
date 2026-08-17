def FFIEntry(*args):
	return {cpp_name: "FFIEntry<"+",".join(*args)+">"}
def Print(*args):
	return {cpp_name: "Print<"+",".join(*args)+">"}
FFIEntry<AddOne> = { cpp_name: "FFIEntry<AddOne>"}

AddOne = { cpp_name: "AddOne"}

AddIt = { cpp_name: "AddIt"}

FFIEntry<AddIt> = { cpp_name: "FFIEntry<AddIt>"}

FFIGen = { cpp_name: "FFIGen"}

