#ifdef COMPILE_VARIANT
 #include "WaitingStateMachine.hpp"
 using SM = variant_sm::WaitingMachine;
 #endif
#ifdef COMPILE_VIRTUAL
 #include "VirtualStateMachine.hpp"
 using SM = virt_sm::StateMachine;
 #endif

int main(){
    SM{};
}