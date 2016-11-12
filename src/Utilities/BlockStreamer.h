#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

class StreamBlock
{
public:
    StreamBlock(uint32_t _id, uint32_t _size, const uint8_t * _data);
    StreamBlock(const StreamBlock & ref);

    StreamBlock & operator=(const StreamBlock & ref);

    uint32_t GetId() const;
    uint32_t GetSize() const;
    uint8_t * GetData() const;

private:
    uint32_t id;
    uint32_t size;
    std::unique_ptr<uint8_t[]> data;
};

class BlockStreamer
{
public:
    ~BlockStreamer();

    bool AddBlock(uint32_t id, size_t size, const uint8_t * data);
    bool RemoveBlock(uint32_t block_id);
    const std::vector<std::unique_ptr<StreamBlock>> & GetBlocks() const;

    bool Save(const std::wstring & filename) const;
    bool Load(const std::wstring & filename);

private:
    void Clear();

    std::vector<std::unique_ptr<StreamBlock>> blocks;

};
