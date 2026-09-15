#include "DemonTrackerPopup.hpp"
#include "DemonListPopup.hpp"
#include "DemonUI.hpp"
#include "../data/DemonData.hpp"

namespace {
    DemonCategory categoryForTag(int tag) {
        switch (tag) {
            case 1: return DemonCategory::Easy;
            case 2: return DemonCategory::Medium;
            case 3: return DemonCategory::Hard;
            case 4: return DemonCategory::Insane;
            case 5: return DemonCategory::Extreme;
            default: return DemonCategory::All;
        }
    }
}

bool DemonTrackerPopup::init() {
    if (!Popup::init(448.f, 300.f)) {
        return false;
    }

    initializeDemonData();
    this->setTitle("Demon Tracker");

    auto const& demons = getDemonEntries();

    auto heroPanel = demonui::panel({374.f, 64.f}, ccc3(91, 48, 31), 238);
    m_mainLayer->addChildAtPosition(heroPanel, Anchor::Center, ccp(0.f, 76.f));

    if (!demons.empty()) {
        if (auto icon = demonui::difficultyIcon(demons.front().category, .43f)) {
            m_mainLayer->addChildAtPosition(icon, Anchor::Center, ccp(-151.f, 76.f));
        }
    }
    else {
        if (auto icon = demonui::difficultyIcon(DemonCategory::Hard, .43f)) {
            icon->setOpacity(120);
            m_mainLayer->addChildAtPosition(icon, Anchor::Center, ccp(-151.f, 76.f));
        }
    }

    auto hardestText = demons.empty()
        ? std::string("Hardest: ---")
        : fmt::format("Hardest: {}", demons.front().name);

    auto hardestLabel = CCLabelBMFont::create(hardestText.c_str(), "bigFont.fnt");
    hardestLabel->setScale(.49f);
    hardestLabel->limitLabelWidth(280.f, .49f, .27f);
    m_mainLayer->addChildAtPosition(hardestLabel, Anchor::Center, ccp(17.f, 84.f));

    auto countText = fmt::format("{} demons registrados", demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.34f);
    countLabel->setOpacity(215);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(17.f, 61.f));

    auto sectionLabel = CCLabelBMFont::create("LISTAS DE DEMONS", "goldFont.fnt");
    sectionLabel->setScale(.30f);
    sectionLabel->setOpacity(185);
    m_mainLayer->addChildAtPosition(sectionLabel, Anchor::Center, ccp(0.f, 31.f));

    this->createCategoryButton("ALL", 0, ccp(-112.f, -13.f));
    this->createCategoryButton("EASY", 1, ccp(0.f, -13.f));
    this->createCategoryButton("MEDIUM", 2, ccp(112.f, -13.f));
    this->createCategoryButton("HARD", 3, ccp(-112.f, -72.f));
    this->createCategoryButton("INSANE", 4, ccp(0.f, -72.f));
    this->createCategoryButton("EXTREME", 5, ccp(112.f, -72.f));

    auto footer = CCLabelBMFont::create("v1.0.0", "goldFont.fnt");
    footer->setScale(.25f);
    footer->setOpacity(105);
    m_mainLayer->addChildAtPosition(footer, Anchor::Bottom, ccp(0.f, 8.f));

    return true;
}

CCMenuItemSpriteExtra* DemonTrackerPopup::createCategoryButton(
    char const* text,
    int tag,
    CCPoint const& offset
) {
    auto category = categoryForTag(tag);

    auto holder = CCNode::create();
    holder->setContentSize({104.f, 50.f});
    holder->setAnchorPoint({.5f, .5f});
    holder->ignoreAnchorPointForPosition(false);

    auto bg = demonui::panel({100.f, 46.f}, ccc3(99, 55, 35), 242);
    bg->setPosition({52.f, 25.f});
    holder->addChild(bg);

    auto iconCategory = tag == 0 ? DemonCategory::Hard : category;
    if (auto icon = demonui::difficultyIcon(iconCategory, .25f)) {
        icon->setPosition({24.f, 25.f});
        holder->addChild(icon);
    }

    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    label->setScale(.25f);
    label->setPosition({67.f, 31.f});
    holder->addChild(label);

    auto count = getDemonEntries(category).size();
    auto countText = fmt::format("{}", count);
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.22f);
    countLabel->setOpacity(180);
    countLabel->setPosition({67.f, 15.f});
    holder->addChild(countLabel);

    auto button = CCMenuItemSpriteExtra::create(
        holder,
        this,
        menu_selector(DemonTrackerPopup::onCategory)
    );
    button->setTag(tag);
    m_buttonMenu->addChildAtPosition(button, Anchor::Center, offset);
    return button;
}

void DemonTrackerPopup::onCategory(CCObject* sender) {
    auto node = static_cast<CCNode*>(sender);
    auto category = categoryForTag(node->getTag());

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
