#include <iostream>
#include "StyleManager.hpp"
#include "psychicui/utils/StringUtils.hpp"

namespace psychicui {

    std::shared_ptr<StyleManager> StyleManager::instance{nullptr};

    std::shared_ptr<StyleManager> StyleManager::getInstance() {
        if (!instance) {
            instance = std::make_shared<StyleManager>();
        }
        return instance;
    }

    void StyleManager::loadFont(const std::string &name, const std::string &path) {
        _fonts[name] = SkTypeface::MakeFromFile(path.c_str());
    }

    sk_sp<SkTypeface> StyleManager::font(const std::string &name) const {
        auto font = _fonts.find(name);
        return font != _fonts.end() ? font->second : nullptr;
    }

    Style *StyleManager::style(std::string selector) {
        std::transform(selector.begin(), selector.end(), selector.begin(), ::tolower);
        auto it = _declarations.find(selector);
        if (it != _declarations.end()) {
            return it->second->style();
        } else {
            auto rule = Rule::fromSelector(selector);
            if (!rule) {
                std::cerr << "Invalid selector: \"" << selector << "\", returning dummy style.";
                return Style::dummyStyle.get();
            }

            _declarations[selector] = std::make_unique<StyleDeclaration>(std::move(rule));

            #ifdef DEBUG_STYLES
            _declarations[selector]->selector = selector;
            #endif

            return _declarations[selector]->style();
        }
    }

    std::unique_ptr<Style> StyleManager::computeStyle(const Component *component) {
        std::vector<std::pair<int, StyleDeclaration*>> inheritedMatches;
        std::vector<std::pair<int, StyleDeclaration*>> directMatches;

        for (const auto &declaration: _declarations) {
            int match = declaration.second->rule()->matches(component);
            if (match == 0) {
                directMatches.emplace_back(declaration.second->weight(), declaration.second.get());
            } else if (match > 0) {
                inheritedMatches.emplace_back(match, declaration.second.get());
            }
        }

        // Sort direct matches by weight, heaviest overlaid last
        std::sort(directMatches.begin(), directMatches.end(), [](const auto &a, const auto &b) {
            return a.first < b.first;
        });

        // Sort inherited matches by match depth, closest overlaid last
        // In case of equal depth, sort using weight
        std::sort(inheritedMatches.begin(), inheritedMatches.end(), [](const auto &a, const auto &b) {
            return (a.first != b.first) ? a.first > b.first : a.second->weight() < b.second->weight();
        });

        auto s = std::make_unique<Style>(style("*"));
        // Computed inherited values
        for (const auto &inheritedMatch: inheritedMatches) {
            s->overlayInheritable(inheritedMatch.second->style());
            #ifdef DEBUG_STYLES
            s->declarations.push_back("[inherited, depth: " + std::to_string(inheritedMatch.first) + "] " + inheritedMatch.second->selector);
            #endif
        }
        for (const auto &directMatch: directMatches) {
            s->overlay(directMatch.second->style());
            #ifdef DEBUG_STYLES
            s->declarations.push_back("[weight: " + std::to_string(directMatch.first) + "] " + directMatch.second->selector);
            #endif
        }

        return std::move(s);
    }

}