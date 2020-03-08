
#include "VirtualStateMachine.hpp"
#include "WaitingStateMachine.hpp"
#include <benchmark/benchmark.h>
static void BM_variant(benchmark::State& state) {
  for (auto _ : state)
    variant_sm::Demo{}();
}
BENCHMARK(BM_variant);
static void BM_virtual(benchmark::State& state) {
  for (auto _ : state)
    virt_sm::Demo{}();
}
BENCHMARK(BM_virtual);


BENCHMARK_MAIN();

// int main() {
//   LOG_TRACE("\n============================================================\n"
//             "\nVariant state machine"
//             "\n============================================================\n");
//   variant_sm::Demo{}();
//   LOG_TRACE("\n============================================================\n"
//             "\nVirtual state machine"
//             "\n============================================================\n");
//   virt_sm::Demo{}();
//   LOG_TRACE(
//       "\n============================================================\n" <<
//       std::flush;);
// }