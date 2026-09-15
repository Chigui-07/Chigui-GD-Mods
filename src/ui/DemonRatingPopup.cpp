#include "DemonRatingPopup.hpp"

namespace {
    constexpr char const* kHeadings[] = {
        "Dificultad general",
        "Consistencia",
        "Precision",
        "Control",
        "Aprendizaje",
        "Presion"
    };

    constexpr char const* kQuestions[] = {
        "Que tan dificil fue completar el nivel?",
        "Que tan dificil fue unir todas las partes en una partida?",
        "Cuanta precision exigieron los timings y movimientos?",
        "Que tan exigente fue controlar los modos de juego?",
        "Que tan dificil fue aprender y recordar el nivel?",
        "Cuanto afectaron nervios, longitud o partes finales?"
    };

    CCLayerColor* makePanel(CCSize const& size, ccColor4B const& color) {
        auto panel = CCLayerColor::create(color, size.width, size.height);
        panel->ignoreAnchorPointForPosition(false);
        panel->setAnchorPoint({.5f, .5f});
        return panel;
    }
}

bool DemonRatingPopup::init(PendingDemonCompletion const& completion) {
    if (!Popup::init(440.f, 285.f)) {
        return false;
    }

    m_completion = completion;
    this->setTitle(completion.name.c_str());

    m_headingLabel = CCLabelBMFont::create("Clasificacion permanente", "bigFont.fnt");
    m_headingLabel->setScale(.50f);
    m_mainLayer->addChildAtPosition(m_headingLabel, Anchor::Center, ccp(0.f, 78.f));

    m_questionPanel = makePanel({370.f, 56.f}, ccc4(29, 47, 72, 112));
    m_mainLayer->addChildAtPosition(m_questionPanel, Anchor::Center, ccp(0.f, 40.f));

    m_questionLabel = CCLabelBMFont::create(
        "Tus respuestas fijaran la posicion del demon y no podran modificarse.",
        "goldFont.fnt"
    );
    m_questionLabel->setScale(.34f);
    m_questionLabel->limitLabelWidth(335.f, .34f, .22f);
    m_mainLayer->addChildAtPosition(m_questionLabel, Anchor::Center, ccp(0.f, 40.f));

    m_valuePanel = makePanel({142.f, 45.f}, ccc4(105, 77, 28, 112));
    m_mainLayer->addChildAtPosition(m_valuePanel, Anchor::Center, ccp(0.f, -3.f));

    m_valueLabel = CCLabelBMFont::create("5 / 10", "bigFont.fnt");
    m_valueLabel->setScale(.72f);
    m_mainLayer->addChildAtPosition(m_valueLabel, Anchor::Center, ccp(0.f, -3.f));

    m_pageLabel = CCLabelBMFont::create("Pregunta 1 / 6", "goldFont.fnt");
    m_pageLabel->setScale(.32f);
    m_pageLabel->setOpacity(190);
    m_mainLayer->addChildAtPosition(m_pageLabel, Anchor::Center, ccp(0.f, -35.f));

    m_progressTrack = makePanel({250.f, 5.f}, ccc4(20, 27, 39, 150));
    m_mainLayer->addChildAtPosition(m_progressTrack, Anchor::Center, ccp(0.f, -49.f));

    m_progressFill = CCLayerColor::create(ccc4(75, 184, 95, 220), 42.f, 5.f);
    m_progressFill->setPosition({0.f, 0.f});
    m_progressTrack->addChild(m_progressFill);

    m_continueButton = this->makeButton(
        "CONTINUAR",
        menu_selector(DemonRatingPopup::onContinue),
        ccp(0.f, -76.f),
        .56f
    );
    m_prevButton = this->makeButton(
        "ATRAS",
        menu_selector(DemonRatingPopup::onPrevious),
        ccp(-95.f, -78.f)
    );
    m_nextButton = this->makeButton(
        "SIGUIENTE",
        menu_selector(DemonRatingPopup::onNext),
        ccp(95.f, -78.f)
    );
    m_finishButton = this->makeButton(
        "CLASIFICAR",
        menu_selector(DemonRatingPopup::onFinish),
        ccp(95.f, -78.f),
        .52f
    );
    m_minusButton = this->makeButton(
        "-",
        menu_selector(DemonRatingPopup::onMinus),
        ccp(-78.f, -3.f),
        .46f
    );
    m_plusButton = this->makeButton(
        "+",
        menu_selector(DemonRatingPopup::onPlus),
        ccp(78.f, -3.f),
        .46f
    );

    this->refreshPage();
    return true;
}

