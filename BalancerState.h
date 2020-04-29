#ifndef BalancerState_h
#define BalancerState_h

enum class State { NoState, ZeroState, ToZeroState, ToGroundState, ToFinalState, BalancedState, BalancingState, GroundState, FinalState, ErrorState };
enum class BalancingState { NotBalancing, Balancing, Done, Error };

#endif
