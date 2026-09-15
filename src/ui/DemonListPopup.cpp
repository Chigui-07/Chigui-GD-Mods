#include "DemonListPopup.hpp"
#include "DemonDetailPopup.hpp"
#include <Geode/ui/ScrollLayer.hpp>

namespace {
    ccColor4B categoryRowColor(DemonCategory category) {
        switch (category) {
            case DemonCategory::Easy: return ccc4(47, 132, 73, 92);
            case DemonCategory::Medium: return ccc4(171, 142, 46, 92);
            case DemonCategory::Hard: return ccc4(184, 94, 39, 92);
            case DemonCategory::Insane: return ccc4(166, 49, 53, 92);
            case DemonCategory::Extreme: return ccc4(112, 59, 151, 92);
            default: return ccc4(47, 76, 119, 92);
        }
    }

    char const* categoryShortName(DemonCategory category) {
        switch (category) {
            case DemonCategory::Easy: return "Easy Demon";
            case DemonCategory::Medium: return "Medium Demon";
            case DemonCategory::Hard: return "Hard Demon";
            case DemonCategory::Insane: return "Insane Demon";
            case DemonCategory::Extreme: return "Extreme Demon";
            default: return "Demon";
        }
    }
}

bool DemonListPopup::init(DemonCategory category) {
    if (!Popup::init(440.f, 285.f)) {
        return false;
    }

    m_category = category;
    m_demons = getDemonEntries(category);
    this->setTitle(getDemonCategoryTitle(category));

    auto countText = fmt::format("{} registrados", m_demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.38f);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 98.f));

    auto hintLabel = CCLabelBMFont::create("Toca VER para abrir la ficha del nivel", "goldFont.fnt");
    hintLabel->setScale(.25f);
    hintLabel->setOpacity(145);
    m_mainLayer->addChildAtPosition(hintLabel, Anchor::Center, ccp(0.f, 82.f));

    auto list = ScrollLayer::create({372.f, 194.f});
    list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(4.f));
    list->setTouchEnabled(true);
    list->setStealingTouches(true);

    if (m_demons.empty()) {
        auto emptyRow = CCNode::create();
        emptyRow->setContentSize({360.f, 70.f});

        auto emptyPanel = CCLayerColor::create(ccc4(33, 48, 70, 95), 356.f, 62.f);
        emptyPanel->setPosition({2.f, 4.f});
        emptyRow->addChild(emptyPanel);

        auto emptyLabel = CCLabelBMFont::create(
            "Todavia no hay demons en esta categoria.",
            "goldFont.fnt"
        );
        emptyLabel->setScale(.34f);
        emptyLabel->setOpacity(200);
        emptyLabel->limitLabelWidth(310.f, .34f, .24f);
        emptyLabel->setPosition({180.f, 35.f});
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
                    360.f
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
        ccp(-listSize.width / 2.f, -listSize.height / 2.f - 17.f)
    );

    return true;
}

CCNode* DemonListPopup::createDemonRow(
    int rank,
    int index,
    DemonEntry const& demon,
    float width
) {
    auto row = CCNode::create();
    row->setContentSize({width, 40.f});

    auto background = CCLayerColor::create(
        categoryRowColor(demon.category),
        width - 4.f,
        38.f
    );
    background->setPosition({2.f, 1.f});
    row->addChild(background);

    auto rankText = fmt::format("#{}", rank);
    auto rankLabel = CCLabelBMFont::create(rankText.c_str(), "bigFont.fnt");
    rankLabel->setScale(.46f);
    rankLabel->setAnchorPoint({0.f, .5f});
    rankLabel->setPosition({10.f, 20.f});
    row->addChild(rankLabel);

    auto nameLabel = CCLabelBMFont::create(demon.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(.38f);
    nameLabel->setAnchorPoint({0.f, .5f});
    nameLabel->setPosition({58.f, 25.f});
    nameLabel->limitLabelWidth(180.f, .38f, .24f);
    row->addChild(nameLabel);

    auto categoryLabel = CCLabelBMFont::create(categoryShortName(demon.category), "goldFont.fnt");
    categoryLabel->setScale(.23f);
    categoryLabel->setOpacity(180);
    categoryLabel->setAnchorPoint({0.f, .5f});
    categoryLabel->setPosition({58.f, 10.f});
    row->addChild(categoryLabel);

    auto scoreText = demon.personalScore >= 0.0
        ? fmt::format("{:.1f}", demon.personalScore)
        : std::string("BASE");
    auto scoreLabel = CCLabelBMFont::create(scoreText.c_str(), "goldFont.fnt");
    scoreLabel->setScale(.29f);
    scoreLabel->setOpacity(210);
    scoreLabel->setPosition({268.f, 20.f});
    row->addChild(scoreLabel);

    auto menu = CCMenu::create();
    menu->setContentSize({width, 40.f});
    menu->setPosition({0.f, 0.f});
    row->addChild(menu);

    auto sprite = ButtonSprite::create(
        "VER",
        "bigFont.fnt",
        "GJ_button_04.png",
        .8f
    );
    sprite->setScale(.34f);

    auto button = CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(DemonListPopup::onDemonSelected)
    );
    button->setTag(index);
    button->setPosition({326.f, 20.f});
    menu->addChild(button);

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
