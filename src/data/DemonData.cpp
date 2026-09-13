#include "DemonData.hpp"

std::vector<DemonEntry> const& getDemonEntries() {
    static const std::vector<DemonEntry> entries = {
        {"Deadlocked", DemonCategory::Easy, 20},
        {"Retro Circles", DemonCategory::Easy, 0},
        {"Glorious Fortress", DemonCategory::Easy, 0},
        {"Beautiful and Damned", DemonCategory::Easy, 0},
        {"Clubstep", DemonCategory::Easy, 14},
        {"WEIRD", DemonCategory::Easy, 0},
        {"Otis", DemonCategory::Easy, 0},
        {"Insomnia", DemonCategory::Easy, 0},
        {"ISpyWithMyLittleEye", DemonCategory::Easy, 0},
        {"Sky High", DemonCategory::Easy, 0},
        {"V01D", DemonCategory::Easy, 0},
        {"Blindfolder", DemonCategory::Easy, 0},
        {"Pjork", DemonCategory::Easy, 0},
        {"Shiver", DemonCategory::Easy, 0},
        {"Xstep V2", DemonCategory::Easy, 0},
        {"Blue Hell", DemonCategory::Easy, 0},
        {"Crescendo", DemonCategory::Easy, 0},
        {"What is it", DemonCategory::Easy, 0},
        {"Lights and Thunder", DemonCategory::Easy, 0},
        {"Platinum Adventure", DemonCategory::Easy, 0},
        {"Ruins of Destiny", DemonCategory::Easy, 0},
        {"ABCDEFGHIJKLMNOPQRS", DemonCategory::Easy, 0},
        {"Demon Mixed", DemonCategory::Easy, 0},
        {"The Nightmare", DemonCategory::Easy, 0},
        {"The Lightning Road", DemonCategory::Easy, 0}
    };
    return entries;
}

std::vector<DemonEntry const*> getDemonEntries(DemonCategory category) {
    std::vector<DemonEntry const*> result;
    for (auto const& entry : getDemonEntries()) {
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
    auto const& entries = getDemonEntries();
    for (size_t i = 0; i < entries.size(); ++i) {
        if (&entries[i] == &demon) {
            return static_cast<int>(i) + 1;
        }
    }
    return 0;
}
