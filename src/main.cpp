#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "ui/DemonTrackerPopup.hpp"

using namespace geode::prelude;

class $modify(DemonTrackerMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        log::info("Demon Tracker v0.1.0 loaded successfully.");

        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
            this,
            menu_selector(DemonTrackerMenuLayer::onDemonTracker)
        );

        if (auto menu = this->getChildByID("bottom-menu")) {
            button->setID("demon-tracker-button"_spr);
            menu->addChild(button);
            menu->updateLayout();
        }

        return true;
    }

    void onDemonTracker(CCObject*) {
        if (auto popup = DemonTrackerPopup::create()) {
            popup->show();
        }
    }
};
