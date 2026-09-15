#pragma once

#include <Geode/Geode.hpp>
#include <array>
#include <optional>
#include <string>
#include <vector>

enum class DemonCategory {
    All,
    Easy,
    Medium,
    Hard,
    Insane,
    Extreme
};

struct DemonEntry {
    std::string key;
    std::string name;
    std::string creator;
    DemonCategory category = DemonCategory::Easy;
    int levelID = 0;
    int officialLevelID = 0;
    double anchorScore = 0.0;
    double personalScore = -1.0;
    std::array<int, 6> answers {0, 0, 0, 0, 0, 0};
    bool imported = false;
    int attempts = 0;
    int rebeats = 0;
};

struct PendingDemonCompletion {
    std::string name;
    std::string creator;
    DemonCategory category = DemonCategory::Easy;
    int levelID = 0;
    bool official = false;
    int coinCount = 0;
    int attempts = 0;
};

template <>
struct matjson::Serialize<DemonEntry> {
    static matjson::Value toJson(DemonEntry const& value) {
        return matjson::makeObject({
            {"key", value.key},
            {"name", value.name},
            {"creator", value.creator},
            {"category", static_cast<int>(value.category)},
            {"levelID", value.levelID},
            {"officialLevelID", value.officialLevelID},
            {"anchorScore", value.anchorScore},
            {"personalScore", value.personalScore},
            {"q1", value.answers[0]},
            {"q2", value.answers[1]},
            {"q3", value.answers[2]},
            {"q4", value.answers[3]},
            {"q5", value.answers[4]},
            {"q6", value.answers[5]},
            {"imported", value.imported},
            {"attempts", value.attempts},
            {"rebeats", value.rebeats}
        });
    }

    static geode::Result<DemonEntry> fromJson(matjson::Value const& value) {
        DemonEntry entry;
        entry.key = GEODE_UNWRAP(value["key"].asString());
        entry.name = GEODE_UNWRAP(value["name"].asString());
        entry.creator = GEODE_UNWRAP(value["creator"].asString());
        entry.category = static_cast<DemonCategory>(GEODE_UNWRAP(value["category"].asInt()));
        entry.levelID = GEODE_UNWRAP(value["levelID"].asInt());
        entry.officialLevelID = GEODE_UNWRAP(value["officialLevelID"].asInt());
        entry.anchorScore = GEODE_UNWRAP(value["anchorScore"].asDouble());
        entry.personalScore = GEODE_UNWRAP(value["personalScore"].asDouble());
        entry.answers[0] = GEODE_UNWRAP(value["q1"].asInt());
        entry.answers[1] = GEODE_UNWRAP(value["q2"].asInt());
        entry.answers[2] = GEODE_UNWRAP(value["q3"].asInt());
        entry.answers[3] = GEODE_UNWRAP(value["q4"].asInt());
        entry.answers[4] = GEODE_UNWRAP(value["q5"].asInt());
        entry.answers[5] = GEODE_UNWRAP(value["q6"].asInt());
        entry.imported = GEODE_UNWRAP(value["imported"].asBool());
        entry.attempts = GEODE_UNWRAP(value["attempts"].asInt());
        entry.rebeats = GEODE_UNWRAP(value["rebeats"].asInt());
        return geode::Ok(entry);
    }
};

void initializeDemonData();
void saveDemonData();

std::vector<DemonEntry> const& getDemonEntries();
std::vector<DemonEntry const*> getDemonEntries(DemonCategory category);
char const* getDemonCategoryTitle(DemonCategory category);
int getDemonGlobalRank(DemonEntry const& demon);
int getDemonCategoryRank(DemonEntry const& demon);

DemonCategory getDemonCategoryForLevel(GJGameLevel* level);
bool hasAllVerifiedCoins(GJGameLevel* level);
bool markRebeatIfTracked(PendingDemonCompletion const& completion);
double calculateDifficultyScore(std::array<int, 6> const& answers);
int registerRatedDemon(
    PendingDemonCompletion const& completion,
    std::array<int, 6> const& answers
);

void setPendingDemonCompletion(PendingDemonCompletion const& completion);
std::optional<PendingDemonCompletion> takePendingDemonCompletion();
