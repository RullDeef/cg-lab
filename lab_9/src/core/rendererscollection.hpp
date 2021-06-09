#pragma once

// segment renderers
#include "line/bresintsegren.hpp"
#include "line/bresrealsegren.hpp"
#include "line/bressmoothsegren.hpp"
#include "line/ddasegren.hpp"
#include "line/librarysegren.hpp"
#include "line/wusegren.hpp"

// circle renderers
#include "circle/brescirren.hpp"
#include "circle/canoncirren.hpp"
#include "circle/libcirren.hpp"
#include "circle/mediancirren.hpp"
#include "circle/paramcirren.hpp"

// ellipse renderers
#include "ellipse/bresellren.hpp"
#include "ellipse/canonellren.hpp"
#include "ellipse/libellren.hpp"
#include "ellipse/medianellren.hpp"
#include "ellipse/paramellren.hpp"

namespace core
{
    template<typename PrimitiveBase>
    RenderersContainer<PrimitiveBase> getRenderers();

    template<>
    RenderersContainer<Segment> getRenderers<Segment>()
    {
        static BresIntSegmentRenderer bresintsegren;
        static BresRealSegmentRenderer bresrealsegren;
        static BresSmoothSegmentRenderer bressmoothsegren;
        static DDASegmentRenderer ddasegren;
        static LibSegmentRenderer libsegren;
        static WuSegmentRenderer wusegren;

        return {
            { bresintsegren.getName(), (PrimitiveRenderer<Segment>*)&bresintsegren },
            { bresrealsegren.getName(), (PrimitiveRenderer<Segment>*)&bresrealsegren },
            { bressmoothsegren.getName(), (PrimitiveRenderer<Segment>*)&bressmoothsegren },
            { ddasegren.getName(), (PrimitiveRenderer<Segment>*)&ddasegren },
            { libsegren.getName(), (PrimitiveRenderer<Segment>*)&libsegren },
            { wusegren.getName(), (PrimitiveRenderer<Segment>*)&wusegren }
        };
    }

    template<>
    RenderersContainer<Circle> getRenderers<Circle>()
    {
        static LibraryCircleRenderer libcirren;
        static ParametricCircleRenderer paramcirren;
        static BresenhemCircleRenderer brescirren;
        static MedianCircleRenderer mediancirren;
        static CanonicCircleRenderer canoncirren;

        return {
            { libcirren.getName(), (PrimitiveRenderer<Circle>*)&libcirren },
            { paramcirren.getName(), (PrimitiveRenderer<Circle>*)&paramcirren },
            { brescirren.getName(), (PrimitiveRenderer<Circle>*)&brescirren },
            { mediancirren.getName(), (PrimitiveRenderer<Circle>*)&mediancirren },
            { canoncirren.getName(), (PrimitiveRenderer<Circle>*)&canoncirren }
        };
    }

    template<>
    RenderersContainer<Ellipse> getRenderers<Ellipse>()
    {
        static LibraryEllipseRenderer libellren;
        static ParametricEllipseRenderer paramellren;
        static BresenhemEllipseRenderer bresellren;
        static CanonicEllipseRenderer canonellren;

        return {
            { libellren.getName(), (PrimitiveRenderer<Ellipse>*) & libellren },
            { paramellren.getName(), (PrimitiveRenderer<Ellipse>*) & paramellren },
            { bresellren.getName(), (PrimitiveRenderer<Ellipse>*) & bresellren },
            { canonellren.getName(), (PrimitiveRenderer<Ellipse>*) & canonellren }
        };
    }
}
