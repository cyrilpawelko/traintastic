#include "messages.hpp"
#include <cassert>
#include "../../../utils/tohex.hpp"

namespace TraintasticDIY {

Checksum calcChecksum(const Message& message)
{
  const uint8_t* p = reinterpret_cast<const uint8_t*>(&message);
  const size_t dataSize = message.dataSize();
  uint8_t checksum = p[0];
  for(size_t i = 1; i <= dataSize; i++)
    checksum ^= p[i];
  return static_cast<Checksum>(checksum);
}

void updateChecksum(Message& message)
{
  *(reinterpret_cast<Checksum*>(&message) + message.dataSize() + 1) = calcChecksum(message);
}

bool isChecksumValid(const Message& message)
{
  return calcChecksum(message) == *(reinterpret_cast<const Checksum*>(&message) + message.dataSize() + 1);
}

std::string toString(const Message& message)
{
  std::string s{::toString(message.opCode)};

  switch(message.opCode)
  {
    case OpCode::Heartbeat:
    case OpCode::GetInfo:
    case OpCode::GetFeatures:
      assert(message.dataSize() == 0);
      break;

    case OpCode::GetInputState:
    {
      const auto& getInputState = static_cast<const GetInputState&>(message);
      s.append(" address=").append(std::to_string(getInputState.address()));
      break;
    }
    case OpCode::SetInputState:
    {
      const auto& setInputState = static_cast<const SetInputState&>(message);
      s.append(" address=").append(std::to_string(setInputState.address()));
      s.append(" state=").append(::toString(setInputState.state));
      break;
    }
    case OpCode::GetOutputState:
    {
      const auto& getOutputState = static_cast<const GetOutputState&>(message);
      s.append(" address=").append(std::to_string(getOutputState.address()));
      break;
    }
    case OpCode::SetOutputState:
    {
      const auto& setOutputState = static_cast<const SetOutputState&>(message);
      s.append(" address=").append(std::to_string(setOutputState.address()));
      s.append(" state=").append(::toString(setOutputState.state));
      break;
    }
    case OpCode::ThrottleUnsubscribe:
    {
      const auto& throttleUnsubscribe = static_cast<const ThrottleUnsubscribe&>(message);
      s.append(" throttle=").append(std::to_string(throttleUnsubscribe.throttleId()));
      s.append(" address=").append(std::to_string(throttleUnsubscribe.address()));
      break;
    }
    case OpCode::ThrottleSetFunction:
    {
      const auto& throttleSetFunction = static_cast<const ThrottleSetFunction&>(message);
      s.append(" throttle=").append(std::to_string(throttleSetFunction.throttleId()));
      s.append(" address=").append(std::to_string(throttleSetFunction.address()));
      s.append(" function=").append(std::to_string(throttleSetFunction.functionNumber()));
      s.append(" value=").append(throttleSetFunction.functionValue() ? "on" : "off");
      break;
    }
    case OpCode::ThrottleSetSpeedDirection:
    {
      const auto& throttleSetSpeedDirection = static_cast<const ThrottleSetSpeedDirection&>(message);
      s.append(" throttle=").append(std::to_string(throttleSetSpeedDirection.throttleId()));
      s.append(" address=").append(std::to_string(throttleSetSpeedDirection.address()));
      if(throttleSetSpeedDirection.isSpeedSet())
      {
        if(!throttleSetSpeedDirection.isEmergencyStop())
        {
          s.append(" speed=").append(std::to_string(throttleSetSpeedDirection.speed));
          s.append(" speed_max=").append(std::to_string(throttleSetSpeedDirection.speedMax));
        }
        else
          s.append(" estop");
      }
      if(throttleSetSpeedDirection.isDirectionSet())
        s.append(" direction=").append((throttleSetSpeedDirection.direction() == Direction::Forward) ? "fwd" : "rev");
      break;
    }
    case OpCode::Features:
    {
      break;
    }
    case OpCode::Info:
    {
      break;
    }
  }

  s.append(" [");
  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&message);
  for(size_t i = 0; i < message.size(); i++)
  {
    if(i != 0)
      s.append(" ");
    s.append(toHex(bytes[i]));
  }
  s.append("]");

  return s;
}

}
