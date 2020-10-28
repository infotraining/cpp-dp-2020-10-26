#ifndef MOCK_COMMAND_HPP
#define MOCK_COMMAND_HPP

#include <trompeloeil.hpp>

struct MockCommand : public Command
{
    MAKE_MOCK0(execute, void());
};

struct MockUndoableCommand : public UndoableCommand
{
    MAKE_MOCK0(execute, void());
    MAKE_MOCK0(undo, void());
    MAKE_CONST_MOCK0(clone, std::unique_ptr<UndoableCommand>());
};

#endif // MOCK_COMMAND_HPP
