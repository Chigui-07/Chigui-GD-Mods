#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class DemonTrackerPopup : public Popup {
protected:
    bool init() override;

    CCMenuItemSpriteExtra* createCategoryButton(
        char const* text,
        int tag,
        CCPoint const& offset
    );

    void onCategory(CCObject* sender);

public:
    static DemonTrackerPopup* create();
};
