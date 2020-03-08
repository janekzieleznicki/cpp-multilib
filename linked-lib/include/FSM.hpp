#include <functional>
#include <iostream>
#include <tuple>
#include <variant>

template <template <typename> typename... States> class WaitingStateMachine {
public:
  template <typename State> void transitionTo() {
    std::visit([](auto statePtr) { statePtr->exit(); }, currentState);
    currentState = &std::get<State>(states);
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
      States<WaitingStateMachine>{*this}...
  };
  std::variant<States<WaitingStateMachine> *...> currentState{&std::get<0>(states)};
};

template <typename State> struct TransitionTo {
  template <typename Machine> void execute_transition(Machine &machine) {
    machine.template transitionTo<State>();
  }
};

struct Nothing {
  template <typename Machine> void execute_transition(Machine &) {}
};

struct OpenEvent {
  int a{100};
};

struct CloseEvent {
  std::string data{"complex, but useless data"};
};

template <typename StateMachine> struct ClosedState;
template <typename StateMachine> struct OpenState;

using Door = WaitingStateMachine<ClosedState, OpenState>;
template <typename StateMachine = Door> struct ClosedState {
  StateMachine &state_machine;
  void entry() {
      state_machine.trace.append("ass");
      std::cout << "Entering closed..." << std::endl;
       }
  void exit() { std::cout << "Exiting closed..." << std::endl; }

  TransitionTo<OpenState<StateMachine>> handle(const OpenEvent &open_event) const {
    std::cout << "Opening the door..." << open_event.a << std::endl;
    return {};
  }

  Nothing handle(const CloseEvent &) const {
    std::cout << "Cannot close. The door is already closed!" << std::endl;
    return {};
  }
};

template <typename StateMachine> struct OpenState {
  StateMachine &state_machine;
  void entry() { std::cout << "Entering open..." << std::endl; }
  void exit() { std::cout << "Exiting open..." << std::endl; }

  Nothing handle(const OpenEvent &) const {
    std::cout << "Cannot open. The door is already open!" << std::endl;
    return {};
  }

  TransitionTo<ClosedState<StateMachine>> handle(const CloseEvent &) const {
    std::cout << "Closing the door..." << std::endl;
    return {};
  }
};