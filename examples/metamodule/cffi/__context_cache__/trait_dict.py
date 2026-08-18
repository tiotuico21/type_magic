def FFIEntry(*args):
	return {cpp_name: "FFIEntry<"+",".join(*args)+">"}
def Print(*args):
	return {cpp_name: "Print<"+",".join(*args)+">"}
FFIEntry<SubArgs> = { cpp_name: "FFIEntry<SubArgs>"}

SubArgs = { cpp_name: "SubArgs"}

FFIEntry<AddOne> = { cpp_name: "FFIEntry<AddOne>"}

AddOne = { cpp_name: "AddOne"}

AddIt = { cpp_name: "AddIt"}

FFIEntry<AddIt> = { cpp_name: "FFIEntry<AddIt>"}

IsTrue = { cpp_name: "IsTrue"}

FFIEntry<IsTrue> = { cpp_name: "FFIEntry<IsTrue>"}

FFIGen = { cpp_name: "FFIGen"}

