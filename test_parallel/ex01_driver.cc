#include "Halide.h"
#include "test.h"

int main() 
{
    int N = 10, M = 20;

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
    test(out.raw_buffer());

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << out(j, i) << ", ";
        }
        std::cout << std::endl;
    }
    return 0;
}
