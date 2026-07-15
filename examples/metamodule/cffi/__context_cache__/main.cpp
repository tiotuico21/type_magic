#include <iostream>

extern "C" int _ZN8__main__8multiplyB2v1B52c8tJTIcFKzyF2ILShI4CrgQElUakCCQB1FiGSRRB9GgCAA_3d_3dEdd(
    double *result,
    void *exc_info,
    double x,
    double y);

int main()
{
    double result;

    _ZN8__main__8multiplyB2v1B52c8tJTIcFKzyF2ILShI4CrgQElUakCCQB1FiGSRRB9GgCAA_3d_3dEdd(
        &result,
        nullptr,
        2.0,
        3.0);

    std::cout << result << "\n";
}

/*
saying dont mangle the name with extern c
DOUBLE *RESULT FOR MOVSD %XMM0, (%RD1) which essentially saying
storing th e result in mem pointed by rd1
//so need a pointer for return result
then nullptr is for exception handling but we never throw exception so stays nullptr
//call the linker symbol of _zna_main
*/