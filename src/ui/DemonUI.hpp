#pragma once

#include <Geode/Geode.hpp>
#include "../data/DemonData.hpp"

using namespace geode::prelude;

namespace demonui {
    inline char const* difficultyIconFrame(DemonCategory category) {
        switch (category) {
            case DemonCategory::Easy: return "difficulty_07_btn_001.png";
            case DemonCategory::Medium: return "difficulty_08_btn_001.png";
            case DemonCategory::Hard: return "difficulty_06_btn_001.png";
            case DemonCategory::Insane: return "difficulty_09_btn_001.png";
            case DemonCategory::Extreme: return "difficulty_10_btn_001.png";
            default: return "difficulty_06_btn_001.png";
        }
    }

    inline char const* categoryName(DemonCategory category) {
        switch (category) {
            case DemonCategory::Easy: return "Easy Demon";
            case DemonCategory::Medium: return "Medium Demon";
            case DemonCategory::Hard: return "Hard Demon";
            case DemonCategory::Insane: return "Insane Demon";
            case DemonCategory::Extreme: return "Extreme Demon";
            default: return "All Demons";
        }
    }

    inline CCSprite* difficultyIcon(DemonCategory category, float scale) {
        auto sprite = CCSprite::createWithSpriteFrameName(difficultyIconFrame(category));
        if (sprite) {
            sprite->setScale(scale);
        }
        return sprite;
    }

    inline cocos2d::extension::CCScale9Sprite* panel(
        CCSize const& size,
        ccColor3B const& color,
        GLubyte opacity = 255
    ) {
        auto bg = cocos2d::extension::CCScale9Sprite::create(
            "square02b_001.png",
            {0.f, 0.f, 80.f, 80.f}
        );
        bg->setContentSize(size);
        bg->setColor(color);
        bg->setOpacity(opacity);
        return bg;
    }
}
