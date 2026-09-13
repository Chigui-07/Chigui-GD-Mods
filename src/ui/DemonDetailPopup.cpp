#include "DemonDetailPopup.hpp"

bool DemonDetailPopup::init(
    DemonEntry const* demon,
    int globalRank,
    int categoryRank
) {
    if (!Popup::init(410.f, 240.f)) {
        return false;
    }

    m_demon = demon;
    m_globalRank = globalRank;
    m_categoryRank = categoryRank;

    this->setTitle(demon->name.c_str());

    auto globalText = fmt::format("Ranking general: #{}", globalRank);
    auto globalLabel = CCLabelBMFont::create(globalText.c_str(), "bigFont.fnt");
    globalLabel->setScale(.46f);
    m_mainLayer->addChildAtPosition(globalLabel, Anchor::Center, ccp(0.f, 58.f));

    auto categoryText = fmt::format(
        "{}: #{}",
        getDemonCategoryTitle(demon->category),
        categoryRank
    );
    auto categoryLabel = CCLabelBMFont::create(categoryText.c_str(), "goldFont.fnt");
    categoryLabel->setScale(.40f);
    m_mainLayer->addChildAtPosition(categoryLabel, Anchor::Center, ccp(0.f, 34.f));

    char const* sourceText = demon->officialLevelID > 0
        ? "Nivel oficial de Geometry Dash"
        : "Nivel online - se abrira la busqueda por nombre";

    auto sourceLabel = CCLabelBMFont::create(sourceText, "goldFont.fnt");
    sourceLabel->setScale(.34f);
    sourceLabel->setOpacity(190);
    m_mainLayer->addChildAtPosition(sourceLabel, Anchor::Center, ccp(0.f, 4.f));

    if (demon->officialLevelID > 0) {
        auto viewSprite = ButtonSprite::create(
            "VER NIVEL",
            "bigFont.fnt",
            "GJ_button_01.png",
            .8f
        );
        viewSprite->setScale(.58f);

        auto viewButton = CCMenuItemSpriteExtra::create(
            viewSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(viewButton, Anchor::Center, ccp(-72.f, -48.f));

        auto playSprite = ButtonSprite::create(
            "JUGAR",
            "bigFont.fnt",
            "GJ_button_01.png",
            .8f
        );
        playSprite->setScale(.58f);

        auto playButton = CCMenuItemSpriteExtra::create(
            playSprite,
            this,
            menu_selector(DemonDetailPopup::onPlayOfficial)
        );
        m_buttonMenu->addChildAtPosition(playButton, Anchor::Center, ccp(72.f, -48.f));
    }
    else {
        auto openSprite = ButtonSprite::create(
            "BUSCAR / JUGAR",
            "bigFont.fnt",
            "GJ_button_01.png",
            .8f
        );
        openSprite->setScale(.58f);

        auto openButton = CCMenuItemSpriteExtra::create(
            openSprite,
            this,
            menu_selector(DemonDetailPopup::onOpenLevel)
        );
        m_buttonMenu->addChildAtPosition(openButton, Anchor::Center, ccp(0.f, -48.f));
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

    auto search = GJSearchObject::create(
        SearchType::Search,
        m_demon->name.c_str()
    );
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
