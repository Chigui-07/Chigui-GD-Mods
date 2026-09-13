#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../data/DemonData.hpp"

using namespace geode::prelude;

class DemonListPopup : public Popup {
protected:
    DemonCategory m_category = DemonCategory::All;

    bool init(DemonCategory category);
    CCNode* createDemonRow(int rank, DemonEntry const& demon, float width);

public:
    static DemonListPopup* create(DemonCategory category);
};
