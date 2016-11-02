#pragma once

#include <unordered_map>
#include <memory>

#include "SpellsDescription.h"

// This database is created to store spells
// Every spell knows how to cast itself
// Spells could be loaded from file (not implemented yet)
// Every spell should be used as spell id to simplify storing and using
// Any spell could be easily obtained from here by it's id
class SpellsDatabase final
{
public:
    SpellsDatabase() = default;
    ~SpellsDatabase() = default;

    const ISpellDescription& GetSpellByName(const std::string& name) const;
    const ISpellDescription& GetSpellById(uint64_t spellId) const;

private:
    std::unordered_map<uint64_t, std::unique_ptr<ISpellDescription>> spells;

};
