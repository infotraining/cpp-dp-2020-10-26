#define CATCH_CONFIG_RUNNER

#include <string>
#include <catch.hpp>
#include <trompeloeil.hpp>
#include <unordered_map>

#include "application.hpp"

#include "mocks/mock_console.hpp"
#include "mocks/mock_command.hpp"

using namespace std;
using namespace trompeloeil;

TEST_CASE("Application main loop", "[app]")
{
    MockConsole mq_console;
    auto mq_command = std::make_shared<MockCommand>();

    const string cmd_name = "TEST";
    const string cmd_exit = "EXIT";	

    Application app(mq_console);
    app.add_command(cmd_name, mq_command);

    SECTION("gets line from input", "[input]")
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_exit).IN_SEQUENCE(seq);

        app.run();
    }

    SECTION("executes a command", "[cmd]")
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_name).IN_SEQUENCE(seq);
        REQUIRE_CALL(*mq_command, execute()).TIMES(1).IN_SEQUENCE(seq);

        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_exit).IN_SEQUENCE(seq);

        app.run();
    }

    SECTION("name of commands are case insensitive", "[cmd]")
    {
        trompeloeil::sequence seq;

        string caseinsensitive_cmd = "teSt";

        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(caseinsensitive_cmd).IN_SEQUENCE(seq);
        REQUIRE_CALL(*mq_command, execute()).TIMES(1).IN_SEQUENCE(seq);

        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_exit).IN_SEQUENCE(seq);

        app.run();
    }

    SECTION("executes command in loop", "[cmd]")
    {
        trompeloeil::sequence seq;

        ALLOW_CALL(mq_console, print(Messages::msg_prompt));
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_name).TIMES(3).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_exit).IN_SEQUENCE(seq);

        REQUIRE_CALL(*mq_command, execute()).TIMES(3);

        app.run();
    }

    SECTION("exit in command line ends a loop", "[input]")
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_exit).IN_SEQUENCE(seq);
        REQUIRE_CALL(*mq_command, execute()).TIMES(0);

        app.run();
    }

    SECTION("uregistered command shows an error message in console", "[input]")
    {
        trompeloeil::sequence seq;

        const string cmd_unregistered = "UNREGISTERED";
        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_unregistered).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, print(eq(Messages::msg_unknown_cmd + cmd_unregistered))).TIMES(1).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, print(Messages::msg_prompt)).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(cmd_exit).IN_SEQUENCE(seq);

        app.run();
    }
}


int main( int argc, char* const argv[] )
{
    // global setup...

    trompeloeil::set_reporter([](::trompeloeil::severity s,
                                   char const *file,
                                   unsigned long line,
                                   const std::string& msg)
    {
        std::ostringstream os;
        if (line) os << file << ':' << line << '\n';
        os << msg;
        if (s == ::trompeloeil::severity::fatal)
        {
            FAIL(os.str());
        }
        CHECK(os.str() == "");
    });

    int result = Catch::Session().run( argc, argv );

    // global clean-up...

    return result;
}
