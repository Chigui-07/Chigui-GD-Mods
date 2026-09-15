#include "DemonDetailPopup.hpp"
#include "DemonUI.hpp"

bool DemonDetailPopup::init(
    DemonEntry const* demon,
    int globalRank,
    int categoryRank
) {
    if (!Popup::init(430.f, 285.f)) {
        return false;
    }

    m_demon = demon;
    m_globalRank = globalRank;
    m_categoryRank = categoryRank;

    this->setTitle(demon->name.c_str());

    if (auto icon = demonui::difficultyIcon(demon->category, .46f)) {
        m_mainLayer->addChildAtPosition(icon, Anchor::Center, ccp(-150.f, 82.f));
    }

    auto categoryLabel = CCLabelBMFont::create(demonui::categoryName(demon->category), "goldFont.fnt");
    categoryLabel->setScale(.33f);
    categoryLabel->setAnchorPoint({0.f, .5f});
    m_mainLayer->addChildAtPosition(categoryLabel, Anchor::Center, ccp(-118.f, 84.f));

    auto card = demonui::panel({350.f, 135.f}, ccc3(91, 48, 31), 235);
    m_mainLayer->addChildAtPosition(card, Anchor::Center, ccp(0.f, 14.f));

    auto globalTitle = CCLabelBMFont::create("RANKING GLOBAL", "goldFont.fnt");
    globalTitle->setScale(.23f);
    globalTitle->setOpacity(185);
    m_mainLayer->addChildAtPosition(globalTitle, Anchor::Center, ccp(-88.f, 52.f));

    auto globalText = fmt::format("#{}", globalRank);
    auto globalLabel = CCLabelBMFont::create(globalText.c_str(), "bigFont.fnt");
    globalLabel->setScale(.64f);
    m_mainLayer->addChildAtPosition(globalLabel, Anchor::Center, ccp(-88.f, 29.f));

    auto categoryTitleText = fmt::format("{} RANK", demonui::categoryName(demon->category));
    auto categoryTitle = CCLabelBMFont::create(categoryTitleText.c_str(), "goldFont.fnt");
    categoryTitle->setScale(.20f);
    categoryTitle->setOpacity(185);
    categoryTitle->limitLabelWidth(135.f, .20f, .14f);
    m_mainLayer->addChildAtPosition(categoryTitle, Anchor::Center, ccp(88.f, 52.f));

    auto categoryRankText = fmt::format("#{}", categoryRank);
    auto categoryRankLabel = CCLabelBMFont::create(categoryRankText.c_str(), "bigFont.fnt");
    categoryRankLabel->setScale(.64f);
    m_mainLayer->addChildAtPosition(categoryRankLabel, Anchor::Center, ccp(88.f, 29.f));

    auto divider = CCLayerColor::create(ccc4(255, 255, 255, 45), 1.f, 43.f);
    divider->ignoreAnchorPointForPosition(false);
    divider->setAnchorPoint({.5f, .5f});
    m_mainLayer->addChildAtPosition(divider, Anchor::Center, ccp(0.f, 37.f));

    auto creatorText = demon->creator.empty()
        ? std::string("Creador: ---")
        : fmt::format("Creador: {}", demon->creator);
    auto creatorLabel = CCLabelBMFont::create(creatorText.c_str(), "goldFont.fnt");
    creatorLabel->setScale(.28f);
    creatorLabel->limitLabelWidth(300.f, .28f, .20f);
    creatorLabel->setOpacity(215);
    m_mainLayer->addChildAtPosition(creatorLabel, Anchor::Center, ccp(0.f, -6.f));

    auto idText = demon->officialLevelID > 0
        ? fmt::format("Nivel oficial #{}", demon->officialLevelID)
        : demon->levelID > 0
            ? fmt::format("Level ID: {}", demon->levelID)
            : std::string("Level ID: no disponible");
    auto idLabel = CCLabelBMFont::create(idText.c_str(), "goldFont.fnt");
    idLabel->setScale(.24f);
    idLabel->setOpacity(170);
    m_mainLayer->addChildAtPosition(idLabel, Anchor::Center, ccp(0.f, -23.f));

    auto statsText = fmt::format("Intentos: {}   Rebeats: {}", demon->attempts, demon->rebeats);
    auto statsLabel = CCLabelBMFont::create(statsText.c_str(), "goldFont.fnt");
    statsLabel->setScale(.22f);
    statsLabel->setOpacity(155);
    m_mainLayer->addChildAtPosition(statsLabel, Anchor::Center, ccp(0.f, -39.f));

    auto scoreText = demon->personalScore >= 0.0
        ? fmt::format("Difficulty Score: {:.1f}", demon->personalScore)
        : std::string("Difficulty Score: importado");
    auto scoreLabel = CCLabelBMFont::create(scoreText.c_str(), "goldFont.fnt");
    scoreLabel->setScale(.29f);
    m_mainLayer->addChildAtPosition(scoreLabel, Anchor::Center, ccp(0.f, -62.f));

    if (demon->officialLevelID > 0) {
        auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSprite->setScale(.72f);
        auto viewButton = CCMenuItemSpriteExtra::create(
            infoSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(viewButton, Anchor::Center, ccp(-58.f, -93.f));

        auto playSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        playSprite->setScale(.64f);
        auto playButton = CCMenuItemSpriteExtra::create(
            playSprite,
            this,
            menu_selector(DemonDetailPopup::onPlayOfficial)
        );
        m_buttonMenu->addChildAtPosition(playButton, Anchor::Center, ccp(58.f, -93.f));

        auto infoLabel = CCLabelBMFont::create("INFO", "goldFont.fnt");
        infoLabel->setScale(.20f);
        m_mainLayer->addChildAtPosition(infoLabel, Anchor::Center, ccp(-58.f, -118.f));

        auto playLabel = CCLabelBMFont::create("JUGAR", "goldFont.fnt");
        playLabel->setScale(.20f);
        m_mainLayer->addChildAtPosition(playLabel, Anchor::Center, ccp(58.f, -118.f));
    }
    else {
        auto playSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        playSprite->setScale(.66f);
        auto openButton = CCMenuItemSpriteExtra::create(
            playSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(openButton, Anchor::Center, ccp(0.f, -93.f));

        auto caption = demon->levelID > 0 ? "ABRIR NIVEL" : "BUSCAR / JUGAR";
        auto label = CCLabelBMFont::create(caption, "goldFont.fnt");
        label->setScale(.20f);
        m_mainLayer->addChildAtPosition(label, Anchor::Center, ccp(0.f, -118.f));
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
