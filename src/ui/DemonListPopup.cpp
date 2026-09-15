#include "DemonListPopup.hpp"
#include "DemonDetailPopup.hpp"
#include "DemonUI.hpp"
#include <Geode/ui/ScrollLayer.hpp>

bool DemonListPopup::init(DemonCategory category) {
    if (!Popup::init(448.f, 300.f)) {
        return false;
    }

    m_category = category;
    m_demons = getDemonEntries(category);
    this->setTitle(getDemonCategoryTitle(category));

    auto countText = fmt::format("{} registrados", m_demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.34f);
    countLabel->setOpacity(205);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 108.f));

    auto listBackdrop = demonui::panel({390.f, 214.f}, ccc3(70, 37, 27), 220);
    m_mainLayer->addChildAtPosition(listBackdrop, Anchor::Center, ccp(0.f, -18.f));

    auto list = ScrollLayer::create({382.f, 206.f});
    list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(3.f));
    list->setTouchEnabled(true);
    list->setStealingTouches(true);

    if (m_demons.empty()) {
        auto emptyRow = CCNode::create();
        emptyRow->setContentSize({370.f, 76.f});

        auto bg = demonui::panel({358.f, 64.f}, ccc3(102, 58, 37), 235);
        bg->setPosition({185.f, 38.f});
        emptyRow->addChild(bg);

        auto emptyLabel = CCLabelBMFont::create(
            "Todavia no hay demons en esta categoria.",
            "goldFont.fnt"
        );
        emptyLabel->setScale(.32f);
        emptyLabel->setOpacity(205);
        emptyLabel->limitLabelWidth(295.f, .32f, .22f);
        emptyLabel->setPosition({185.f, 38.f});
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
                    370.f
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
    row->setContentSize({width, 48.f});
    row->setAnchorPoint({0.f, 0.f});
    row->setPosition({0.f, 0.f});

    auto content = CCNode::create();
    content->setContentSize({width, 46.f});
    content->setAnchorPoint({.5f, .5f});
    content->ignoreAnchorPointForPosition(false);

    auto bg = demonui::panel({width - 6.f, 44.f}, ccc3(103, 58, 37), 242);
    bg->setPosition({width / 2.f, 23.f});
    content->addChild(bg);

    auto rankBg = demonui::panel({43.f, 34.f}, ccc3(66, 36, 27), 245);
    rankBg->setPosition({32.f, 23.f});
    content->addChild(rankBg);

    auto rankText = fmt::format("#{}", rank);
    auto rankLabel = CCLabelBMFont::create(rankText.c_str(), "bigFont.fnt");
    rankLabel->setScale(.31f);
    rankLabel->setPosition({32.f, 23.f});
    content->addChild(rankLabel);

    if (auto icon = demonui::difficultyIcon(demon.category, .25f)) {
        icon->setPosition({76.f, 23.f});
        content->addChild(icon);
    }

    auto nameLabel = CCLabelBMFont::create(demon.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(.34f);
    nameLabel->setAnchorPoint({0.f, .5f});
    nameLabel->setPosition({101.f, 29.f});
    nameLabel->limitLabelWidth(168.f, .34f, .22f);
    content->addChild(nameLabel);

    auto subText = demon.creator.empty()
        ? std::string(demonui::categoryName(demon.category))
        : fmt::format("{}  -  {}", demonui::categoryName(demon.category), demon.creator);
    auto subLabel = CCLabelBMFont::create(subText.c_str(), "goldFont.fnt");
    subLabel->setScale(.18f);
    subLabel->setOpacity(170);
    subLabel->setAnchorPoint({0.f, .5f});
    subLabel->setPosition({101.f, 12.f});
    subLabel->limitLabelWidth(190.f, .18f, .14f);
    content->addChild(subLabel);

    auto scoreTitle = CCLabelBMFont::create("SCORE", "goldFont.fnt");
    scoreTitle->setScale(.16f);
    scoreTitle->setOpacity(135);
    scoreTitle->setPosition({300.f, 31.f});
    content->addChild(scoreTitle);

    auto scoreText = demon.personalScore >= 0.0
        ? fmt::format("{:.1f}", demon.personalScore)
        : std::string("BASE");
    auto scoreLabel = CCLabelBMFont::create(scoreText.c_str(), "goldFont.fnt");
    scoreLabel->setScale(.24f);
    scoreLabel->setOpacity(215);
    scoreLabel->setPosition({300.f, 16.f});
    content->addChild(scoreLabel);

    auto arrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    if (arrow) {
        arrow->setFlipX(true);
        arrow->setScale(.32f);
        arrow->setPosition({346.f, 23.f});
        content->addChild(arrow);
    }

    auto button = CCMenuItemSpriteExtra::create(
        content,
        this,
        menu_selector(DemonListPopup::onDemonSelected)
    );
    button->setTag(index);
    button->setPosition({width / 2.f, 24.f});
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
