#pragma once

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
    std::string name;
    DemonCategory category;
};

std::vector<DemonEntry> const& getDemonEntries();
std::vector<DemonEntry const*> getDemonEntries(DemonCategory category);
char const* getDemonCategoryTitle(DemonCategory category);
