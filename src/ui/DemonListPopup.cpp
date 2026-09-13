#include "DemonListPopup.hpp"
#include "DemonDetailPopup.hpp"
#include <Geode/ui/ScrollLayer.hpp>

bool DemonListPopup::init(DemonCategory category) {
    if (!Popup::init(420.f, 260.f)) {
        return false;
    }

    m_category = category;
    m_demons = getDemonEntries(category);
    this->setTitle(getDemonCategoryTitle(category));

    auto countText = fmt::format("Demons registrados: {}", m_demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.42f);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 82.f));

    auto list = ScrollLayer::create({350.f, 175.f});
    list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(3.f));
    list->setTouchEnabled(true);
    list->setStealingTouches(true);

    if (m_demons.empty()) {
        auto emptyRow = CCNode::create();
        emptyRow->setContentSize({340.f, 36.f});

        auto emptyLabel = CCLabelBMFont::create(
            "Todavia no hay demons registrados.",
            "goldFont.fnt"
        );
        emptyLabel->setScale(.38f);
        emptyLabel->setOpacity(190);
        emptyLabel->setPosition({170.f, 18.f});
        emptyRow->addChild(emptyLabel);
        list->m_contentLayer->addChild(emptyRow);
    }
    else {
        for (size_t i = 0; i < m_demons.size(); ++i) {
            list->m_contentLayer->addChild(
                this->createDemonRow(
                    static_cast<int>(i) + 1,
                    static_cast<int>(i),
                    *m_demons[i],
                    340.f
                )
            );
        }
    }

    list->m_contentLayer->updateLayout();
    list->scrollToTop();

    auto listSize = list->getScaledContentSize();
    m_mainLayer->addChildAtPosition(
        list,
        Anchor::Center,
        ccp(-listSize.width / 2.f, -listSize.height / 2.f - 12.f)
    );

    return true;
}

CCNode* DemonListPopup::createDemonRow(
    int rank,
    int index,
    DemonEntry const& demon,
    float width
) {
    auto row = CCMenu::create();
    row->setContentSize({width, 25.f});
    row->setAnchorPoint({0.f, 0.f});
    row->setPosition({0.f, 0.f});

    auto rowText = fmt::format("#{}  {}", rank, demon.name);
    auto sprite = ButtonSprite::create(
        rowText.c_str(),
        "bigFont.fnt",
        "GJ_button_01.png",
        .8f
    );
    sprite->setScale(.45f);

    auto button = CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(DemonListPopup::onDemonSelected)
    );
    button->setTag(index);
    button->setPosition({width / 2.f, 12.5f});
    row->addChild(button);

    return row;
}

void DemonListPopup::onDemonSelected(CCObject* sender) {
    auto node = static_cast<CCNode*>(sender);
    auto index = node->getTag();

    if (index < 0 || index >= static_cast<int>(m_demons.size())) {
        return;
    }

    auto demon = m_demons[index];
    auto globalRank = getDemonGlobalRank(*demon);
    auto categoryRank = index + 1;

    if (auto popup = DemonDetailPopup::create(demon, globalRank, categoryRank)) {
        popup->show();
    }
}

DemonListPopup* DemonListPopup::create(DemonCategory category) {
    auto ret = new DemonListPopup();
    if (ret->init(category)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
