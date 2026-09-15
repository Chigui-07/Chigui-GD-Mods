#include "DemonDetailPopup.hpp"
#include "DemonUI.hpp"

bool DemonDetailPopup::init(
    DemonEntry const* demon,
    int globalRank,
    int categoryRank
) {
    if (!Popup::init(438.f, 300.f)) {
        return false;
    }

    m_demon = demon;
    m_globalRank = globalRank;
    m_categoryRank = categoryRank;

    this->setTitle(demon->name.c_str());

    if (auto icon = demonui::difficultyIcon(demon->category, .45f)) {
        m_mainLayer->addChildAtPosition(icon, Anchor::Center, ccp(-153.f, 96.f));
    }

    auto categoryLabel = CCLabelBMFont::create(demonui::categoryName(demon->category), "goldFont.fnt");
    categoryLabel->setScale(.31f);
    categoryLabel->setAnchorPoint({0.f, .5f});
    m_mainLayer->addChildAtPosition(categoryLabel, Anchor::Center, ccp(-120.f, 97.f));

    auto scoreCard = demonui::panel({100.f, 38.f}, ccc3(112, 75, 25), 240);
    m_mainLayer->addChildAtPosition(scoreCard, Anchor::Center, ccp(127.f, 96.f));

    auto scoreTitle = CCLabelBMFont::create("SCORE", "goldFont.fnt");
    scoreTitle->setScale(.18f);
    scoreTitle->setOpacity(170);
    m_mainLayer->addChildAtPosition(scoreTitle, Anchor::Center, ccp(127.f, 105.f));

    auto scoreText = demon->personalScore >= 0.0
        ? fmt::format("{:.1f}", demon->personalScore)
        : std::string("BASE");
    auto scoreLabel = CCLabelBMFont::create(scoreText.c_str(), "bigFont.fnt");
    scoreLabel->setScale(.37f);
    m_mainLayer->addChildAtPosition(scoreLabel, Anchor::Center, ccp(127.f, 89.f));

    auto card = demonui::panel({358.f, 142.f}, ccc3(91, 48, 31), 240);
    m_mainLayer->addChildAtPosition(card, Anchor::Center, ccp(0.f, 13.f));

    auto globalTitle = CCLabelBMFont::create("RANKING GLOBAL", "goldFont.fnt");
    globalTitle->setScale(.22f);
    globalTitle->setOpacity(180);
    m_mainLayer->addChildAtPosition(globalTitle, Anchor::Center, ccp(-90.f, 55.f));

    auto globalText = fmt::format("#{}", globalRank);
    auto globalLabel = CCLabelBMFont::create(globalText.c_str(), "bigFont.fnt");
    globalLabel->setScale(.62f);
    m_mainLayer->addChildAtPosition(globalLabel, Anchor::Center, ccp(-90.f, 31.f));

    auto categoryTitleText = fmt::format("{} RANK", demonui::categoryName(demon->category));
    auto categoryTitle = CCLabelBMFont::create(categoryTitleText.c_str(), "goldFont.fnt");
    categoryTitle->setScale(.19f);
    categoryTitle->setOpacity(180);
    categoryTitle->limitLabelWidth(135.f, .19f, .13f);
    m_mainLayer->addChildAtPosition(categoryTitle, Anchor::Center, ccp(90.f, 55.f));

    auto categoryRankText = fmt::format("#{}", categoryRank);
    auto categoryRankLabel = CCLabelBMFont::create(categoryRankText.c_str(), "bigFont.fnt");
    categoryRankLabel->setScale(.62f);
    m_mainLayer->addChildAtPosition(categoryRankLabel, Anchor::Center, ccp(90.f, 31.f));

    auto divider = CCLayerColor::create(ccc4(255, 255, 255, 42), 1.f, 48.f);
    divider->ignoreAnchorPointForPosition(false);
    divider->setAnchorPoint({.5f, .5f});
    m_mainLayer->addChildAtPosition(divider, Anchor::Center, ccp(0.f, 42.f));

    auto horizontal = CCLayerColor::create(ccc4(255, 255, 255, 34), 310.f, 1.f);
    horizontal->ignoreAnchorPointForPosition(false);
    horizontal->setAnchorPoint({.5f, .5f});
    m_mainLayer->addChildAtPosition(horizontal, Anchor::Center, ccp(0.f, 2.f));

    auto creatorText = demon->creator.empty()
        ? std::string("Creador: ---")
        : fmt::format("Creador: {}", demon->creator);
    auto creatorLabel = CCLabelBMFont::create(creatorText.c_str(), "goldFont.fnt");
    creatorLabel->setScale(.27f);
    creatorLabel->limitLabelWidth(300.f, .27f, .19f);
    creatorLabel->setOpacity(215);
    m_mainLayer->addChildAtPosition(creatorLabel, Anchor::Center, ccp(0.f, -17.f));

    auto idText = demon->officialLevelID > 0
        ? fmt::format("Nivel oficial #{}", demon->officialLevelID)
        : demon->levelID > 0
            ? fmt::format("Level ID: {}", demon->levelID)
            : std::string("Level ID: no disponible");
    auto idLabel = CCLabelBMFont::create(idText.c_str(), "goldFont.fnt");
    idLabel->setScale(.23f);
    idLabel->setOpacity(165);
    m_mainLayer->addChildAtPosition(idLabel, Anchor::Center, ccp(0.f, -35.f));

    auto attemptsText = fmt::format("Intentos: {}", demon->attempts);
    auto attemptsLabel = CCLabelBMFont::create(attemptsText.c_str(), "goldFont.fnt");
    attemptsLabel->setScale(.22f);
    attemptsLabel->setOpacity(155);
    m_mainLayer->addChildAtPosition(attemptsLabel, Anchor::Center, ccp(-70.f, -52.f));

    auto rebeatsText = fmt::format("Rebeats: {}", demon->rebeats);
    auto rebeatsLabel = CCLabelBMFont::create(rebeatsText.c_str(), "goldFont.fnt");
    rebeatsLabel->setScale(.22f);
    rebeatsLabel->setOpacity(155);
    m_mainLayer->addChildAtPosition(rebeatsLabel, Anchor::Center, ccp(70.f, -52.f));

    if (demon->officialLevelID > 0) {
        auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSprite->setScale(.70f);
        auto viewButton = CCMenuItemSpriteExtra::create(
            infoSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(viewButton, Anchor::Center, ccp(-58.f, -96.f));

        auto playSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        playSprite->setScale(.62f);
        auto playButton = CCMenuItemSpriteExtra::create(
            playSprite,
            this,
            menu_selector(DemonDetailPopup::onPlayOfficial)
        );
        m_buttonMenu->addChildAtPosition(playButton, Anchor::Center, ccp(58.f, -96.f));

        auto infoLabel = CCLabelBMFont::create("INFO", "goldFont.fnt");
        infoLabel->setScale(.19f);
        m_mainLayer->addChildAtPosition(infoLabel, Anchor::Center, ccp(-58.f, -121.f));

        auto playLabel = CCLabelBMFont::create("JUGAR", "goldFont.fnt");
        playLabel->setScale(.19f);
        m_mainLayer->addChildAtPosition(playLabel, Anchor::Center, ccp(58.f, -121.f));
    }
    else {
        auto playSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        playSprite->setScale(.64f);
        auto openButton = CCMenuItemSpriteExtra::create(
            playSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(openButton, Anchor::Center, ccp(0.f, -96.f));

        auto caption = demon->levelID > 0 ? "ABRIR NIVEL" : "BUSCAR / JUGAR";
        auto label = CCLabelBMFont::create(caption, "goldFont.fnt");
        label->setScale(.19f);
        m_mainLayer->addChildAtPosition(label, Anchor::Center, ccp(0.f, -121.f));
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
