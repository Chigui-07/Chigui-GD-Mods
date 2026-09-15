#include "DemonListPopup.hpp"
#include "DemonDetailPopup.hpp"
#include "DemonUI.hpp"
#include <Geode/ui/ScrollLayer.hpp>

bool DemonListPopup::init(DemonCategory category) {
    if (!Popup::init(440.f, 292.f)) {
        return false;
    }

    m_category = category;
    m_demons = getDemonEntries(category);
    this->setTitle(getDemonCategoryTitle(category));

    auto countText = fmt::format("{} registrados", m_demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.35f);
    countLabel->setOpacity(210);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 101.f));

    auto hintLabel = CCLabelBMFont::create("Toca cualquier nivel para abrir su ficha", "goldFont.fnt");
    hintLabel->setScale(.24f);
    hintLabel->setOpacity(140);
    m_mainLayer->addChildAtPosition(hintLabel, Anchor::Center, ccp(0.f, 84.f));

    auto list = ScrollLayer::create({378.f, 201.f});
    list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(5.f));
    list->setTouchEnabled(true);
    list->setStealingTouches(true);

    if (m_demons.empty()) {
        auto emptyRow = CCNode::create();
        emptyRow->setContentSize({364.f, 74.f});

        auto bg = demonui::panel({352.f, 62.f}, ccc3(91, 48, 31), 225);
        bg->setPosition({182.f, 37.f});
        emptyRow->addChild(bg);

        auto emptyLabel = CCLabelBMFont::create(
            "Todavia no hay demons en esta categoria.",
            "goldFont.fnt"
        );
        emptyLabel->setScale(.33f);
        emptyLabel->setOpacity(205);
        emptyLabel->limitLabelWidth(290.f, .33f, .23f);
        emptyLabel->setPosition({182.f, 37.f});
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
                    364.f
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
        ccp(-listSize.width / 2.f, -listSize.height / 2.f - 18.f)
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
    row->setContentSize({width, 52.f});
    row->setAnchorPoint({0.f, 0.f});
    row->setPosition({0.f, 0.f});

    auto content = CCNode::create();
    content->setContentSize({width, 50.f});
    content->setAnchorPoint({.5f, .5f});
    content->ignoreAnchorPointForPosition(false);

    auto bg = demonui::panel({width - 6.f, 47.f}, ccc3(91, 48, 31), 235);
    bg->setPosition({width / 2.f, 25.f});
    content->addChild(bg);

    if (auto icon = demonui::difficultyIcon(demon.category, .29f)) {
        icon->setPosition({30.f, 25.f});
        content->addChild(icon);
    }

    auto rankText = fmt::format("#{}", rank);
    auto rankLabel = CCLabelBMFont::create(rankText.c_str(), "bigFont.fnt");
    rankLabel->setScale(.34f);
    rankLabel->setPosition({67.f, 25.f});
    content->addChild(rankLabel);

    auto nameLabel = CCLabelBMFont::create(demon.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(.36f);
    nameLabel->setAnchorPoint({0.f, .5f});
    nameLabel->setPosition({94.f, 31.f});
    nameLabel->limitLabelWidth(175.f, .36f, .23f);
    content->addChild(nameLabel);

    auto categoryLabel = CCLabelBMFont::create(demonui::categoryName(demon.category), "goldFont.fnt");
    categoryLabel->setScale(.20f);
    categoryLabel->setOpacity(175);
    categoryLabel->setAnchorPoint({0.f, .5f});
    categoryLabel->setPosition({94.f, 14.f});
    content->addChild(categoryLabel);

    auto scoreText = demon.personalScore >= 0.0
        ? fmt::format("Score {:.1f}", demon.personalScore)
        : std::string("Importado");
    auto scoreLabel = CCLabelBMFont::create(scoreText.c_str(), "goldFont.fnt");
    scoreLabel->setScale(.22f);
    scoreLabel->setOpacity(200);
    scoreLabel->setPosition({287.f, 25.f});
    content->addChild(scoreLabel);

    auto arrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    if (arrow) {
        arrow->setFlipX(true);
        arrow->setScale(.34f);
        arrow->setPosition({339.f, 25.f});
        content->addChild(arrow);
    }

    auto button = CCMenuItemSpriteExtra::create(
        content,
        this,
        menu_selector(DemonListPopup::onDemonSelected)
    );
    button->setTag(index);
    button->setPosition({width / 2.f, 26.f});
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
    auto categoryRank = getDemonCategoryRank(*demon);

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
