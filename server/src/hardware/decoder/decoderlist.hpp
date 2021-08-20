/**
 * server/src/hardware/decoder/decoderlist.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019-2020 Reinder Feenstra
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


#ifndef TRAINTASTIC_SERVER_HARDWARE_DECODER_DECODERLIST_HPP
#define TRAINTASTIC_SERVER_HARDWARE_DECODER_DECODERLIST_HPP

#include "../../core/objectlist.hpp"
#include "decoder.hpp"

class DecoderList : public ObjectList<Decoder>
{
  protected:
    void worldEvent(WorldState state, WorldEvent event) final;
    bool isListedProperty(const std::string& name) final;

  public:
    CLASS_ID("decoder_list")

    Method<std::shared_ptr<Decoder>()> add;
    Method<void(const std::shared_ptr<Decoder>&)> remove;

    DecoderList(Object& _parent, const std::string& parentPropertyName);

    TableModelPtr getModel() final;
};

#endif
