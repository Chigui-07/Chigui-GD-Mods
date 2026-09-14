#include "DemonData.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>

using namespace geode::prelude;

namespace {
    std::vector<DemonEntry> s_entries;
    bool s_loaded = false;
    std::optional<PendingDemonCompletion> s_pendingCompletion;

    std::string normalizeName(std::string value) {
        std::string result;
        result.reserve(value.size());
        for (auto ch : value) {
            auto c = static_cast<unsigned char>(ch);
            if (std::isalnum(c)) {
                result.push_back(static_cast<char>(std::tolower(c)));
            }
        }
        return result;
    }

    std::string makeKey(PendingDemonCompletion const& completion) {
        if (completion.levelID > 0) {
            return fmt::format(
                "{}:{}",
                completion.official ? "main" : "online",
                completion.levelID
            );
        }
        return "name:" + normalizeName(completion.name);
    }

    bool matchesCompletion(DemonEntry const& entry, PendingDemonCompletion const& completion) {
        auto key = makeKey(completion);
        if (!entry.key.empty() && entry.key == key) {
            return true;
        }

        if (entry.levelID > 0 && completion.levelID > 0 && entry.levelID == completion.levelID) {
            return entry.officialLevelID > 0 == completion.official;
        }

        return normalizeName(entry.name) == normalizeName(completion.name);
    }

    bool answersAreHarder(
        std::array<int, 6> const& lhs,
        std::array<int, 6> const& rhs
    ) {
        for (size_t i = 0; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i]) {
                return lhs[i] > rhs[i];
            }
        }
        return false;
    }

    std::vector<DemonEntry> makeLegacyEntries() {
        struct Seed {
            char const* name;
            int officialID;
        };

        static constexpr Seed seeds[] = {
            {"Deadlocked", 20},
            {"Retro Circles", 0},
            {"Glorious Fortress", 0},
            {"Beautiful and Damned", 0},
            {"Clubstep", 14},
            {"WEIRD", 0},
            {"Otis", 0},
            {"Insomnia", 0},
            {"ISpyWithMyLittleEye", 0},
            {"Sky High", 0},
            {"V01D", 0},
            {"Blindfolder", 0},
            {"Pjork", 0},
            {"Shiver", 0},
            {"Xstep V2", 0},
            {"Blue Hell", 0},
            {"Crescendo", 0},
            {"What is it", 0},
            {"Lights and Thunder", 0},
            {"Platinum Adventure", 0},
            {"Ruins of Destiny", 0},
            {"ABCDEFGHIJKLMNOPQRS", 0},
            {"Demon Mixed", 0},
            {"The Nightmare", 0},
            {"The Lightning Road", 0}
        };

        std::vector<DemonEntry> result;
        result.reserve(std::size(seeds));

        for (size_t i = 0; i < std::size(seeds); ++i) {
            auto const& seed = seeds[i];
            auto official = seed.officialID > 0;
            auto anchor = 80.0 - static_cast<double>(i) * 2.5;

            result.push_back(DemonEntry {
                .key = official
                    ? fmt::format("main:{}", seed.officialID)
                    : "legacy:" + normalizeName(seed.name),
                .name = seed.name,
                .creator = official ? "RobTop" : "",
                .category = DemonCategory::Easy,
                .levelID = seed.officialID,
                .officialLevelID = seed.officialID,
                .anchorScore = anchor,
                .personalScore = -1.0,
                .answers = {0, 0, 0, 0, 0, 0},
                .imported = true,
                .attempts = 0,
                .rebeats = 0
            });
        }

        return result;
    }
}

void initializeDemonData() {
    if (s_loaded) {
        return;
    }

    s_loaded = true;
    s_entries = Mod::get()->getSavedValue<std::vector<DemonEntry>>(
        "demon-entries-v2",
        {}
    );

    // Temporary migration for the development tester. This block will be
    // removed before the public v1.0 release so new users start with their
    // own empty Demon Tracker.
    if (s_entries.empty() && !Mod::get()->getSavedValue<bool>("legacy-seed-v020", false)) {
        s_entries = makeLegacyEntries();
        Mod::get()->setSavedValue("legacy-seed-v020", true);
        saveDemonData();
    }
}

void saveDemonData() {
    Mod::get()->setSavedValue("demon-entries-v2", s_entries);
}

std::vector<DemonEntry> const& getDemonEntries() {
    initializeDemonData();
    return s_entries;
}

std::vector<DemonEntry const*> getDemonEntries(DemonCategory category) {
    initializeDemonData();

    std::vector<DemonEntry const*> result;
    for (auto const& entry : s_entries) {
        if (category == DemonCategory::All || entry.category == category) {
            result.push_back(&entry);
        }
    }
    return result;
}

char const* getDemonCategoryTitle(DemonCategory category) {
    switch (category) {
        case DemonCategory::Easy: return "Easy Demons";
        case DemonCategory::Medium: return "Medium Demons";
        case DemonCategory::Hard: return "Hard Demons";
        case DemonCategory::Insane: return "Insane Demons";
        case DemonCategory::Extreme: return "Extreme Demons";
        default: return "All Demons";
    }
}

