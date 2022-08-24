#include <catch2/catch.hpp>
#include "../UI/Textbox.hpp"
#include "../UI/UISystem.hpp"
#include "../UI/TokenUI.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Edit TextBox", "[EditTextbox]"){
    auto ui = UISystem({0,0});
    auto text = "text";
    std::string str = text;
    auto tb = ui.create_widget<Textbox>(ui.get_root(), &str);
    tb->event_text_input('a');
    REQUIRE_THAT(str, Equals("texta"));
    tb->event_text_input('\b');
    tb->event_text_input('\b');
    REQUIRE_THAT(str, Equals("tex"));
}
