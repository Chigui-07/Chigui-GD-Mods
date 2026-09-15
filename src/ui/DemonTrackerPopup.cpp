#include "DemonTrackerPopup.hpp"
#include "DemonListPopup.hpp"
#include "../data/DemonData.hpp"

namespace {
    CCLayerColor* makePanel(CCSize const& size, ccColor4B const& color) {
        auto panel = CCLayerColor::create(color, size.width, size.height);
        panel->ignoreAnchorPointForPosition(false);
        panel->setAnchorPoint({.5f, .5f});
        return panel;
    }

    char const* categoryButtonTexture(int tag) {
        switch (tag) {
            case 1: return "GJ_button_01.png"; // Easy
            case 2: return "GJ_button_06.png"; // Medium
            case 3: return "GJ_button_06.png"; // Hard
            case 4: return "GJ_button_05.png"; // Insane
            case 5: return "GJ_button_02.png"; // Extreme
            default: return "GJ_button_04.png"; // All
        }
    }
}

bool DemonTrackerPopup::init() {
    if (!Popup::init(440.f, 275.f)) {
        return false;
    }

    initializeDemonData();
    this->setTitle("Demon Tracker");

    auto const& demons = getDemonEntries();

    auto heroPanel = makePanel({370.f, 62.f}, ccc4(24, 44, 72, 115));
    m_mainLayer->addChildAtPosition(heroPanel, Anchor::Center, ccp(0.f, 56.f));

    auto hardestText = demons.empty()
        ? std::string("Hardest: ---")
        : fmt::format("Hardest: {}", demons.front().name);

    auto hardestLabel = CCLabelBMFont::create(hardestText.c_str(), "bigFont.fnt");
    hardestLabel->setScale(.54f);
    hardestLabel->limitLabelWidth(330.f, .54f, .30f);
    m_mainLayer->addChildAtPosition(hardestLabel, Anchor::Center, ccp(0.f, 67.f));

    auto countText = fmt::format("{} demons registrados", demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.39f);
    countLabel->setOpacity(220);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 43.f));

    auto sectionLabel = CCLabelBMFont::create("LISTAS DE DEMONS", "goldFont.fnt");
    sectionLabel->setScale(.30f);
    sectionLabel->setOpacity(180);
    m_mainLayer->addChildAtPosition(sectionLabel, Anchor::Center, ccp(0.f, 15.f));

    this->createCategoryButton("ALL", 0, ccp(-120.f, -11.f));
    this->createCategoryButton("EASY", 1, ccp(0.f, -11.f));
    this->createCategoryButton("MEDIUM", 2, ccp(120.f, -11.f));
    this->createCategoryButton("HARD", 3, ccp(-120.f, -50.f));
    this->createCategoryButton("INSANE", 4, ccp(0.f, -50.f));
    this->createCategoryButton("EXTREME", 5, ccp(120.f, -50.f));

    auto footer = CCLabelBMFont::create("v0.3.0 - visual refresh", "goldFont.fnt");
    footer->setScale(.28f);
    footer->setOpacity(130);
    m_mainLayer->addChildAtPosition(footer, Anchor::Bottom, ccp(0.f, 10.f));

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
        categoryButtonTexture(tag),
        .8f
    );
    sprite->setScale(.54f);

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
