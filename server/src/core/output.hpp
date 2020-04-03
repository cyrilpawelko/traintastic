/**
 * server/src/core/output.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019 Reinder Feenstra
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TRAINTASTIC_SERVER_CORE_OUTPUT_HPP
#define TRAINTASTIC_SERVER_CORE_OUTPUT_HPP

#include "idobject.hpp"

class Output : public IdObject
{
  protected:
    virtual void valueChanged(bool value) {}
    virtual bool setValue(bool& value) { return true; }

  public:
    Property<bool> value;

    Output(const std::weak_ptr<World> world, const std::string& _id);
};

#endif
