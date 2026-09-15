#include "DemonDetailPopup.hpp"

namespace {
    CCLayerColor* makePanel(CCSize const& size, ccColor4B const& color) {
        auto panel = CCLayerColor::create(color, size.width, size.height);
        panel->ignoreAnchorPointForPosition(false);
        panel->setAnchorPoint({.5f, .5f});
        return panel;
    }

    ccColor4B categoryColor(DemonCategory category) {
        switch (category) {
            case DemonCategory::Easy: return ccc4(47, 132, 73, 120);
            case DemonCategory::Medium: return ccc4(171, 142, 46, 120);
            case DemonCategory::Hard: return ccc4(184, 94, 39, 120);
            case DemonCategory::Insane: return ccc4(166, 49, 53, 120);
            case DemonCategory::Extreme: return ccc4(112, 59, 151, 120);
            default: return ccc4(47, 76, 119, 120);
        }
    }

    char const* categoryShortName(DemonCategory category) {
        switch (category) {
            case DemonCategory::Easy: return "EASY";
            case DemonCategory::Medium: return "MEDIUM";
            case DemonCategory::Hard: return "HARD";
            case DemonCategory::Insane: return "INSANE";
            case DemonCategory::Extreme: return "EXTREME";
            default: return "ALL";
        }
    }
}

