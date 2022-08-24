#include <catch2/catch.hpp>
#include "../UI/Textbox.hpp"
#include "../UI/UISystem.hpp"
#include "../UI/TokenUI.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Edit TextBox", "[EditTextbox]"){
    auto ui = UISystem({0,0});
    std::string text = "text";
    StrGetter g = [&text](){return text;};
    StrSetter s = [&text](auto str){text = str;};
    auto tb = ui.create_widget<Textbox>(ui.get_root(), StrBinding{g, s});
    tb->event_text_input('a');
    REQUIRE_THAT(text, Equals("texta"));
    tb->event_text_input('\b');
    tb->event_text_input('\b');
    REQUIRE_THAT(text, Equals("tex"));
}
