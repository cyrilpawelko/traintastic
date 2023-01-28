/**
 * server/src/hardware/protocol/marklincan/iohandler/udpiohandler.hpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2023 Reinder Feenstra
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

#ifndef TRAINTASTIC_SERVER_HARDWARE_PROTOCOL_MARKLINCAN_IOHANDLER_UDPIOHANDLER_HPP
#define TRAINTASTIC_SERVER_HARDWARE_PROTOCOL_MARKLINCAN_IOHANDLER_UDPIOHANDLER_HPP

#include "networkiohandler.hpp"
#include <boost/asio/ip/udp.hpp>

namespace MarklinCAN {

class UDPIOHandler final : public NetworkIOHandler
{
  private:
    static constexpr uint16_t localPort = 15730;
    static constexpr uint16_t remotePort = 15731;

    boost::asio::ip::udp::socket m_readSocket;
    boost::asio::ip::udp::endpoint m_readEndpoint;
    std::array<std::byte, 1500> m_readBuffer;
    boost::asio::ip::udp::socket m_writeSocket;
    boost::asio::ip::udp::endpoint m_writeEndpoint;

    void read() final;
    void write() final;

  public:
    UDPIOHandler(Kernel& kernel, const std::string& hostname);

    void stop() final;
};

}

#endif
