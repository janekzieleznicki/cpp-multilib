#pragma once
#include "Logger.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
namespace virt_sm {
/*============================================================================*/
enum class EventType { SENDEVENT, RECEIVEEVENT, TIMEOUTEVENT };
struct BaseEvent {
  virtual EventType get_type() const = 0;
  virtual ~BaseEvent() = default;
};
struct SendEvent final : BaseEvent {

  virtual EventType get_type() const override { return EventType::SENDEVENT; }
  int a{100};
};

struct ReceiveEvent final : BaseEvent {
  virtual EventType get_type() const override {
    return EventType::RECEIVEEVENT;
  }
  std::string data{"complex, but useless data"};
};

struct TimeoutEvent final : BaseEvent {
  virtual EventType get_type() const override {
    return EventType::TIMEOUTEVENT;
  }
  std::string data{"complex, but useless data"};
};

/*============================================================================*/
enum class StateType { IDLESTATE, WAITINGSTATE };
class StateMachine;
struct BaseState {
  virtual ~BaseState() = default;
  virtual StateType get_type() const = 0;
  virtual std::optional<StateType> handle(const BaseEvent &) const = 0;
  virtual std::unique_ptr<BaseEvent> operator()() const = 0;
  virtual void entry() = 0;
  virtual void exit() = 0;
};
/*============================================================================*/
struct IdleState final : BaseState {
  StateMachine &state_machine_;
  mutable long long counter{0};
  IdleState(StateMachine &state_machine) : state_machine_{state_machine} {}
  StateType get_type() const override { return StateType::IDLESTATE; }
  void entry() override { LOG_TRACE("Entering IdleState..." << std::endl); }
  void exit() override { LOG_TRACE("Exiting IdleState..." << std::endl); }
  std::unique_ptr<BaseEvent> operator()() const override {
    LOG_TRACE("IdleState called" << std::endl);
    return nullptr;
  }
  std::optional<StateType> handle(const BaseEvent &event) const override {
    switch (event.get_type()) {
    case EventType::SENDEVENT:
      LOG_TRACE("Sending!" << std::endl);
      counter += static_cast<const SendEvent &>(event).a;
      return StateType::WAITINGSTATE;
    default:
      LOG_TRACE("IdleState Ignoring event!" << std::endl);
      return std::nullopt;
    }
  }
};
/*============================================================================*/
struct WaitingState final : BaseState {
  StateMachine &state_machine_;

  WaitingState(StateMachine &state_machine) : state_machine_{state_machine} {}
  StateType get_type() const override { return StateType::WAITINGSTATE; }
  void entry() override { LOG_TRACE("Entering WaitingState..." << std::endl); }
  void exit() override { LOG_TRACE("Exiting WaitingState..." << std::endl); }
  std::unique_ptr<BaseEvent> operator()() const override;
  std::optional<StateType> handle(const BaseEvent &event) const override {
    switch (event.get_type()) {
    case EventType::RECEIVEEVENT:
      LOG_TRACE("Received!" << std::endl);
      return StateType::IDLESTATE;
    case EventType::TIMEOUTEVENT:
      LOG_TRACE("TimedOut!" << std::endl);
      return StateType::IDLESTATE;
    default:
      LOG_TRACE("WaitingState Ignoring event!" << std::endl);
      return std::nullopt;
    }
  }
};
/*============================================================================*/

class StateMachine {
public:
  bool timeout{false};
  StateMachine() {
    states.emplace(StateType::IDLESTATE,std::make_unique<IdleState>(*this));
    states.emplace(StateType::WAITINGSTATE,std::make_unique<WaitingState>(*this));
    current_state = states.at(StateType::IDLESTATE).get();
  }
  /**
   * Return true if transition occured
   */
  bool operator()() {
    auto event = (*current_state)();
    if (event.get()) {
      handle(*event);
      return true;
    }
    return false;
  }
  void transition_to(const StateType &state_type) {
    current_state->exit();
    current_state = states.at(state_type).get();
    current_state->entry();
  }
  void handle(const BaseEvent &event) {
    if (auto new_state = current_state->handle(event); new_state)
      transition_to(*new_state);
  }

private:
  BaseState *current_state{nullptr};
  std::map<StateType, std::unique_ptr<BaseState>> states;
};
std::unique_ptr<BaseEvent> WaitingState::operator()() const {
  LOG_TRACE("WaitingState called" << std::endl);
  if (state_machine_.timeout) {
    LOG_TRACE("WaitingState Timer ran out!" << std::endl);
    state_machine_.timeout = false;
    return std::make_unique<TimeoutEvent>();
  }
  return nullptr;
}
/*============================================================================*/
struct Demo {
  StateMachine waiting_machine;
  void operator()() {
    while (waiting_machine())
      ;
    waiting_machine.handle(SendEvent{});
    while (waiting_machine())
      ;
    waiting_machine.handle(ReceiveEvent{});
    while (waiting_machine())
      ;
    waiting_machine.handle(SendEvent{});
    while (waiting_machine())
      ;
    while (waiting_machine())
      ;
    while (waiting_machine())
      ;
    while (waiting_machine())
      ;
    waiting_machine.timeout = true;
    while (waiting_machine())
      ;
    while (waiting_machine())
      ;
    while (waiting_machine())
      ;
    waiting_machine.handle(SendEvent{});
    waiting_machine.timeout = true;
    while (waiting_machine())
      ;
  }
  void benchmark_tranistions() {
    waiting_machine.handle(SendEvent{});
    while (waiting_machine())
      ;
    waiting_machine.handle(ReceiveEvent{});
    while (waiting_machine())
      ;
    waiting_machine.handle(SendEvent{});
    while (waiting_machine())
      ;
    waiting_machine.timeout = true;
    while (waiting_machine())
      ;
  }
  void benchmark_ignored_event() { waiting_machine.handle(ReceiveEvent{}); }
  static void benchmark_construction() {
    StateMachine state_machine;
    state_machine();
  }
};
} // namespace virt_sm