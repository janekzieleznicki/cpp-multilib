
#include "VirtualStateMachine.hpp"
#include "WaitingStateMachine.hpp"
#include <benchmark/benchmark.h>
/*============================================================================*/
static void BM_variant_construction(benchmark::State &state) {
  for (auto _ : state)
    variant_sm::Demo{}.benchmark_construction();
}
BENCHMARK(BM_variant_construction);
static void BM_virtual_construction(benchmark::State &state) {
  for (auto _ : state)
    virt_sm::Demo{}.benchmark_construction();
}
BENCHMARK(BM_virtual_construction);
/*============================================================================*/
static void BM_variant_transitions(benchmark::State &state) {
  auto demo{variant_sm::Demo{}};
  for (auto _ : state)
    demo.benchmark_tranistions();
}
BENCHMARK(BM_variant_transitions);
static void BM_virtual_transitions(benchmark::State &state) {
  auto demo{virt_sm::Demo{}};
  for (auto _ : state)
    demo.benchmark_tranistions();
}
BENCHMARK(BM_virtual_transitions);
/*============================================================================*/
static void BM_variant_ignored(benchmark::State &state) {
  auto demo{variant_sm::Demo{}};
  for (auto _ : state)
    demo.benchmark_ignored_event();
}
BENCHMARK(BM_variant_ignored);
static void BM_virtual_ignored(benchmark::State &state) {
  auto demo{virt_sm::Demo{}};
  for (auto _ : state)
    demo.benchmark_ignored_event();
}
BENCHMARK(BM_virtual_ignored);
/*============================================================================*/
static void BM_variant(benchmark::State &state) {
  for (auto _ : state)
    variant_sm::Demo{}();
}
BENCHMARK(BM_variant);
static void BM_virtual(benchmark::State &state) {
  for (auto _ : state)
    virt_sm::Demo{}();
}
BENCHMARK(BM_virtual);
/*============================================================================*/

BENCHMARK_MAIN();