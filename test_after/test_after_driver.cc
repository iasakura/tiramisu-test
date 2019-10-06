#include "Halide.h"
#include "test_after.o.h"

int main() 
{
    int N = 48, M = 48;

    Halide::Buffer<int32_t> out(N, M);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            out(j, i) = 1;
        }
    }
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << out(j, i) << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "Executing..." << std::endl;
    test_after(out.raw_buffer());

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << out(j, i) << ", ";
        }
        std::cout << std::endl;
    }
    return 0;
}
