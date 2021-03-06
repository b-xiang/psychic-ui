#include <iostream>
#include <utility>
#include "../components/Menu.hpp"
#include "MenuButton.hpp"

namespace psychic_ui {

    MenuButton::MenuButton(const MenuItem *menuItem) :
        Component(),
        _menuItem(menuItem) {
        setTag("MenuButton");

        _mouseChildren = false; // !important
    }

    const MenuItem *MenuButton::menuItem() const {
        return _menuItem;
    }

    bool MenuButton::selected() const {
        return _selected;
    }

    void MenuButton::setSelected(const bool selected) {
        if (_selected != selected) {
            _selected = selected;
            invalidateStyle();
        }
    }

    bool MenuButton::active() const {
        return Component::active() || _selected;
    };
}
