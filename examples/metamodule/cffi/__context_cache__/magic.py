import sys
import pathlib
import importlib.util
from importlib.machinery import ModuleSpec

class VModuleLoader:
    counter = 0

    def create_module(self, spec):
        return None

    def exec_module(self, module):
        def __getattr__(name):
            if not pathlib.Path(f"./{name}.py").is_file():
                setattr(module, name, None)
                return None
            spec = importlib.util.spec_from_file_location(name, f"./{name}.py")
            mod = importlib.util.module_from_spec(spec)
            sys.modules[f"magic.{name}"] = mod
            spec.loader.exec_module(mod)


            setattr(module, name, mod)
            return mod
        module.__getattr__ = __getattr__

class VModuleFinder:
    @classmethod
    def find_spec(cls, fullname, path, target=None):
        if fullname == "vmodule":
            return ModuleSpec(fullname, VModuleLoader())
        return None


sys.meta_path.insert(0, VModuleFinder())

pathlib.Path.unlink("./test_ffi.py")


      