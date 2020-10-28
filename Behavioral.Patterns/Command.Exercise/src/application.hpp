#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <unordered_map>

#include "console.hpp"
#include "command.hpp"

namespace Messages
{
    constexpr auto msg_unknown_cmd = "Unknown command: ";
    constexpr auto msg_prompt = "Enter a command: ";
};

class Application
{
    static const std::string cmd_exit;

    Console& console_;
    std::unordered_map<std::string, CommandSharedPtr> cmds_;

public:
    Application(Console& console) : console_{console}
    {}

    void run()
    {
        while(true)
        {
           console_.print(Messages::msg_prompt);

            auto cmd_text = console_.get_line();
            boost::to_upper(cmd_text);

            if (cmd_text == cmd_exit)
            {
                return;
            }

            try
            {
                cmds_.at(cmd_text)->execute();
            }
            catch(const std::out_of_range&)
            {
                console_.print(Messages::msg_unknown_cmd + cmd_text);
            }
        }
    }

    void add_command(const std::string& name, CommandSharedPtr cmd)
    {
        cmds_.insert(std::make_pair(boost::to_upper_copy(name), cmd));
    }
};

const std::string Application::cmd_exit = "EXIT";

#endif // APPLICATION_HPP
