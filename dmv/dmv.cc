#include <tiramisu/tiramisu.h>

using namespace tiramisu;

int main(int argc, char** argv) {
    tiramisu::init("dmv");

    static const int T = 128;
    static const int N = 48;
    
    var i("i", 0, N);
    var j("j", 0, N);
    var t("t", 0, T);

    input mat("mat", {i, j}, p_float32);
    input vec("vec", {j}, p_float32);

    computation prod_init("prod_init", {t, i}, 0.f);
    computation prod("prod", {t, i, j}, p_float32);
    prod.set_expression(prod(t, i, j - 1) + mat(i, j) * prod(t - 1, j, N - 1));
    computation out("out", {i}, p_float32, prod(T - 1, i, N - 1));

    prod.after(prod_init, i);
    prod.interchange(i, j);
    out.after(prod, computation::root);

    buffer buf_mat("buf_mat", {N, N}, p_float32, a_input);
    buffer buf_vec("buf_vec", {N}, p_float32, a_input);
    buffer buf_prod("buf_prod", {2, N}, p_float32, a_temporary);
    buffer buf_out("buf_out", {N}, p_float32, a_output);

    mat.store_in(&buf_mat, {i, j});
    vec.store_in(&buf_vec, {j});
    prod_init.store_in(&buf_prod, {0, i});
    prod.store_in(&buf_prod, {t % 2, i});
    out.store_in(&buf_out, {i});

    tiramisu::codegen({&buf_mat, &buf_vec, &buf_out}, "dmv.o", false, true);

    return 0;
}