bool DemonDetailPopup::init(
    DemonEntry const* demon,
    int globalRank,
    int categoryRank
) {
    if (!Popup::init(430.f, 280.f)) {
        return false;
    }

    m_demon = demon;
    m_globalRank = globalRank;
    m_categoryRank = categoryRank;

    this->setTitle(demon->name.c_str());

    auto badge = makePanel({100.f, 22.f}, categoryColor(demon->category));
    m_mainLayer->addChildAtPosition(badge, Anchor::Center, ccp(0.f, 84.f));

    auto badgeLabel = CCLabelBMFont::create(categoryShortName(demon->category), "goldFont.fnt");
    badgeLabel->setScale(.28f);
    m_mainLayer->addChildAtPosition(badgeLabel, Anchor::Center, ccp(0.f, 84.f));

    auto globalPanel = makePanel({148.f, 55.f}, ccc4(29, 47, 71, 118));
    auto categoryPanel = makePanel({148.f, 55.f}, categoryColor(demon->category));
    m_mainLayer->addChildAtPosition(globalPanel, Anchor::Center, ccp(-82.f, 45.f));
    m_mainLayer->addChildAtPosition(categoryPanel, Anchor::Center, ccp(82.f, 45.f));

    auto globalTitle = CCLabelBMFont::create("RANKING GLOBAL", "goldFont.fnt");
    globalTitle->setScale(.24f);
    globalTitle->setOpacity(190);
    m_mainLayer->addChildAtPosition(globalTitle, Anchor::Center, ccp(-82.f, 59.f));

    auto globalText = fmt::format("#{}", globalRank);
    auto globalLabel = CCLabelBMFont::create(globalText.c_str(), "bigFont.fnt");
    globalLabel->setScale(.62f);
    m_mainLayer->addChildAtPosition(globalLabel, Anchor::Center, ccp(-82.f, 39.f));

    auto categoryTitle = fmt::format("{} RANK", categoryShortName(demon->category));
    auto categoryTitleLabel = CCLabelBMFont::create(categoryTitle.c_str(), "goldFont.fnt");
    categoryTitleLabel->setScale(.24f);
    categoryTitleLabel->setOpacity(210);
    m_mainLayer->addChildAtPosition(categoryTitleLabel, Anchor::Center, ccp(82.f, 59.f));

    auto categoryText = fmt::format("#{}", categoryRank);
    auto categoryRankLabel = CCLabelBMFont::create(categoryText.c_str(), "bigFont.fnt");
    categoryRankLabel->setScale(.62f);
    m_mainLayer->addChildAtPosition(categoryRankLabel, Anchor::Center, ccp(82.f, 39.f));

    auto infoPanel = makePanel({335.f, 58.f}, ccc4(25, 40, 61, 105));
    m_mainLayer->addChildAtPosition(infoPanel, Anchor::Center, ccp(0.f, -6.f));

    auto creatorText = demon->creator.empty()
        ? std::string("Creador: ---")
        : fmt::format("Creador: {}", demon->creator);
    auto creatorLabel = CCLabelBMFont::create(creatorText.c_str(), "goldFont.fnt");
    creatorLabel->setScale(.31f);
    creatorLabel->limitLabelWidth(300.f, .31f, .22f);
    creatorLabel->setOpacity(215);
    m_mainLayer->addChildAtPosition(creatorLabel, Anchor::Center, ccp(0.f, 8.f));

    auto idText = demon->officialLevelID > 0
        ? fmt::format("Nivel oficial #{}", demon->officialLevelID)
        : demon->levelID > 0
            ? fmt::format("Level ID: {}", demon->levelID)
            : std::string("Level ID: no disponible");
    auto idLabel = CCLabelBMFont::create(idText.c_str(), "goldFont.fnt");
    idLabel->setScale(.27f);
    idLabel->setOpacity(165);
    m_mainLayer->addChildAtPosition(idLabel, Anchor::Center, ccp(0.f, -7.f));

    auto statsText = fmt::format("Intentos: {}   Rebeats: {}", demon->attempts, demon->rebeats);
    auto statsLabel = CCLabelBMFont::create(statsText.c_str(), "goldFont.fnt");
    statsLabel->setScale(.25f);
    statsLabel->setOpacity(150);
    m_mainLayer->addChildAtPosition(statsLabel, Anchor::Center, ccp(0.f, -21.f));

    auto scorePanel = makePanel({165.f, 31.f}, ccc4(117, 91, 32, 105));
    m_mainLayer->addChildAtPosition(scorePanel, Anchor::Center, ccp(0.f, -49.f));

    auto scoreText = demon->personalScore >= 0.0
        ? fmt::format("Score personal: {:.1f}", demon->personalScore)
        : std::string("Score personal: BASE");
    auto scoreLabel = CCLabelBMFont::create(scoreText.c_str(), "goldFont.fnt");
    scoreLabel->setScale(.30f);
    m_mainLayer->addChildAtPosition(scoreLabel, Anchor::Center, ccp(0.f, -49.f));

    if (demon->officialLevelID > 0) {
        auto viewSprite = ButtonSprite::create(
            "VER NIVEL",
            "bigFont.fnt",
            "GJ_button_04.png",
            .8f
        );
        viewSprite->setScale(.52f);

        auto viewButton = CCMenuItemSpriteExtra::create(
            viewSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(viewButton, Anchor::Center, ccp(-72.f, -84.f));

        auto playSprite = ButtonSprite::create(
            "JUGAR",
            "bigFont.fnt",
            "GJ_button_01.png",
            .8f
        );
        playSprite->setScale(.52f);

        auto playButton = CCMenuItemSpriteExtra::create(
            playSprite,
            this,
            menu_selector(DemonDetailPopup::onPlayOfficial)
        );
        m_buttonMenu->addChildAtPosition(playButton, Anchor::Center, ccp(72.f, -84.f));
    }
    else {
        auto caption = demon->levelID > 0 ? "ABRIR NIVEL" : "BUSCAR / JUGAR";
        auto openSprite = ButtonSprite::create(
            caption,
            "bigFont.fnt",
            "GJ_button_01.png",
            .8f
        );
        openSprite->setScale(.54f);

        auto openButton = CCMenuItemSpriteExtra::create(
            openSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(openButton, Anchor::Center, ccp(0.f, -84.f));
    }

    return true;
}

void DemonDetailPopup::onOpenLevel(CCObject*) {
    if (!m_demon) {
        return;
    }

    if (m_demon->officialLevelID > 0) {
        auto manager = GameLevelManager::sharedState();
        auto level = manager->getMainLevel(m_demon->officialLevelID, false);

        if (!level) {
            FLAlertLayer::create(
                "Demon Tracker",
                "No se pudo cargar el nivel oficial.",
                "OK"
            )->show();
            return;
        }

        auto scene = LevelInfoLayer::scene(level, false);
        CCDirector::sharedDirector()->replaceScene(
            CCTransitionFade::create(.5f, scene)
        );
        return;
    }

    auto query = m_demon->levelID > 0
        ? fmt::format("{}", m_demon->levelID)
        : m_demon->name;

    auto search = GJSearchObject::create(SearchType::Search, query.c_str());
    auto scene = LevelBrowserLayer::scene(search);
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(.5f, scene)
    );
}

void DemonDetailPopup::onPlayOfficial(CCObject*) {
    if (!m_demon || m_demon->officialLevelID <= 0) {
        return;
    }

    auto manager = GameLevelManager::sharedState();
    auto level = manager->getMainLevel(m_demon->officialLevelID, false);

    if (!level) {
        FLAlertLayer::create(
            "Demon Tracker",
            "No se pudo cargar el nivel oficial.",
            "OK"
        )->show();
        return;
    }

    auto scene = PlayLayer::scene(level, false, false);
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(.5f, scene)
    );
}

DemonDetailPopup* DemonDetailPopup::create(
    DemonEntry const* demon,
    int globalRank,
    int categoryRank
) {
    auto ret = new DemonDetailPopup();
    if (ret->init(demon, globalRank, categoryRank)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
