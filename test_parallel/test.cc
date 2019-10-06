#include <tiramisu/tiramisu.h>

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("test");

    // -------------------------------------------------------
    // Layer I: provide the algorithm.
    // -------------------------------------------------------

    // Declare an iterator. The range of this iterator is [0, 10[
    // i.e., 0<=i<10
    var x("x", 0, 10);
    var y("y", 0, 20);

    computation S_init("S_init", {x, y}, expr(1));
    computation S("S", {x, y}, p_int32);

    S.set_expression(S(x - 1, y) + S(x, y - 1));
    S.parallelize(x);
    S.after(S_init, y);

    buffer buf("buf", {10, 20}, p_int32, a_output);

    S.store_in(&buf, {x, y});
    tiramisu::codegen({&buf}, "");

    return 0;
}
