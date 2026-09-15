#include "DemonTrackerPopup.hpp"
#include "DemonListPopup.hpp"
#include "DemonUI.hpp"
#include "../data/DemonData.hpp"

bool DemonTrackerPopup::init() {
    if (!Popup::init(440.f, 285.f)) {
        return false;
    }

    initializeDemonData();
    this->setTitle("Demon Tracker");

    auto const& demons = getDemonEntries();

    auto heroPanel = demonui::panel({365.f, 64.f}, ccc3(91, 48, 31), 235);
    m_mainLayer->addChildAtPosition(heroPanel, Anchor::Center, ccp(0.f, 61.f));

    if (!demons.empty()) {
        if (auto icon = demonui::difficultyIcon(demons.front().category, .42f)) {
            m_mainLayer->addChildAtPosition(icon, Anchor::Center, ccp(-147.f, 61.f));
        }
    }

    auto hardestText = demons.empty()
        ? std::string("Hardest: ---")
        : fmt::format("Hardest: {}", demons.front().name);

    auto hardestLabel = CCLabelBMFont::create(hardestText.c_str(), "bigFont.fnt");
    hardestLabel->setScale(.49f);
    hardestLabel->limitLabelWidth(285.f, .49f, .28f);
    m_mainLayer->addChildAtPosition(hardestLabel, Anchor::Center, ccp(15.f, 69.f));

    auto countText = fmt::format("{} demons registrados", demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.35f);
    countLabel->setOpacity(220);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(15.f, 47.f));

    auto sectionLabel = CCLabelBMFont::create("LISTAS DE DEMONS", "goldFont.fnt");
    sectionLabel->setScale(.30f);
    sectionLabel->setOpacity(185);
    m_mainLayer->addChildAtPosition(sectionLabel, Anchor::Center, ccp(0.f, 18.f));

    this->createCategoryButton("ALL", 0, ccp(-150.f, -25.f));
    this->createCategoryButton("EASY", 1, ccp(-88.f, -25.f));
    this->createCategoryButton("MEDIUM", 2, ccp(-29.f, -25.f));
    this->createCategoryButton("HARD", 3, ccp(30.f, -25.f));
    this->createCategoryButton("INSANE", 4, ccp(89.f, -25.f));
    this->createCategoryButton("EXTREME", 5, ccp(150.f, -25.f));

    auto hint = CCLabelBMFont::create("Toca una dificultad para abrir su ranking", "goldFont.fnt");
    hint->setScale(.24f);
    hint->setOpacity(135);
    m_mainLayer->addChildAtPosition(hint, Anchor::Center, ccp(0.f, -76.f));

    auto footer = CCLabelBMFont::create("v0.3.1 - GD style pass", "goldFont.fnt");
    footer->setScale(.26f);
    footer->setOpacity(115);
    m_mainLayer->addChildAtPosition(footer, Anchor::Bottom, ccp(0.f, 9.f));

    return true;
}

CCMenuItemSpriteExtra* DemonTrackerPopup::createCategoryButton(
    char const* text,
    int tag,
    CCPoint const& offset
) {
    CCNode* visual = nullptr;

    if (tag == 0) {
        auto allSprite = ButtonSprite::create(
            "ALL",
            "bigFont.fnt",
            "GJ_button_04.png",
            .8f
        );
        allSprite->setScale(.42f);
        visual = allSprite;
    }
    else {
        auto holder = CCNode::create();
        holder->setContentSize({54.f, 64.f});
        holder->setAnchorPoint({.5f, .5f});
        holder->ignoreAnchorPointForPosition(false);

        auto category = DemonCategory::Easy;
        switch (tag) {
            case 2: category = DemonCategory::Medium; break;
            case 3: category = DemonCategory::Hard; break;
            case 4: category = DemonCategory::Insane; break;
            case 5: category = DemonCategory::Extreme; break;
            default: category = DemonCategory::Easy; break;
        }

        if (auto icon = demonui::difficultyIcon(category, .38f)) {
            icon->setPosition({27.f, 39.f});
            holder->addChild(icon);
        }

        auto label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setScale(.22f);
        label->setPosition({27.f, 8.f});
        holder->addChild(label);

        visual = holder;
    }

    auto button = CCMenuItemSpriteExtra::create(
        visual,
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
