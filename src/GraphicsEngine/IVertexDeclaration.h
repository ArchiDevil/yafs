#pragma once

#include <memory>

namespace ShiftEngine
{
class IVertexDeclaration
{
public:
    virtual ~IVertexDeclaration() = default;
    virtual void Bind() = 0;
};

using IVertexDeclarationPtr = std::shared_ptr<IVertexDeclaration>;
}
