#include "DemonListPopup.hpp"
#include <Geode/ui/ScrollLayer.hpp>

bool DemonListPopup::init(DemonCategory category) {
    if (!Popup::init(420.f, 260.f)) {
        return false;
    }

    m_category = category;
    this->setTitle(getDemonCategoryTitle(category));

    auto demons = getDemonEntries(category);
    auto countText = fmt::format("Demons registrados: {}", demons.size());
    auto countLabel = CCLabelBMFont::create(countText.c_str(), "goldFont.fnt");
    countLabel->setScale(.42f);
    m_mainLayer->addChildAtPosition(countLabel, Anchor::Center, ccp(0.f, 82.f));

    auto list = ScrollLayer::create({350.f, 175.f});
    list->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout(2.f));
    list->setTouchEnabled(true);

    if (demons.empty()) {
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
        for (size_t i = 0; i < demons.size(); ++i) {
            list->m_contentLayer->addChild(
                this->createDemonRow(static_cast<int>(i) + 1, *demons[i], 340.f)
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

CCNode* DemonListPopup::createDemonRow(int rank, DemonEntry const& demon, float width) {
    auto row = CCNode::create();
    row->setContentSize({width, 22.f});

    auto rankText = fmt::format("#{}", rank);
    auto rankLabel = CCLabelBMFont::create(rankText.c_str(), "goldFont.fnt");
    rankLabel->setScale(.42f);
    rankLabel->setAnchorPoint({0.f, .5f});
    rankLabel->setPosition({8.f, 11.f});
    row->addChild(rankLabel);

    auto nameLabel = CCLabelBMFont::create(demon.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(.40f);
    nameLabel->setAnchorPoint({0.f, .5f});
    nameLabel->setPosition({58.f, 11.f});
    row->addChild(nameLabel);

    return row;
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
