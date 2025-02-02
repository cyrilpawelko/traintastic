/**
 * server/src/hardware/output/list/outputlist.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019-2023 Reinder Feenstra
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

#ifndef TRAINTASTIC_SERVER_HARDWARE_OUTPUT_LIST_OUTPUTLIST_HPP
#define TRAINTASTIC_SERVER_HARDWARE_OUTPUT_LIST_OUTPUTLIST_HPP

#include "../../../core/objectlist.hpp"
#include "outputlistcolumn.hpp"
#include "../../../core/method.hpp"
#include "../output.hpp"

class OutputKeyboard;

class OutputList : public ObjectList<Output>
{
  CLASS_ID("list.output")

  protected:
    void worldEvent(WorldState state, WorldEvent event) final;
    bool isListedProperty(std::string_view name) final;

  public:
    const OutputListColumn columns;

    Method<std::shared_ptr<Output>()> create;
    Method<void(const std::shared_ptr<Output>&)> delete_;
    Method<std::shared_ptr<OutputKeyboard>()> outputKeyboard;
    Method<std::shared_ptr<OutputKeyboard>(uint32_t)> outputKeyboardChannel;

    OutputList(Object& _parent, std::string_view parentPropertyName, OutputListColumn _columns);

    TableModelPtr getModel() final;
};

#endif
