#include <tiramisu/tiramisu.h>

using namespace tiramisu;

int main(int argc, char** argv) {
    tiramisu::init("test_after");

    static const int N = 48;
    static const int M = 48;
    
    var x("x", 0, N);
    var y("y", 0, M);
    
    computation f("f", {y, x}, x + y);
    computation g("g", {y, x}, f(y, 48 - 1 - x));

    if (std::getenv("AFTER")) g.after(f, x);
    else g.after(f, computation::root);

    buffer buf_f("buf_f", {M, N}, p_int32, a_temporary);
    buffer buf_g("buf_g", {M, N}, p_int32, a_output);

    f.store_in(&buf_f, {y, x});
    g.store_in(&buf_g, {y, x});

    tiramisu::codegen({&buf_g}, "test_after.o", false, true);

    return 0;
}
