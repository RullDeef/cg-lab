#pragma once

#include "core/basecutter.hpp"


namespace core
{
    class WeilerAtherton : public PolyCutter
    {
    public:
        WeilerAtherton() : PolyCutter(u8"Вейлер-Азертон") {}
        virtual void cut(BasicRegion& region, const BasicRegion& cutter) override;
    };
}
