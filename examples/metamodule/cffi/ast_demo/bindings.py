
from numpy import typename

import sys
import pathlib
import importlib.abc
import importlib.util
from importlib.machinery import ModuleSpec

class MagicLoader():
    counter = 0

    def create_module(self, spec):
        return None

    def exec_module(self, module):
        def __getattr__(name):
            if not pathlib.Path(f"./{name}.py").is_file():
                print("No module")
                print(f"Name failed to find: {name}\n\n")
                #setattr(module, name, None)
                return None
            print("\n\nFOUND MODULE\n\n")
            fullname = f"bindings.{name}"
            spec = importlib.util.spec_from_file_location(fullname, f"./{name}.py")
            mod = importlib.util.module_from_spec(spec)
            sys.modules[fullname] = mod
           
            spec.loader.exec_module(mod)

            #sats it returns none
            setattr(module, name, mod)
            return mod
        module.__getattr__ = __getattr__


class MagicFinder():
    @classmethod
    def find_spec(cls, fullname, path, target=None):
        print(f"\n\nENTERED FINDER - FULL NAME: {fullname} Path: {path} \n\n")
        if fullname == "bindings":
            #    magic_name = f"magic.{fullname}"
            print(f"\n\nFOUND - FULL NAME: {fullname} Path: {path} \n\n")
            return ModuleSpec(fullname, MagicLoader(), is_package=True)
        print(f"\n\nNOT FOUND - FULL NAME: {fullname} Path: {path} \n\n")
        return None


sys.meta_path.insert(0, MagicFinder()) 

