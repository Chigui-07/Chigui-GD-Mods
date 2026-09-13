#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../data/DemonData.hpp"

using namespace geode::prelude;

class DemonListPopup : public Popup {
protected:
    DemonCategory m_category = DemonCategory::All;
    std::vector<DemonEntry const*> m_demons;

    bool init(DemonCategory category);
    CCNode* createDemonRow(
        int rank,
        int index,
        DemonEntry const& demon,
        float width
    );
    void onDemonSelected(CCObject* sender);

public:
    static DemonListPopup* create(DemonCategory category);
};
