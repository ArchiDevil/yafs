#include "SpellsDatabase.h"

#include <Utilities/logger.hpp>

#include <json/json.hpp>

#include <fstream>

using json = nlohmann::json;

void SpellsDatabase::LoadSpellsFromFile(const std::string& filename)
{
    std::ifstream stream {filename};
    if (!stream.is_open())
        return;

    json jsonDocument = json::parse(stream);
    for (auto& param : jsonDocument)
    {
        for (auto& arrayElement : param)
        {
            try
            {
                std::string name = arrayElement["name"];
                std::hash<std::string> nameHasher;
                uint64_t hash = (uint64_t)nameHasher(name);
                LOG_INFO("Loading spell: ", name);
                std::string type = arrayElement["type"];
                float cooldown = arrayElement["cooldown"];

                if (type == "projectile")
                {
                    float damage = arrayElement["damage"];
                    float spread = arrayElement["spread"];
                    float energy = arrayElement["energy"];
                    spells[hash] = std::make_unique<ProjectileSpellDescription>(name, cooldown, damage, spread, energy);
                }
                else if (type == "multiprojectile")
                {
                    float damage = arrayElement["damage"];
                    float spread = arrayElement["spread"];
                    unsigned count = arrayElement["count"];
                    float per_bullet_spread = arrayElement["per-bullet-spread"];
                    float energy = arrayElement["energy"];
                    spells[hash] = std::make_unique<MultiProjectileSpellDescription>(name, cooldown, damage, spread, count, per_bullet_spread, energy);
                }
                else if (type == "detectormine")
                {
                    float damage = arrayElement["damage"];
                    float energy = arrayElement["energy"];
                    spells[hash] = std::make_unique<DetectorMineSpellDescription>(name, cooldown, damage, energy);
                }
                else if (type == "timedmine")
                {
                    float damage = arrayElement["damage"];
                    float energy = arrayElement["energy"];
                    double time = arrayElement["energy"];
                    spells[hash] = std::make_unique<TimedMineSpellDescription>(name, cooldown, damage, energy, time);
                }
                else
                {
                    throw std::invalid_argument("Wrong type");
                }
            }
            catch (const std::exception& e)
            {
                LOG_ERROR("Exception while loading spell: ", e.what());
                continue;
            }
        }
    }
}

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
