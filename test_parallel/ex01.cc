#include <tiramisu/tiramisu.h>

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("test");

    // -------------------------------------------------------
    // Layer I: provide the algorithm.
    // -------------------------------------------------------

    var x("x", 0, 10);
    var y("y", 0, 20);

    computation S_init("S_init", {y, x}, expr(1));
    computation S("S", {y, x}, p_int32);

    S.set_expression(tiramisu::expr(o_select, x == 0 || y == 0, 1, S(y, x - 1) + S(y - 1, x)));
    if (std::getenv("VECTORIZE")) S.parallelize(y);
    S.after(S_init, y);

    buffer buf("buf", {20, 10}, p_int32, a_output);

    S_init.store_in(&buf, {y, x});
    S.store_in(&buf, {y, x});
    tiramisu::codegen({&buf}, "test.o");

    return 0;
}
