

class MyClass:

    def __call__(self,*args):
        print("Args are: ",",".join(str(x) for x in [*args]))

x = MyClass()
x(1,2,3,4)