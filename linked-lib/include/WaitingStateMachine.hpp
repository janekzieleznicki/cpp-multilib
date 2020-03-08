#pragma once
#include "Logger.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
namespace variant_sm {
/*============================================================================*/
struct SendEvent {
  int a{100};
};

struct ReceiveEvent {
  std::string data{"complex, but useless data"};
};

struct TimeoutEvent {
  std::string data{"complex, but useless data"};
};
using MaybeEvent =
    std::variant<std::monostate, SendEvent, ReceiveEvent, TimeoutEvent>;
/*============================================================================*/

template <template <typename> typename... States> class WaitingStateMachine {
public:
  bool timeout{false};
  void operator()() {
    auto maybe_event =
        std::visit([](auto statePtr) { return (*statePtr)(); }, currentState);
    std::visit([this](auto &&event) { handle(event); }, std::move(maybe_event));
    // std::visit([this](auto statePtr) { handle((*statePtr)()); },
    //            currentState);
  }

  template <template <typename StateMachine> typename State>
  void transitionTo() {
    std::visit([](auto statePtr) { statePtr->exit(); }, currentState);
    currentState = &std::get<State<WaitingStateMachine>>(states);
    std::visit([](auto statePtr) { statePtr->entry(); }, currentState);
  }

  template <typename Event> void handle(const Event &event) {
    auto passEventToState = [this, &event](auto statePtr) {
      statePtr->handle(event).execute_transition(*this);
    };
    std::visit(passEventToState, currentState);
  }
  std::string trace{};

private:
  std::tuple<States<WaitingStateMachine>...> states{
      States<WaitingStateMachine>{*this}...};
  std::variant<States<WaitingStateMachine> *...> currentState{
      &std::get<0>(states)};
};
/*============================================================================*/
// Helpers
template <template <typename StateMachine> typename State> struct TransitionTo {
  template <typename Machine> void execute_transition(Machine &machine) {
    machine.template transitionTo<State>();
  }
};

struct Nothing {
  template <typename Machine> void execute_transition(Machine &) {}
};
/*============================================================================*/

template <typename StateMachine> struct IdleState;
template <typename StateMachine> struct WaitingState;
/*============================================================================*/

using WaitingMachine = WaitingStateMachine<IdleState, WaitingState>;
/*============================================================================*/
template <typename StateMachine> struct IdleState {
  StateMachine &state_machine_;
  mutable long long counter{0};
  void entry() { LOG_TRACE("Entering IdleState..." << std::endl); }
  void exit() { LOG_TRACE("Exiting IdleState..." << std::endl); }
  MaybeEvent operator()() {
    LOG_TRACE("IdleState called" << std::endl);
    return std::monostate{};
  }
  template <typename DiscardedEvent>
  Nothing handle(const DiscardedEvent &) const {
    LOG_TRACE("IdleState Ignoring event!" << std::endl);
    return {};
  }
  Nothing handle(const std::monostate &) const { return {}; }
  TransitionTo<WaitingState> handle(const SendEvent &send_event) const {
    counter += send_event.a;
    LOG_TRACE("Sending!" << std::endl);
    return {};
  }
};
/*============================================================================*/

template <typename StateMachine> struct WaitingState {
  StateMachine &state_machine_;
  void entry() { LOG_TRACE("Entering WaitingState..." << std::endl); }
  void exit() { LOG_TRACE("Exiting WaitingState..." << std::endl); }
  MaybeEvent operator()() {
    LOG_TRACE("WaitingState called" << std::endl);
    if (state_machine_.timeout) {
      LOG_TRACE("WaitingState Timer ran out!" << std::endl);
      state_machine_.timeout = false;
      return ReceiveEvent{};
    } else
      return std::monostate{};
  }
  template <typename DiscardedEvent>
  Nothing handle(const DiscardedEvent &) const {
    LOG_TRACE("WaitingState Ignoring event!" << std::endl);
    return {};
  }
  Nothing handle(const std::monostate &) const { return {}; }
  TransitionTo<IdleState> handle(const ReceiveEvent &) const {
    LOG_TRACE("Received!" << std::endl);
    return {};
  }
};
/*============================================================================*/
struct Demo {
  WaitingMachine waiting_machine;
  void operator()() {
    waiting_machine();
    waiting_machine.handle(SendEvent{});
    waiting_machine();
    waiting_machine.handle(ReceiveEvent{});
    waiting_machine();
    waiting_machine.handle(SendEvent{});
    waiting_machine();
    waiting_machine();
    waiting_machine();
    waiting_machine();
    waiting_machine.timeout = true;
    waiting_machine();
    waiting_machine();
    waiting_machine();
    waiting_machine.handle(SendEvent{});
    waiting_machine.timeout = true;
    waiting_machine();
  }
  void benchmark_tranistions() {
    waiting_machine.handle(SendEvent{});
    waiting_machine();
    waiting_machine.handle(ReceiveEvent{});
    waiting_machine();
    waiting_machine.handle(SendEvent{});
    waiting_machine();
    waiting_machine.timeout = true;
    waiting_machine();
  }
  static void benchmark_construction() {
    WaitingMachine state_machine;
    state_machine();
  }
};
} // namespace variant_sm