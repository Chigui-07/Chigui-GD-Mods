#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../data/DemonData.hpp"

using namespace geode::prelude;

class DemonDetailPopup : public Popup {
protected:
    DemonEntry const* m_demon = nullptr;
    int m_globalRank = 0;
    int m_categoryRank = 0;

    bool init(DemonEntry const* demon, int globalRank, int categoryRank);
    void onOpenLevel(CCObject* sender);
    void onPlayOfficial(CCObject* sender);

public:
    static DemonDetailPopup* create(
        DemonEntry const* demon,
        int globalRank,
        int categoryRank
    );
};
