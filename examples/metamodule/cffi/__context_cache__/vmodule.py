import sys
import importlib.abc
import importlib.util

class TypeMagicFInder(importlib.abc.MetaPathFinder):
    def find_spec(self, fullname, path, target = None):
        print("python is looking for: ", fullname)
        if fullname == "typemagic":
            print("create a mpdile and intercept")

            return importlib.util.spec_from_loader(
                fullname, 
                TypeMagicLoader()
            )
        return None
