#include "DemonData.hpp"

std::vector<DemonEntry> const& getDemonEntries() {
    static const std::vector<DemonEntry> entries = {
        {"Deadlocked", DemonCategory::Easy},
        {"Retro Circles", DemonCategory::Easy},
        {"Glorious Fortress", DemonCategory::Easy},
        {"Beautiful and Damned", DemonCategory::Easy},
        {"Clubstep", DemonCategory::Easy},
        {"WEIRD", DemonCategory::Easy},
        {"Otis", DemonCategory::Easy},
        {"Insomnia", DemonCategory::Easy},
        {"ISpyWithMyLittleEye", DemonCategory::Easy},
        {"Sky High", DemonCategory::Easy},
        {"V01D", DemonCategory::Easy},
        {"Blindfolder", DemonCategory::Easy},
        {"Pjork", DemonCategory::Easy},
        {"Shiver", DemonCategory::Easy},
        {"Xstep V2", DemonCategory::Easy},
        {"Blue Hell", DemonCategory::Easy},
        {"Crescendo", DemonCategory::Easy},
        {"What is it", DemonCategory::Easy},
        {"Lights and Thunder", DemonCategory::Easy},
        {"Platinum Adventure", DemonCategory::Easy},
        {"Ruins of Destiny", DemonCategory::Easy},
        {"ABCDEFGHIJKLMNOPQRS", DemonCategory::Easy},
        {"Demon Mixed", DemonCategory::Easy},
        {"The Nightmare", DemonCategory::Easy},
        {"The Lightning Road", DemonCategory::Easy}
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
