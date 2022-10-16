#include <catch2/catch.hpp>
#include "../UI/WTextbox.hpp"
#include "../UI/UISystem.hpp"
#include "../UI/TokenUI.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Edit TextBox", "[EditTextbox]"){
    auto ui = UISystem({0,0});
    std::string text = "text";
    GetterFn<std::string> g = [&text](){return text;};
    SetterFn<std::string> s = [&text](auto str){text = str;};
    auto tb = ui.create_widget<WTextbox>(ui.get_root(), Binding<std::string>{g, s}, sf::Vector2i{10,10}, sf::Vector2i{0,0});
    tb->event_clicked();
    tb->event_text_input('a');
    REQUIRE_THAT(text, Equals("texta"));
    tb->event_text_input('\b');
    tb->event_text_input('\b');
    REQUIRE_THAT(text, Equals("tex"));
}

TEST_CASE("Mouse Inside", "[IsMouseInside]"){
    auto ui = UISystem({400,400});
    auto ele = ui.create_widget<UIElement>(ui.get_root(), "test_uielement", sf::Vector2i{10,10}, sf::Vector2i{10,10});
    auto tb = ui.create_widget<WTextbox>(ele, Binding<std::string>{}, sf::Vector2i{10,10}, sf::Vector2i{10,10});
    LOGDBG("", fmt::format("{},{}\n", tb->get_parent_position().x, tb->get_parent_position().y));
    REQUIRE(tb->is_mouse_inside({25,25}));
}
