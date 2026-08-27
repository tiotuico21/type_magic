def FFIEntry(*args):
	return {cpp_name: "FFIEntry<"+",".join(*args)+">"}
def Print(*args):
	return {cpp_name: "Print<"+",".join(*args)+">"}
SubOne = { "cpp_name": "SubOne"}

FFIEntry_SubOne = { "cpp_name": "FFIEntry<SubOne>"}

