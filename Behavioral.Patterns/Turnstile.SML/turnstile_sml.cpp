#include <iostream>
#include <random>

#include "boost/sml.hpp"

namespace sml = boost::sml;

using namespace std;

// events
struct Coin
{
};

struct Pass
{
};

// transition table
struct TurnstileTransitions
{
    auto operator()() const noexcept
    {
        using namespace sml;

        // clang-format off

        return make_transition_table(
            *"Start"_s / [] { cout << "Starting turnstile..." << endl; } = "Locked"_s,
             "Locked"_s + event<Coin> / [] { cout << "Unlock" << endl; } = "Unlocked"_s,
             "Locked"_s + event<Pass> / [] { cout << "Alarm" << endl; },
             "Unlocked"_s + event<Coin> / [] { cout << "Thank you" << endl; },
             "Unlocked"_s + event<Pass> / [] { cout << "Lock" << endl; } = "Locked"_s);

        // clang-format on
    }
};

class TurnstileFSM
{
    sml::sm<TurnstileTransitions> turnstile_fsm_;

public:
    void coin()
    {
        auto coin_event = Coin{};
        turnstile_fsm_.process_event(coin_event);
    }

    void pass()
    {
        turnstile_fsm_.process_event(Pass{});
    }
};

int main()
{
    TurnstileFSM turnstile;

    turnstile.coin();
    turnstile.coin();
    turnstile.pass();
    turnstile.coin();
    turnstile.coin();
    turnstile.pass();
    turnstile.pass();
    turnstile.pass();
    turnstile.coin();
}