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
}

bool DemonRatingPopup::init(PendingDemonCompletion const& completion) {
    if (!Popup::init(420.f, 250.f)) {
        return false;
    }

    m_completion = completion;
    this->setTitle(completion.name.c_str());

    m_headingLabel = CCLabelBMFont::create("Clasificacion permanente", "bigFont.fnt");
    m_headingLabel->setScale(.52f);
    m_mainLayer->addChildAtPosition(m_headingLabel, Anchor::Center, ccp(0.f, 57.f));

    m_questionLabel = CCLabelBMFont::create(
        "Tus respuestas fijaran la posicion del demon y no podran modificarse.",
        "goldFont.fnt"
    );
    m_questionLabel->setScale(.36f);
    m_questionLabel->limitLabelWidth(355.f, .36f, .22f);
    m_mainLayer->addChildAtPosition(m_questionLabel, Anchor::Center, ccp(0.f, 27.f));

    m_valueLabel = CCLabelBMFont::create("5 / 10", "bigFont.fnt");
    m_valueLabel->setScale(.72f);
    m_mainLayer->addChildAtPosition(m_valueLabel, Anchor::Center, ccp(0.f, -3.f));

    m_pageLabel = CCLabelBMFont::create("Pregunta 1 / 6", "goldFont.fnt");
    m_pageLabel->setScale(.34f);
    m_pageLabel->setOpacity(190);
    m_mainLayer->addChildAtPosition(m_pageLabel, Anchor::Center, ccp(0.f, -31.f));

    m_continueButton = this->makeButton(
        "CONTINUAR",
        menu_selector(DemonRatingPopup::onContinue),
        ccp(0.f, -62.f),
        .58f
    );
    m_prevButton = this->makeButton(
        "ATRAS",
        menu_selector(DemonRatingPopup::onPrevious),
        ccp(-92.f, -69.f)
    );
    m_nextButton = this->makeButton(
        "SIGUIENTE",
        menu_selector(DemonRatingPopup::onNext),
        ccp(92.f, -69.f)
    );
    m_finishButton = this->makeButton(
        "CLASIFICAR",
        menu_selector(DemonRatingPopup::onFinish),
        ccp(92.f, -69.f),
        .55f
    );
    m_minusButton = this->makeButton(
        "-",
        menu_selector(DemonRatingPopup::onMinus),
        ccp(-62.f, -3.f),
        .48f
    );
    m_plusButton = this->makeButton(
        "+",
        menu_selector(DemonRatingPopup::onPlus),
        ccp(62.f, -3.f),
        .48f
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
    auto sprite = ButtonSprite::create(text, "bigFont.fnt", "GJ_button_01.png", .8f);
    sprite->setScale(scale);

    auto button = CCMenuItemSpriteExtra::create(sprite, this, callback);
    m_buttonMenu->addChildAtPosition(button, Anchor::Center, offset);
    return button;
}

void DemonRatingPopup::refreshPage() {
    auto intro = m_page < 0;

    m_continueButton->setVisible(intro);
    m_valueLabel->setVisible(!intro);
    m_pageLabel->setVisible(!intro);
    m_minusButton->setVisible(!intro);
    m_plusButton->setVisible(!intro);
    m_prevButton->setVisible(!intro && m_page > 0);
    m_nextButton->setVisible(!intro && m_page < 5);
    m_finishButton->setVisible(!intro && m_page == 5);

    if (intro) {
        m_headingLabel->setString("Clasificacion permanente");
        m_questionLabel->setString(
            "Tus respuestas fijaran la posicion del demon. Una vez confirmado, no podra modificarse."
        );
        m_questionLabel->limitLabelWidth(355.f, .36f, .22f);
        return;
    }

    m_headingLabel->setString(kHeadings[m_page]);
    m_questionLabel->setString(kQuestions[m_page]);
    m_questionLabel->limitLabelWidth(355.f, .36f, .22f);

    auto valueText = fmt::format("{} / 10", m_answers[m_page]);
    m_valueLabel->setString(valueText.c_str());

    auto pageText = fmt::format("Pregunta {} / 6", m_page + 1);
    m_pageLabel->setString(pageText.c_str());
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
