#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "data/DemonData.hpp"
#include "ui/DemonRatingPopup.hpp"

using namespace geode::prelude;

class $modify(DemonTrackerPlayLayer, PlayLayer) {
    void levelComplete() {
        auto level = m_level;
        auto wasPractice = m_isPracticeMode;

        PlayLayer::levelComplete();

        if (!level || wasPractice) {
            return;
        }

        if (level->m_demon.value() != 1) {
            return;
        }

        auto official = level->m_levelType == GJLevelType::Main;
        PendingDemonCompletion completion {
            .name = level->m_levelName.c_str(),
            .creator = official ? "RobTop" : level->m_creatorName.c_str(),
            .category = getDemonCategoryForLevel(level),
            .levelID = level->m_levelID.value(),
            .official = official,
            .coinCount = level->m_coins,
            .attempts = level->m_attempts.value()
        };

        if (markRebeatIfTracked(completion)) {
            log::info("Demon Tracker: rebeat registrado para {}", completion.name);
            return;
        }

        if (!hasAllVerifiedCoins(level)) {
            log::info(
                "Demon Tracker: {} llego al 100%, pero aun faltan monedas verificadas",
                completion.name
            );
            return;
        }

        setPendingDemonCompletion(completion);
        log::info("Demon Tracker: nuevo demon listo para clasificar: {}", completion.name);
    }
};

class $modify(DemonTrackerEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        auto pending = takePendingDemonCompletion();
        if (!pending.has_value()) {
            return;
        }

        if (auto popup = DemonRatingPopup::create(*pending)) {
            popup->show();
        }
    }
};
