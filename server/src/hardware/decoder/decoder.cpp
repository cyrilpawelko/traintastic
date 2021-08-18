/**
 * server/src/hardware/decoder/decoder.cpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019-2021 Reinder Feenstra
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

#include "decoder.hpp"
#include "decoderlist.hpp"
#include "decoderlisttablemodel.hpp"
#include "decoderchangeflags.hpp"
#include "decoderfunction.hpp"
#include "decoderfunctions.hpp"
#include "../../world/world.hpp"
#include "../commandstation/commandstation.hpp"
#include "../../core/attributes.hpp"
#include "../../utils/displayname.hpp"
#include "../../utils/almostzero.hpp"

//constexpr uint16_t addressDCCMin = 1;
constexpr uint16_t addressDCCShortMax = 127;

const std::shared_ptr<Decoder> Decoder::null;

Decoder::Decoder(const std::weak_ptr<World>& world, std::string_view _id) :
  IdObject(world, _id),
  name{this, "name", "", PropertyFlags::ReadWrite | PropertyFlags::Store},
  commandStation{this, "command_station", nullptr, PropertyFlags::ReadWrite | PropertyFlags::Store,
    [this](const std::shared_ptr<CommandStation>& value)
    {
      std::shared_ptr<Decoder> decoder = std::dynamic_pointer_cast<Decoder>(shared_from_this());
      assert(decoder);

      //if(value)
      // TODO: check compatible??

      if(commandStation)
        commandStation->decoders->removeObject(decoder);

      if(value)
        value->decoders->addObject(decoder);

      return true;
    }},
  protocol{this, "protocol", DecoderProtocol::Auto, PropertyFlags::ReadWrite | PropertyFlags::Store,
    [this](const DecoderProtocol& value)
    {
      if(value == DecoderProtocol::DCC && address > addressDCCShortMax)
        longAddress = true;
      updateEditable();
    }},
  address{this, "address", 0, PropertyFlags::ReadWrite | PropertyFlags::Store,
    [this](const uint16_t& value)
    {
      if(protocol == DecoderProtocol::DCC)
      {
        if(value > addressDCCShortMax)
          longAddress = true;
        updateEditable();
      }
    }},
  longAddress{this, "long_address", false, PropertyFlags::ReadWrite | PropertyFlags::Store},
  emergencyStop{this, "emergency_stop", false, PropertyFlags::ReadWrite,
    [this](const bool&)
    {
      changed(DecoderChangeFlags::EmergencyStop);
      updateEditable();
    }},
  direction{this, "direction", Direction::Forward, PropertyFlags::ReadWrite,
    [this](const Direction&)
    {
      changed(DecoderChangeFlags::Direction);
    }},
  speedSteps{this, "speed_steps", speedStepsAuto, PropertyFlags::ReadWrite | PropertyFlags::Store,
    [this](const uint8_t&)
    {
      changed(DecoderChangeFlags::SpeedSteps);
    }},
  throttle{this, "throttle", throttleMin, PropertyFlags::ReadWrite,
    [this](const double& value)
    {
      changed(DecoderChangeFlags::Throttle);
      updateEditable();
    }},
  functions{this, "functions", nullptr, PropertyFlags::ReadOnly | PropertyFlags::Store | PropertyFlags::SubObject},
  notes{this, "notes", "", PropertyFlags::ReadWrite | PropertyFlags::Store}
{
  functions.setValueInternal(std::make_shared<DecoderFunctions>(*this, functions.name()));

  auto w = world.lock();

  m_worldMute = contains(w->state.value(), WorldState::Mute);
  m_worldNoSmoke = contains(w->state.value(), WorldState::NoSmoke);

//  const bool editable = w && contains(w->state.value(), WorldState::Edit) && speedStep == 0;

  Attributes::addDisplayName(name, DisplayName::Object::name);
  Attributes::addEnabled(name, false);
  m_interfaceItems.add(name);
  Attributes::addEnabled(commandStation, false);
  Attributes::addObjectList(commandStation, w->commandStations);
  m_interfaceItems.add(commandStation);
  Attributes::addEnabled(protocol, false);
  Attributes::addValues(protocol, DecoderProtocolValues);
  m_interfaceItems.add(protocol);
  Attributes::addEnabled(address, false);
  m_interfaceItems.add(address);
  Attributes::addEnabled(longAddress, false);
  m_interfaceItems.add(longAddress);
  Attributes::addObjectEditor(emergencyStop, false);
  m_interfaceItems.add(emergencyStop);
  Attributes::addValues(direction, DirectionValues);
  Attributes::addObjectEditor(direction, false);
  m_interfaceItems.add(direction);
  Attributes::addEnabled(speedSteps, false);
  m_interfaceItems.add(speedSteps);
  Attributes::addMinMax(throttle, throttleMin, throttleMax);
  Attributes::addObjectEditor(throttle, false);
  m_interfaceItems.add(throttle);
  m_interfaceItems.add(functions);
  Attributes::addDisplayName(notes, DisplayName::Object::notes);
  m_interfaceItems.add(notes);

  updateEditable();
}

void Decoder::addToWorld()
{
  IdObject::addToWorld();

  if(auto world = m_world.lock())
    world->decoders->addObject(shared_ptr<Decoder>());
}

bool Decoder::hasFunction(uint32_t number) const
{
  for(auto& f : *functions)
    if(f->number == number)
      return true;
  return false;
}

const std::shared_ptr<DecoderFunction>& Decoder::getFunction(uint32_t number) const
{
  for(auto& f : *functions)
    if(f->number == number)
      return f;

  return DecoderFunction::null;
}

const std::shared_ptr<DecoderFunction>& Decoder::getFunction(DecoderFunctionFunction function) const
{
  for(auto& f : *functions)
    if(f->function == function)
      return f;

  return DecoderFunction::null;
}

bool Decoder::getFunctionValue(uint32_t number) const
{
  return getFunctionValue(getFunction(number));
}

bool Decoder::getFunctionValue(const std::shared_ptr<DecoderFunction>& function) const
{
  if(!function)
    return false;

  assert(this == &function->decoder());

  // Apply mute/noSmoke world states:
  if(m_worldMute)
  {
    if(function->function == DecoderFunctionFunction::Mute)
      return true;
    else if(function->function == DecoderFunctionFunction::Sound && !getFunction(DecoderFunctionFunction::Mute))
      return false;
  }
  if(m_worldNoSmoke)
  {
    if(function->function == DecoderFunctionFunction::Smoke)
      return false;
  }

  return function->value;
}

void Decoder::setFunctionValue(uint32_t number, bool value)
{
  const auto& f = getFunction(number);
  if(f && getFunctionValue(f) != value)
    f->value.setValueInternal(value);
}

void Decoder::worldEvent(WorldState state, WorldEvent event)
{
  IdObject::worldEvent(state, event);
  updateEditable(contains(state, WorldState::Edit));

  // Handle mute/noSmoke world states:
  m_worldMute = contains(state, WorldState::Mute);
  m_worldNoSmoke = contains(state, WorldState::NoSmoke);

  if(event == WorldEvent::Mute || event == WorldEvent::Unmute)
  {
    bool hasMute = false;

    for(auto& f : *functions)
      if(f->function == DecoderFunctionFunction::Mute)
      {
        if(!f->value)
          changed(DecoderChangeFlags::FunctionValue, f->number);
        hasMute = true;
      }

    if(!hasMute)
    {
      for(auto& f : *functions)
        if(f->function == DecoderFunctionFunction::Sound && f->value)
          changed(DecoderChangeFlags::FunctionValue, f->number);
    }
  }
  else if(event == WorldEvent::NoSmoke || event == WorldEvent::Smoke)
  {
    for(auto& f : *functions)
      if(f->function == DecoderFunctionFunction::Smoke && f->value)
        changed(DecoderChangeFlags::FunctionValue, f->number);
  }
}

void Decoder::updateEditable()
{
  auto w = m_world.lock();
  updateEditable(w && contains(w->state.value(), WorldState::Edit));
}

void Decoder::updateEditable(bool editable)
{
  const bool stopped = editable && almostZero(throttle.value());
  name.setAttributeEnabled(editable);
  commandStation.setAttributeEnabled(stopped);
  protocol.setAttributeEnabled(stopped);
  address.setAttributeEnabled(stopped);
  longAddress.setAttributeEnabled(stopped && protocol == DecoderProtocol::DCC && address < addressDCCShortMax);
  speedSteps.setAttributeEnabled(stopped);
}

void Decoder::changed(DecoderChangeFlags changes, uint32_t functionNumber)
{
  if(commandStation)
    commandStation->decoderChanged(*this, changes, functionNumber);
}