int getDemonGlobalRank(DemonEntry const& demon) {
    initializeDemonData();
    for (size_t i = 0; i < s_entries.size(); ++i) {
        if (&s_entries[i] == &demon || s_entries[i].key == demon.key) {
            return static_cast<int>(i) + 1;
        }
    }
    return 0;
}

int getDemonCategoryRank(DemonEntry const& demon) {
    initializeDemonData();
    int rank = 0;
    for (auto const& entry : s_entries) {
        if (entry.category != demon.category) {
            continue;
        }
        ++rank;
        if (&entry == &demon || entry.key == demon.key) {
            return rank;
        }
    }
    return 0;
}

DemonCategory getDemonCategoryForLevel(GJGameLevel* level) {
    if (!level) {
        return DemonCategory::Easy;
    }

    if (level->m_levelType == GJLevelType::Main) {
        return DemonCategory::Easy;
    }

    switch (level->m_demonDifficulty) {
        case static_cast<int>(DemonDifficultyType::EasyDemon):
            return DemonCategory::Easy;
        case static_cast<int>(DemonDifficultyType::MediumDemon):
            return DemonCategory::Medium;
        case static_cast<int>(DemonDifficultyType::InsaneDemon):
            return DemonCategory::Insane;
        case static_cast<int>(DemonDifficultyType::ExtremeDemon):
            return DemonCategory::Extreme;
        default:
            return DemonCategory::Hard;
    }
}

bool hasAllVerifiedCoins(GJGameLevel* level) {
    if (!level) {
        return false;
    }

    if (level->m_coins <= 0 || level->m_coinsVerified.value() != 1) {
        return true;
    }

    auto stats = GameStatsManager::sharedState();
    auto requiredCoins = std::min(level->m_coins, 3);

    for (int i = 1; i <= requiredCoins; ++i) {
        auto key = level->getCoinKey(i);
        auto owned = level->m_levelType == GJLevelType::Main
            ? stats->hasSecretCoin(key)
            : stats->hasUserCoin(key);

        if (!owned) {
            return false;
        }
    }

    return true;
}

bool markRebeatIfTracked(PendingDemonCompletion const& completion) {
    initializeDemonData();

    for (auto& entry : s_entries) {
        if (!matchesCompletion(entry, completion)) {
            continue;
        }

        ++entry.rebeats;
        entry.attempts = std::max(entry.attempts, completion.attempts);

        if (entry.levelID <= 0 && completion.levelID > 0) {
            entry.levelID = completion.levelID;
            entry.officialLevelID = completion.official ? completion.levelID : 0;
            entry.key = makeKey(completion);
        }
        if (entry.creator.empty() && !completion.creator.empty()) {
            entry.creator = completion.creator;
        }

        saveDemonData();
        return true;
    }

    return false;
}

double calculateDifficultyScore(std::array<int, 6> const& answers) {
    auto weightedAverage =
        answers[0] * 0.30 +
        answers[1] * 0.20 +
        answers[2] * 0.15 +
        answers[3] * 0.15 +
        answers[4] * 0.10 +
        answers[5] * 0.10;

    return ((weightedAverage - 1.0) / 9.0) * 100.0;
}

int registerRatedDemon(
    PendingDemonCompletion const& completion,
    std::array<int, 6> const& answers
) {
    initializeDemonData();

    if (markRebeatIfTracked(completion)) {
        for (size_t i = 0; i < s_entries.size(); ++i) {
            if (matchesCompletion(s_entries[i], completion)) {
                return static_cast<int>(i) + 1;
            }
        }
    }

    auto score = calculateDifficultyScore(answers);
    DemonEntry entry {
        .key = makeKey(completion),
        .name = completion.name,
        .creator = completion.creator,
        .category = completion.category,
        .levelID = completion.levelID,
        .officialLevelID = completion.official ? completion.levelID : 0,
        .anchorScore = score,
        .personalScore = score,
        .answers = answers,
        .imported = false,
        .attempts = completion.attempts,
        .rebeats = 0
    };

    size_t insertAt = s_entries.size();
    for (size_t i = 0; i < s_entries.size(); ++i) {
        auto const& current = s_entries[i];

        if (score > current.anchorScore) {
            insertAt = i;
            break;
        }

        if (std::abs(score - current.anchorScore) < 0.0001) {
            if (!current.imported && answersAreHarder(answers, current.answers)) {
                insertAt = i;
                break;
            }
            // Exact ties are deliberately inserted below the existing demon.
        }
    }

    s_entries.insert(s_entries.begin() + static_cast<std::ptrdiff_t>(insertAt), entry);
    saveDemonData();
    return static_cast<int>(insertAt) + 1;
}

void setPendingDemonCompletion(PendingDemonCompletion const& completion) {
    s_pendingCompletion = completion;
}

std::optional<PendingDemonCompletion> takePendingDemonCompletion() {
    auto result = s_pendingCompletion;
    s_pendingCompletion.reset();
    return result;
}
