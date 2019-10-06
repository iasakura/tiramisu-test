TIRAMISU_ROOT=/home/izumi.asakura/dev/tiramisu
GXX=g++ -g

.PHONY: run
run: ${test_name}
	./${test_name}
${test_name}_generator: ${test_name}.cc
	${GXX} -std=c++11 -fno-rtti -DHALIDE_NO_JPEG -I${TIRAMISU_ROOT}/include -I${TIRAMISU_ROOT}/3rdParty/isl/include/ -I${TIRAMISU_ROOT}/3rdParty/Halide/include -I${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/3rdParty/isl/build/lib -L${TIRAMISU_ROOT}/3rdParty/Halide/lib/ -o ${test_name}_generator  -ltiramisu -lisl -lHalide -ldl -lpthread -lz -lm -Wl,-rpath,${TIRAMISU_ROOT}/build ${test_name}.cc -ltiramisu -lisl -lHalide -ldl -lpthread -lz -lm
${test_name}.o: ${test_name}_generator
	./${test_name}_generator
${test_name}.h: ${test_name}.o
${test_name}: ${test_name}.o ${test_name}.h ${test_name}_driver.cc
	${GXX} -std=c++11 -fno-rtti -I${TIRAMISU_ROOT}/include -I${TIRAMISU_ROOT}/3rdParty/Halide/include -L${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/3rdParty/Halide/lib/ -o ${test_name} -ltiramisu -lHalide -ldl -lpthread -lz -lm -Wl,-rpath,${TIRAMISU_ROOT}/build ${test_name}_driver.cc ${test_name}.o -ltiramisu -lHalide -ldl -lpthread -lz -lm
