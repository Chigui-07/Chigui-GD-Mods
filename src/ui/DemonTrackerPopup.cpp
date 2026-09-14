#include "DemonTrackerPopup.hpp"
#include "DemonListPopup.hpp"
#include "../data/DemonData.hpp"

bool DemonTrackerPopup::init() {
    if (!Popup::init(420.f, 250.f)) {
        return false;
    }

    initializeDemonData();
    this->setTitle("Demon Tracker");

    auto const& demons = getDemonEntries();
    auto hardestText = demons.empty()
        ? std::string("Hardest: ---")
        : fmt::format("Hardest: {}", demons.front().name);

    auto hardestLabel = CCLabelBMFont::create(hardestText.c_str(), "bigFont.fnt");
    hardestLabel->setScale(.55f);
    hardestLabel->limitLabelWidth(330.f, .55f, .30f);
    m_mainLayer->addChildAtPosition(hardestLabel, Anchor::Center, ccp(0.f, 68.f));

    auto countText = fmt::format("Demons registrados: {}", demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.48f);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 45.f));

    this->createCategoryButton("ALL", 0, ccp(0.f, 12.f));
    this->createCategoryButton("EASY", 1, ccp(-100.f, -25.f));
    this->createCategoryButton("MEDIUM", 2, ccp(0.f, -25.f));
    this->createCategoryButton("HARD", 3, ccp(100.f, -25.f));
    this->createCategoryButton("INSANE", 4, ccp(-55.f, -65.f));
    this->createCategoryButton("EXTREME", 5, ccp(55.f, -65.f));

    auto footer = CCLabelBMFont::create("v0.2.0 - ranking automatico", "goldFont.fnt");
    footer->setScale(.32f);
    footer->setOpacity(150);
    m_mainLayer->addChildAtPosition(footer, Anchor::Bottom, ccp(0.f, 12.f));

    return true;
}

CCMenuItemSpriteExtra* DemonTrackerPopup::createCategoryButton(
    char const* text,
    int tag,
    CCPoint const& offset
) {
    auto sprite = ButtonSprite::create(
        text,
        "bigFont.fnt",
        "GJ_button_01.png",
        .8f
    );
    sprite->setScale(.58f);

    auto button = CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(DemonTrackerPopup::onCategory)
    );
    button->setTag(tag);
    m_buttonMenu->addChildAtPosition(button, Anchor::Center, offset);

    return button;
}

void DemonTrackerPopup::onCategory(CCObject* sender) {
    auto node = static_cast<CCNode*>(sender);
    auto category = DemonCategory::All;

    switch (node->getTag()) {
        case 1: category = DemonCategory::Easy; break;
        case 2: category = DemonCategory::Medium; break;
        case 3: category = DemonCategory::Hard; break;
        case 4: category = DemonCategory::Insane; break;
        case 5: category = DemonCategory::Extreme; break;
        default: category = DemonCategory::All; break;
    }

    if (auto popup = DemonListPopup::create(category)) {
        popup->show();
    }
}

DemonTrackerPopup* DemonTrackerPopup::create() {
    auto ret = new DemonTrackerPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
