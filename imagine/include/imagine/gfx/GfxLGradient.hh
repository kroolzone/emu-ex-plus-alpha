#pragma once

/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/config/defs.hh>
#include <imagine/gfx/defs.hh>
#include <imagine/gfx/GeomQuadMesh.hh>

namespace Gfx
{

struct LGradientStopDesc
{
	GC pos;
	VertexColor color;
};

class LGradient
{
public:
	constexpr LGradient() {}

	void draw(RendererCommands &r) const;
	void setColor(ColorComp r, ColorComp g, ColorComp b);
	void setTranslucent(ColorComp a);
	void setColorStop(ColorComp r, ColorComp g, ColorComp b, uint32_t i);
	void setTranslucentStop(ColorComp a, uint32_t i);
	void setPos(const LGradientStopDesc *stop, uint32_t stops, GC x, GC y, GC x2, GC y2);
	void setPos(const LGradientStopDesc *stop, uint32_t stops, const GCRect &d);
	uint32_t stops() const;
	explicit operator bool() const;

protected:
	GeomQuadMesh g{};
	uint32_t stops_ = 0;
};

}
