#include "BlockStreamer.h"

#include <fstream>
#include <algorithm>

//for memcpy
#include <cstring>

static const uint32_t magic = 0xf0f0f0f0;

StreamBlock::StreamBlock(uint32_t _id, uint32_t _size, const uint8_t * _data)
    : id(_id)
    , size(_size)
{
    data.reset(new uint8_t[size]);
    memcpy(data.get(), _data, size);
}

StreamBlock::StreamBlock(const StreamBlock & ref)
{
    *this = ref;
}

uint32_t StreamBlock::GetId() const
{
    return id;
}

uint32_t StreamBlock::GetSize() const
{
    return size;
}

uint8_t * StreamBlock::GetData() const
{
    return data.get();
}

StreamBlock & StreamBlock::operator=(const StreamBlock & ref)
{
    if (&ref == this)
        return *this;

    this->id = ref.id;
    this->size = ref.size;
    data.reset(new uint8_t[size]);
    memcpy(data.get(), ref.data.get(), size);
    return *this;
}

BlockStreamer::~BlockStreamer()
{
    Clear();
}

bool BlockStreamer::AddBlock(uint32_t id, size_t size, const uint8_t * data)
{
    for (auto & elem : blocks)
        if (elem->GetId() == id)
            return false;

    blocks.emplace_back(std::make_unique<StreamBlock>(id, size, data));
    return true;
}

bool BlockStreamer::RemoveBlock(uint32_t block_id)
{
    for (size_t i = 0; i < blocks.size(); i++)
    {
        if (blocks[i]->GetId() == block_id)
        {
            std::swap(blocks[i], blocks.back());
            blocks.pop_back();
            return true;
        }
    }

    return false;
}

bool BlockStreamer::Save(const std::wstring & filename) const
{
#if defined WIN32 || defined _WIN32
    std::ofstream out;
    out.open(filename, std::ios_base::binary);
    if (!out.is_open() || out.fail())
        return false;
    //save header...
    //magic number
    out.write((const char*)&magic, sizeof(uint32_t));
    //writing blocks...
    for (auto & elem : blocks)
    {
        uint32_t id = elem->GetId();
        out.write((const char*)&id, sizeof(uint32_t));
        uint32_t size = elem->GetSize();
        out.write((const char*)&size, sizeof(uint32_t));
        void * data = elem->GetData();
        out.write((const char*)data, elem->GetSize());
    }
    out.close();
#endif
    return true;
}

bool BlockStreamer::Load(const std::wstring & filename)
{
#if defined WIN32 || defined _WIN32
    std::ifstream in;
    in.open(filename, std::ios_base::binary);
    if (!in.is_open() || in.fail())
        return false;

    uint32_t somemagic = 0;
    in.read((char*)&somemagic, sizeof(uint32_t));

    if (in.gcount() < sizeof(uint32_t))
        return false;
    if (somemagic != magic)
        return false;

    while (!in.eof())
    {
        uint32_t id = 0;
        uint32_t size = 0;

        in.read((char*)&id, sizeof(uint32_t));
        uint32_t readBytes = (uint32_t)in.gcount();
        if (readBytes < sizeof(uint32_t) && readBytes > 0)
        {
            Clear();
            in.close();
            return false;
        }
        else if (readBytes == 0)
        {
            break;
        }

        in.read((char*)&size, sizeof(uint32_t));
        if (in.gcount() < sizeof(uint32_t))
        {
            Clear();
            in.close();
            return false;
        }

        std::unique_ptr<uint8_t> data {new uint8_t[size]};
        in.read((char*)data.get(), size);
        if (in.gcount() < size)
        {
            Clear();
            in.close();
            return false;
        }

        blocks.emplace_back(std::make_unique<StreamBlock>(id, size, data.get()));
    }

    in.close();
#endif
    return true;
}

void BlockStreamer::Clear()
{
    blocks.clear();
}

const std::vector<std::unique_ptr<StreamBlock>> & BlockStreamer::GetBlocks() const
{
    return blocks;
}