CCMenuItemSpriteExtra* DemonRatingPopup::makeButton(
    char const* text,
    SEL_MenuHandler callback,
    CCPoint const& offset,
    float scale
) {
    auto texture = "GJ_button_04.png";
    auto caption = std::string(text);

    if (caption == "+" || caption == "CLASIFICAR") {
        texture = "GJ_button_01.png";
    }
    else if (caption == "-") {
        texture = "GJ_button_05.png";
    }
    else if (caption == "ATRAS") {
        texture = "GJ_button_03.png";
    }

    auto sprite = ButtonSprite::create(text, "bigFont.fnt", texture, .8f);
    sprite->setScale(scale);

    auto button = CCMenuItemSpriteExtra::create(sprite, this, callback);
    m_buttonMenu->addChildAtPosition(button, Anchor::Center, offset);
    return button;
}

void DemonRatingPopup::refreshPage() {
    auto intro = m_page < 0;

    m_continueButton->setVisible(intro);
    m_valuePanel->setVisible(!intro);
    m_valueLabel->setVisible(!intro);
    m_pageLabel->setVisible(!intro);
    m_progressTrack->setVisible(!intro);
    m_minusButton->setVisible(!intro);
    m_plusButton->setVisible(!intro);
    m_prevButton->setVisible(!intro && m_page > 0);
    m_nextButton->setVisible(!intro && m_page < 5);
    m_finishButton->setVisible(!intro && m_page == 5);

    if (intro) {
        m_headingLabel->setString("Clasificacion permanente");
        m_questionLabel->setString(
            "Responde pensando en como se sintio para ti al completarlo. Una vez confirmado, el orden sera permanente."
        );
        m_questionLabel->limitLabelWidth(335.f, .34f, .22f);
        return;
    }

    m_headingLabel->setString(kHeadings[m_page]);
    m_questionLabel->setString(kQuestions[m_page]);
    m_questionLabel->limitLabelWidth(335.f, .34f, .22f);

    auto valueText = fmt::format("{} / 10", m_answers[m_page]);
    m_valueLabel->setString(valueText.c_str());

    auto pageText = fmt::format("Pregunta {} / 6", m_page + 1);
    m_pageLabel->setString(pageText.c_str());

    auto progressWidth = 250.f * static_cast<float>(m_page + 1) / 6.f;
    m_progressFill->setContentSize({progressWidth, 5.f});
}

void DemonRatingPopup::onContinue(CCObject*) {
    m_page = 0;
    this->refreshPage();
}

void DemonRatingPopup::onPrevious(CCObject*) {
    if (m_page > 0) {
        --m_page;
        this->refreshPage();
    }
}

void DemonRatingPopup::onNext(CCObject*) {
    if (m_page >= 0 && m_page < 5) {
        ++m_page;
        this->refreshPage();
    }
}

void DemonRatingPopup::onMinus(CCObject*) {
    if (m_page < 0) {
        return;
    }
    m_answers[m_page] = std::max(1, m_answers[m_page] - 1);
    this->refreshPage();
}

void DemonRatingPopup::onPlus(CCObject*) {
    if (m_page < 0) {
        return;
    }
    m_answers[m_page] = std::min(10, m_answers[m_page] + 1);
    this->refreshPage();
}

void DemonRatingPopup::onFinish(CCObject*) {
    FLAlertLayer::create(
        this,
        "Confirmar clasificacion",
        "Tus respuestas se usaran para clasificar este demon de forma <cy>permanente</c>. La posicion aun no se mostrara.",
        "Volver",
        "Clasificar",
        350.f
    )->show();
}

void DemonRatingPopup::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (!btn2) {
        return;
    }

    auto score = calculateDifficultyScore(m_answers);
    auto rank = registerRatedDemon(m_completion, m_answers);

    auto resultText = fmt::format(
        "<cg>{}</c> ha sido clasificado.\n\nDifficulty Score: <cy>{:.1f}</c>\nRanking general: <cy>#{}</c>",
        m_completion.name,
        score,
        rank
    );

    auto result = FLAlertLayer::create(
        "Demon clasificado",
        resultText.c_str(),
        "OK"
    );

    this->removeFromParentAndCleanup(true);
    result->show();
}

DemonRatingPopup* DemonRatingPopup::create(PendingDemonCompletion const& completion) {
    auto ret = new DemonRatingPopup();
    if (ret->init(completion)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
