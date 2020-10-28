#include <string>

#include <catch.hpp>
#include <trompeloeil.hpp>

#include "command.hpp"
#include "console.hpp"
#include "document.hpp"

#include "mocks/mock_clipboard.hpp"
#include "mocks/mock_command.hpp"
#include "mocks/mock_console.hpp"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

using namespace std;

TEST_CASE("Print command", "[cmd]")
{
    const string content = "abcDEF";
    const string printed_content = "[" + content + "]";

    Document doc{content};
    MockConsole mq_console;

    PrintCmd print_cmd{doc, mq_console};

    SECTION("when executing prints document's content in console")
    {
        REQUIRE_CALL(mq_console, print(printed_content));

        print_cmd.execute();
    }
}

TEST_CASE("Clear command", "[cmd]")
{
    Document doc{"abc"};
    CommandHistory history;

    ClearCmd clear_cmd{doc, history};
    ClearCmd cpy = clear_cmd;

    SECTION("when executing")
    {
        clear_cmd.execute();

        SECTION("clears a document")
        {
            REQUIRE(doc.text() == "");
        }

        SECTION("stores command in a history")
        {
            auto last_cmd = history.pop_last_command();

            REQUIRE(typeid(*(last_cmd)) == typeid(ClearCmd));
        }

        SECTION("undo restores state of a document")
        {
            clear_cmd.undo();

            REQUIRE(doc.text() == "abc");
        }
    }
}

TEST_CASE("ToUpper command", "[cmd]")
{
    const string content = "abcDEF";
    const string content_uppercase = "ABCDEF";

    Document doc{"abcDEF"};
    CommandHistory history;

    ToUpperCmd to_upper_cmd{doc, history};

    SECTION("when executing")
    {
        to_upper_cmd.execute();

        SECTION("converts document to upper case")
        {
            REQUIRE(doc.text() == content_uppercase);
        }

        SECTION("stores command in a history")
        {
            auto last_cmd = history.pop_last_command();

            REQUIRE(typeid(*last_cmd) == typeid(ToUpperCmd));
        }

        SECTION("undo restores state of a document")
        {
            to_upper_cmd.undo();

            REQUIRE(doc.text() == content);
        }
    }
}

TEST_CASE("Paste command", "[cmd]")
{
    const string text = "abc";
    const string clipboard_text = "def";

    Document doc{text};
    MockClipboard mq_clipboard;
    CommandHistory history;

    PasteCmd paste_cmd{doc, mq_clipboard, history};

    REQUIRE_CALL(mq_clipboard, content()).RETURN(clipboard_text);

    SECTION("when executing")
    {
        paste_cmd.execute();

        SECTION("content from clipboard is paste at the end of document")
        {
            auto expected = text + clipboard_text;
            REQUIRE(doc.text() == expected);
        }

        SECTION("command is stored in a history")
        {
            auto last_cmd = history.pop_last_command();

            REQUIRE(typeid(*last_cmd) == typeid(PasteCmd));
        }

        SECTION("undo restores state of the document")
        {
            paste_cmd.undo();

            REQUIRE(doc.text() == text);
        }
    }
}

template <typename MockT>
class UndoableMockProxy : public CloneableCommand<UndoableMockProxy<MockT>>
{
    MockT& mq_;

public:
    UndoableMockProxy(MockT& mq) : mq_{mq}
    {
    }

    void execute() override
    {
        mq_.execute();
    }

    void undo() override
    {
        mq_.undo();
    }
};

TEST_CASE("AddText command", "[cmd]")
{
    const string start_content = "abc";
    const string input_text = "def";

    Document doc{start_content};
    CommandHistory history;
    MockConsole mq_console;

    AddTextCmd add_text_cmd{doc, mq_console, history};

    SECTION("when executing displays prompt in console and reads text")
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(mq_console, print("Write text: ")).IN_SEQUENCE(seq);
        REQUIRE_CALL(mq_console, get_line()).RETURN(input_text).IN_SEQUENCE(seq);

        add_text_cmd.execute();

        SECTION("text is appended to the document")
        {
            auto expected_content = start_content + input_text;
            REQUIRE(doc.text() == expected_content);
        }

        SECTION("command is stored in a history")
        {
            auto last_cmd = history.pop_last_command();

            REQUIRE(typeid(*last_cmd) == typeid(AddTextCmd));
        }

        SECTION("undo restores state of the document")
        {
            add_text_cmd.undo();

            REQUIRE(doc.text() == start_content);
        }
    }
}

TEST_CASE("Undo command", "[cmd]")
{
    MockConsole mq_console;
    CommandHistory history;
    UndoCmd undo_cmd{mq_console, history};

    SECTION("when executing pops last command and calls undo")
    {
        MockUndoableCommand mq_undo_cmd;
        auto mq_proxy = make_unique<UndoableMockProxy<MockUndoableCommand>>(mq_undo_cmd);

        history.record_last_command(std::move(mq_proxy));

        REQUIRE_CALL(mq_undo_cmd, undo());

        undo_cmd.execute();
    }

    SECTION("when history is empty prints info")
    {
        REQUIRE_CALL(mq_console, print("Command history is empty. Nothing to undo."));

        undo_cmd.execute();
    }
}

TEST_CASE("Pop from empty CommandHistory throws an exception", "[cmd][cmd_history]")
{
    CommandHistory history;

    REQUIRE_THROWS_AS(history.pop_last_command(), std::out_of_range);
}

// TODO - uncomment tests

// TEST_CASE("ToLower command", "[cmd]")
// {
//     const string content = "abcDEF";
//     const string content_lowercase = "abcdef";

//     Document doc{"abcDEF"};
//     CommandHistory history;

//     ToLowerCmd to_lower_cmd{doc, history};

//     SECTION("when executing")
//     {
//         to_lower_cmd.execute();

//         SECTION("converts document to lower case")
//         {
//             REQUIRE(doc.text() == content_lowercase);
//         }

//         SECTION("stores command in a history")
//         {
//             auto last_cmd = history.pop_last_command();

//             REQUIRE(typeid(*last_cmd) == typeid(ToLowerCmd));
//         }

//         SECTION("undo restores state of a document")
//         {
//             to_lower_cmd.undo();

//             REQUIRE(doc.text() == content);
//         }
//     }
// }

// TEST_CASE("Copy command", "[cmd]")
// {
//     const string text = "abc";

//     Document doc{text};
//     MockClipboard mq_clipboard;

//     CopyCmd copy_cmd{doc, mq_clipboard};

//     SECTION("executed copies content of a document to a clipboard")
//     {
//         REQUIRE_CALL(mq_clipboard, set_content(text));

//         copy_cmd.execute();
//     }
// }
