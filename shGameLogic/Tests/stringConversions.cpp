//
// Created by tim on 23.06.20.
//

#include <gtest/gtest.h>
#include <string>

#include "enumsToString.h"

TEST(string_conversions_test, all) {
    using namespace sh;
    using namespace sh::util::strings;
    const std::string fasc = "Fascist";
    const std::string lib = "Liberal";
    const std::string pres = "President";
    const std::string chancellor = "Chancellor";

    EXPECT_EQ(toString(*toCardType(fasc)), fasc);
    EXPECT_EQ(toString(*toCardType(lib)), lib);

    EXPECT_EQ(toString(*toGovRole(pres)), pres);
    EXPECT_EQ(toString(*toGovRole(chancellor)), chancellor);

    EXPECT_EQ(toString(*toPlayerType(lib)), lib);
    EXPECT_EQ(toString(*toPlayerType(fasc)), fasc);
}

TEST(string_conversions_test, special_player_role) {
    using namespace sh;
    using namespace sh::util::strings;
    const std::string hitler = "Hitler";
    const std::string fasc = "Fascist";
    EXPECT_EQ(toString(*toPlayerType(hitler)), fasc);
    EXPECT_EQ(toString(*toPlayerType(hitler), true), fasc);
}

TEST(string_conversions_test, not_found) {
    using namespace sh;
    using namespace sh::util::strings;
    EXPECT_FALSE(toCardType("gibberish").has_value());
    EXPECT_FALSE(toGovRole("gibberish").has_value());
    EXPECT_FALSE(toPlayerType("gibberish").has_value());
}

