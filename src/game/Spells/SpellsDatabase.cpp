#include "SpellsDatabase.h"

#include <Utilities/logger.hpp>

#include <fstream>

const ISpellDescription& SpellsDatabase::GetSpellById(uint64_t spellId) const
{
    auto iter = spells.find(spellId);

    if (iter == spells.end())
        throw std::invalid_argument("Spell not found");

    return *(*iter).second.get();
}

const ISpellDescription& SpellsDatabase::GetSpellByName(const std::string& name) const
{
    for (auto& spell : spells) // HACK: this is very slow method to find spell by name, but for now (just 4 spells) it will serve good
    {
        if (spell.second->GetSpellName() == name)
        {
            return *spell.second.get();
        }
    }

    throw std::invalid_argument("Spell not found");
}
