#include "DemonTrackerPopup.hpp"

bool DemonTrackerPopup::init() {
    if (!Popup::init(420.f, 250.f)) {
        return false;
    }

    this->setTitle("Demon Tracker");

    auto hardestLabel = CCLabelBMFont::create("Hardest: ---", "bigFont.fnt");
    hardestLabel->setScale(.55f);
    m_mainLayer->addChildAtPosition(hardestLabel, Anchor::Center, ccp(0.f, 68.f));

    auto countLabel = CCLabelBMFont::create("Demons registrados: 0", "goldFont.fnt");
    countLabel->setScale(.48f);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 45.f));

    this->createCategoryButton("ALL", 0, ccp(0.f, 12.f));
    this->createCategoryButton("EASY", 1, ccp(-100.f, -25.f));
    this->createCategoryButton("MEDIUM", 2, ccp(0.f, -25.f));
    this->createCategoryButton("HARD", 3, ccp(100.f, -25.f));
    this->createCategoryButton("INSANE", 4, ccp(-55.f, -65.f));
    this->createCategoryButton("EXTREME", 5, ccp(55.f, -65.f));

    auto footer = CCLabelBMFont::create("v0.1.0 - interfaz inicial", "goldFont.fnt");
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

    char const* category = "ALL";
    switch (node->getTag()) {
        case 1: category = "EASY DEMONS"; break;
        case 2: category = "MEDIUM DEMONS"; break;
        case 3: category = "HARD DEMONS"; break;
        case 4: category = "INSANE DEMONS"; break;
        case 5: category = "EXTREME DEMONS"; break;
        default: category = "TODOS LOS DEMONS"; break;
    }

    auto message = fmt::format(
        "La lista de <cy>{}</c> se conectara al sistema de ranking en la siguiente etapa.",
        category
    );

    FLAlertLayer::create(
        "Demon Tracker",
        message.c_str(),
        "OK"
    )->show();
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
