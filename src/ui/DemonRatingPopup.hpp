#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../data/DemonData.hpp"

using namespace geode::prelude;

class DemonRatingPopup : public Popup, public FLAlertLayerProtocol {
protected:
    PendingDemonCompletion m_completion;
    std::array<int, 6> m_answers {5, 5, 5, 5, 5, 5};
    int m_page = -1;

    CCLabelBMFont* m_headingLabel = nullptr;
    CCLabelBMFont* m_questionLabel = nullptr;
    CCLabelBMFont* m_valueLabel = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;

    CCLayerColor* m_questionPanel = nullptr;
    CCLayerColor* m_valuePanel = nullptr;
    CCLayerColor* m_progressTrack = nullptr;
    CCLayerColor* m_progressFill = nullptr;

    CCMenuItemSpriteExtra* m_continueButton = nullptr;
    CCMenuItemSpriteExtra* m_prevButton = nullptr;
    CCMenuItemSpriteExtra* m_nextButton = nullptr;
    CCMenuItemSpriteExtra* m_finishButton = nullptr;
    CCMenuItemSpriteExtra* m_minusButton = nullptr;
    CCMenuItemSpriteExtra* m_plusButton = nullptr;

    bool init(PendingDemonCompletion const& completion);
    CCMenuItemSpriteExtra* makeButton(
        char const* text,
        SEL_MenuHandler callback,
        CCPoint const& offset,
        float scale = .52f
    );
    void refreshPage();

    void onContinue(CCObject*);
    void onPrevious(CCObject*);
    void onNext(CCObject*);
    void onMinus(CCObject*);
    void onPlus(CCObject*);
    void onFinish(CCObject*);

    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override;

public:
    static DemonRatingPopup* create(PendingDemonCompletion const& completion);
};
