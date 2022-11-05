#pragma once
#include <memory.h>

#include <numeric>

#include "../Colors.hpp"
#include "../UIElement.hpp"

/**
 * @brief This class can be used to display a set of tools the user can interact with.
 * See also WTool.hpp
 */
class WToolbar : public UIElement {
 public:
  WToolbar(UISystem* ui, std::weak_ptr<UIElement> parent)
      : UIElement(ui, parent, "Toolbar") {
    update_size();
  }

  /**
   * @brief Set the tools (UIElements, WTools, or a subclass thereof) to be displayed.
   *
   * @tparam T Any container type that supports range-based for loops
   * @param tools The tools we want to add to this toolbar
   */
  template <class T>
  void set_tools(T tools) {
    for (std::shared_ptr<UIElement> tool : tools) {
      add_child(tool);
    }
    reposition_children();
    update_size();
  }

  /**
   * @brief Set the position of every children to match the set layout.
   */
  void reposition_children() {
    sf::Vector2i toolbarPos = get_parent_position() + relativePosition;
    sf::Vector2i childPos(xspace, yspace);
    for (auto&& child : children) {
      child->update_position(childPos);
      childPos.x += child->size.x + xspace;
    }
  }

  /**
   * @brief Draw the background of this widget to fit every new child.
   */
  void update_size() {
    auto heightComparator = [](std::shared_ptr<UIElement> a,
                               std::shared_ptr<UIElement> b) {
      return a->size.y - b->size.y;
    };
    int maxHeight = 0;
    int childWidth = 0;
    for (auto&& child : children) {
      auto& [childX, childY] = child->size;
      maxHeight = std::max(maxHeight, childY);
      childWidth += childX;
    }
    int width = childWidth + (children.size() + 1) * xspace;
    int height = maxHeight + 2 * yspace;

    renderTex = std::make_unique<sf::RenderTexture>();
    renderTex->create(width, height);
    renderTex->clear(colors::STANDARD_DARK);
    renderTex->display();
    spriteTex = std::make_unique<sf::Texture>(renderTex->getTexture());
    sprite.setTexture(*spriteTex, true);
  }

  int xspace = 2;  // Configures the border width around every tool widget
  int yspace = 5;  // Configures the border height around every tool widget

 private:
  using UIElement::add_child;
  std::unique_ptr<sf::RenderTexture> renderTex;
  std::unique_ptr<sf::Texture> spriteTex;
};
