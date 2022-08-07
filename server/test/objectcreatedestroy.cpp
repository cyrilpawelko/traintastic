/**
 * server/test/objectcreatedestroy.cpp
 *
 * This file is part of the traintastic test suite.
 *
 * Copyright (C) 2021-2022 Reinder Feenstra
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

#include <catch2/catch.hpp>
#include "../src/world/world.hpp"
#include "../src/board/board.hpp"
#include "../src/hardware/input/input.hpp"
#include "interfaces.hpp"

TEST_CASE("Create world => destroy world", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());

  world.reset();
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and board => destroy world", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->boards->length == 0);

  std::weak_ptr<Board> boardWeak = world->boards->add();
  REQUIRE_FALSE(boardWeak.expired());
  REQUIRE(boardWeak.lock()->getClassId() == Board::classId);
  REQUIRE(worldWeak.lock()->boards->length == 1);

  world.reset();
  REQUIRE(boardWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and board => destroy board", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->boards->length == 0);

  std::weak_ptr<Board> boardWeak = world->boards->add();
  REQUIRE_FALSE(boardWeak.expired());
  REQUIRE(worldWeak.lock()->boards->length == 1);

  world->boards->remove(boardWeak.lock());
  REQUIRE(boardWeak.expired());
  REQUIRE(worldWeak.lock()->boards->length == 0);

  world.reset();
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world and interface => destroy world", "[object-create-destroy]", INTERFACES)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(interfaceWeak.lock()->getClassId() == TestType::classId);

  world.reset();
  REQUIRE(interfaceWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world and interface => destroy interface", "[object-create-destroy]", INTERFACES)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);

  world->interfaces->remove(interfaceWeak.lock());
  REQUIRE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);

  world.reset();
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and decoder => destroy world", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());

  std::weak_ptr<Decoder> decoderWeak = world->decoders->add();
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE(decoderWeak.lock()->getClassId() == Decoder::classId);

  world.reset();
  REQUIRE(decoderWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and decoder => destroy decoder", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 0);

  std::weak_ptr<Decoder> decoderWeak = world->decoders->add();
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 1);

  world->decoders->remove(decoderWeak.lock());
  REQUIRE(decoderWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 0);

  world.reset();
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world, decoder and function => destroy world", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 0);

  std::weak_ptr<Decoder> decoderWeak = world->decoders->add();
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 1);

  std::weak_ptr<DecoderFunctions> functionsWeak = decoderWeak.lock()->functions->shared_ptr<DecoderFunctions>();
  REQUIRE_FALSE(functionsWeak.expired());
  REQUIRE(functionsWeak.lock()->getClassId() == DecoderFunctions::classId);

  functionsWeak.lock()->add();
  REQUIRE(functionsWeak.lock()->items.size() == 1);
  std::weak_ptr<DecoderFunction> functionWeak = functionsWeak.lock()->items[0];
  REQUIRE_FALSE(functionWeak.expired());
  REQUIRE(functionWeak.lock()->getClassId() == DecoderFunction::classId);

  world.reset();
  REQUIRE(functionWeak.expired());
  REQUIRE(functionsWeak.expired());
  REQUIRE(decoderWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world, decoder and function => destroy function", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 0);

  std::weak_ptr<Decoder> decoderWeak = world->decoders->add();
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE(worldWeak.lock()->decoders->length == 1);

  std::weak_ptr<DecoderFunctions> functionsWeak = decoderWeak.lock()->functions->shared_ptr<DecoderFunctions>();
  REQUIRE_FALSE(functionsWeak.expired());

  functionsWeak.lock()->add();
  REQUIRE(functionsWeak.lock()->items.size() == 1);
  std::weak_ptr<DecoderFunction> functionWeak = functionsWeak.lock()->items[0];
  REQUIRE_FALSE(functionsWeak.expired());

  functionsWeak.lock()->remove(functionWeak.lock());
  REQUIRE(functionWeak.expired());
  REQUIRE(functionsWeak.lock()->items.empty());

  world.reset();
  REQUIRE(functionsWeak.expired());
  REQUIRE(decoderWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world, interface and decoder => destroy interface", "[object-create-destroy]", INTERFACES_DECODER)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->decoders->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->decoders->length == 0);
  REQUIRE(interfaceWeak.lock()->decoders->length == 0);

  std::weak_ptr<Decoder> decoderWeak = interfaceWeak.lock()->decoders->add();
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE(decoderWeak.lock()->interface.value() == std::dynamic_pointer_cast<DecoderController>(interfaceWeak.lock()));
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->decoders->length == 1);
  REQUIRE(interfaceWeak.lock()->decoders->length == 1);

  world->interfaces->remove(interfaceWeak.lock());
  REQUIRE(interfaceWeak.expired());
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE_FALSE(decoderWeak.lock()->interface.value().operator bool());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->decoders->length == 1);

  world.reset();
  REQUIRE(decoderWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world, interface and decoder => destroy decoder", "[object-create-destroy]", INTERFACES_DECODER)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->decoders->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->decoders->length == 0);
  REQUIRE(interfaceWeak.lock()->decoders->length == 0);

  std::weak_ptr<Decoder> decoderWeak = interfaceWeak.lock()->decoders->add();
  REQUIRE_FALSE(decoderWeak.expired());
  REQUIRE(decoderWeak.lock()->interface.value() == std::dynamic_pointer_cast<DecoderController>(interfaceWeak.lock()));
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->decoders->length == 1);
  REQUIRE(interfaceWeak.lock()->decoders->length == 1);

  world->decoders->remove(decoderWeak.lock());
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(decoderWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->decoders->length == 0);
  REQUIRE(interfaceWeak.lock()->decoders->length == 0);

  world.reset();
  REQUIRE(interfaceWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and input => destroy world", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());

  std::weak_ptr<Input> inputWeak = world->inputs->add();
  REQUIRE_FALSE(inputWeak.expired());
  REQUIRE(inputWeak.lock()->getClassId() == Input::classId);

  world.reset();
  REQUIRE(inputWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and input => destroy input", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->inputs->length == 0);

  std::weak_ptr<Input> inputWeak = world->inputs->add();
  REQUIRE_FALSE(inputWeak.expired());
  REQUIRE(worldWeak.lock()->inputs->length == 1);

  world->inputs->remove(inputWeak.lock());
  REQUIRE(inputWeak.expired());
  REQUIRE(worldWeak.lock()->inputs->length == 0);

  world.reset();
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world, interface and input => destroy interface", "[object-create-destroy]", INTERFACES_INPUT)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->inputs->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->inputs->length == 0);
  REQUIRE(interfaceWeak.lock()->inputs->length == 0);

  std::weak_ptr<Input> inputWeak = interfaceWeak.lock()->inputs->add();
  REQUIRE_FALSE(inputWeak.expired());
  REQUIRE(inputWeak.lock()->interface.value() == std::dynamic_pointer_cast<InputController>(interfaceWeak.lock()));
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->inputs->length == 1);
  REQUIRE(interfaceWeak.lock()->inputs->length == 1);

  world->interfaces->remove(interfaceWeak.lock());
  REQUIRE(interfaceWeak.expired());
  REQUIRE_FALSE(inputWeak.expired());
  REQUIRE_FALSE(inputWeak.lock()->interface.value().operator bool());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->inputs->length == 1);

  world.reset();
  REQUIRE(inputWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world, interface and input => destroy input", "[object-create-destroy]", INTERFACES_INPUT)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->inputs->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->inputs->length == 0);
  REQUIRE(interfaceWeak.lock()->inputs->length == 0);

  std::weak_ptr<Input> inputWeak = interfaceWeak.lock()->inputs->add();
  REQUIRE_FALSE(inputWeak.expired());
  REQUIRE(inputWeak.lock()->interface.value() == std::dynamic_pointer_cast<InputController>(interfaceWeak.lock()));
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->inputs->length == 1);
  REQUIRE(interfaceWeak.lock()->inputs->length == 1);

  world->inputs->remove(inputWeak.lock());
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(inputWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->inputs->length == 0);
  REQUIRE(interfaceWeak.lock()->inputs->length == 0);

  world.reset();
  REQUIRE(interfaceWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and output => destroy world", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());

  std::weak_ptr<Output> outputWeak = world->outputs->add();
  REQUIRE_FALSE(outputWeak.expired());
  REQUIRE(outputWeak.lock()->getClassId() == Output::classId);

  world.reset();
  REQUIRE(outputWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEST_CASE("Create world and output => destroy output", "[object-create-destroy]")
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->outputs->length == 0);

  std::weak_ptr<Output> outputWeak = world->outputs->add();
  REQUIRE_FALSE(outputWeak.expired());
  REQUIRE(worldWeak.lock()->outputs->length == 1);

  world->outputs->remove(outputWeak.lock());
  REQUIRE(outputWeak.expired());
  REQUIRE(worldWeak.lock()->outputs->length == 0);

  world.reset();
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world, interface and output => destroy interface", "[object-create-destroy]", INTERFACES_OUTPUT)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->outputs->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->outputs->length == 0);
  REQUIRE(interfaceWeak.lock()->outputs->length == 0);

  std::weak_ptr<Output> outputWeak = interfaceWeak.lock()->outputs->add();
  REQUIRE_FALSE(outputWeak.expired());
  REQUIRE(outputWeak.lock()->interface.value() == std::dynamic_pointer_cast<OutputController>(interfaceWeak.lock()));
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->outputs->length == 1);
  REQUIRE(interfaceWeak.lock()->outputs->length == 1);

  world->interfaces->remove(interfaceWeak.lock());
  REQUIRE(interfaceWeak.expired());
  REQUIRE_FALSE(outputWeak.expired());
  REQUIRE_FALSE(outputWeak.lock()->interface.value().operator bool());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->outputs->length == 1);

  world.reset();
  REQUIRE(outputWeak.expired());
  REQUIRE(worldWeak.expired());
}

TEMPLATE_TEST_CASE("Create world, interface and output => destroy output", "[object-create-destroy]", INTERFACES_OUTPUT)
{
  auto world = World::create();
  std::weak_ptr<World> worldWeak = world;
  REQUIRE_FALSE(worldWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 0);
  REQUIRE(worldWeak.lock()->outputs->length == 0);

  std::weak_ptr<TestType> interfaceWeak = std::dynamic_pointer_cast<TestType>(world->interfaces->add(TestType::classId));
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->outputs->length == 0);
  REQUIRE(interfaceWeak.lock()->outputs->length == 0);

  std::weak_ptr<Output> outputWeak = interfaceWeak.lock()->outputs->add();
  REQUIRE_FALSE(outputWeak.expired());
  REQUIRE(outputWeak.lock()->interface.value() == std::dynamic_pointer_cast<OutputController>(interfaceWeak.lock()));
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->outputs->length == 1);
  REQUIRE(interfaceWeak.lock()->outputs->length == 1);

  world->outputs->remove(outputWeak.lock());
  REQUIRE_FALSE(interfaceWeak.expired());
  REQUIRE(outputWeak.expired());
  REQUIRE(worldWeak.lock()->interfaces->length == 1);
  REQUIRE(worldWeak.lock()->outputs->length == 0);
  REQUIRE(interfaceWeak.lock()->outputs->length == 0);

  world.reset();
  REQUIRE(interfaceWeak.expired());
  REQUIRE(worldWeak.expired());
}
