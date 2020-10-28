#include <catch.hpp>
#include <trompeloeil.hpp>

#include "document.hpp"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

using namespace std;

TEST_CASE("Document - default constructed", "[doc]")
{
    Document doc;

    SECTION("is empty after creation")
    {
        REQUIRE(doc.text() == "");
    }

    SECTION("length returns zero")
    {
        REQUIRE(doc.length() == 0);
    }
}

TEST_CASE("Document - value constructed", "[doc]")
{
    Document doc{"abc"};

    SECTION("is initialized with value")
    {
        REQUIRE(doc.text() == "abc");
    }

    SECTION("length returns number of characters")
    {
        REQUIRE(doc.length() == 3);
    }
}

struct DocumentTestsFixture
{
    const string abc = "abc";
    Document doc{abc};
};

TEST_CASE_METHOD(DocumentTestsFixture, "Clearing a document", "[doc]")
{
    doc.clear();

    SECTION("sets text to an empty string")
    {
        REQUIRE(doc.text() == "");
    }
}

TEST_CASE_METHOD(DocumentTestsFixture, "Adding text to document", "[doc]")
{
    const string def = "def";
    auto required = abc + def;

    doc.add_text(def);

    SECTION("pushes text at the back")
    {
        REQUIRE(doc.text() == required);
    }

    SECTION("changes length")
    {
        REQUIRE(doc.length() == required.size());
    }
}

TEST_CASE("ToUpper converts text to upper case", "[doc]")
{
    Document doc{"abcDEF"};

    doc.to_upper();

    REQUIRE(doc.text() == "ABCDEF");
}

TEST_CASE("ToLower converts text to lower case", "[doc]")
{
    Document doc{"abcDEF"};

    doc.to_lower();

    REQUIRE(doc.text() == "abcdef");
}

TEST_CASE_METHOD(DocumentTestsFixture, "Replacing text in a document", "[doc]")
{
    doc.replace(0, 2, "def");

    REQUIRE(doc.text() == "defc");
}

TEST_CASE_METHOD(DocumentTestsFixture, "Creates a snapshot(memento)", "[doc]")
{
    REQUIRE(doc.text() == "abc");

    auto snapshot = doc.create_memento();

    SECTION("and restores the previous state")
    {
        doc.clear();

        REQUIRE(doc.text() == "");

        doc.set_memento(snapshot);

        REQUIRE(doc.text() == "abc");
    }
}
