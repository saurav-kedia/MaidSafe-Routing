/*  Copyright 2014 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.maidsafe.net/licenses

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#include "maidsafe/routing/message_handler.h"

#include <vector>

#include "maidsafe/common/serialisation/binary_archive.h"
#include "maidsafe/common/serialisation/compile_time_mapper.h"

//#include "maidsafe/routing/cacheable_get.h"
//#include "maidsafe/routing/cacheable_get_response.h"
#include "maidsafe/routing/connect.h"
#include "maidsafe/routing/connection_manager.h"
//#include "maidsafe/routing/find_group.h"
//#include "maidsafe/routing/find_group_response.h"
//#include "maidsafe/routing/forward_connect.h"
#include "maidsafe/routing/ping.h"
#include "maidsafe/routing/ping_response.h"
#include "maidsafe/routing/types.h"
//#include "maidsafe/routing/vault_message.h"

namespace maidsafe {

namespace routing {

MessageHandler::MessageHandler(AsioService& asio_service,
                               rudp::ManagedConnections& managed_connections,
                               ConnectionManager& connection_manager)
    : asio_service_(asio_service),
      rudp_(managed_connections),
      connection_manager_(connection_manager) {}

void MessageHandler::OnMessageReceived(rudp::ReceivedMessage&& serialised_message) {
  InputVectorStream binary_stream{std::move(serialised_message)};

  auto message(Parse<TypeFromMessage>(serialised_message) > (serialised_message));
  // FIXME (dirvine) Check firewall 19/11/2014
  HandleMessage(message);
  // FIXME (dirvine) add to firewall 19/11/2014
}

void MessageHandler::HandleMessage(const Ping& ping_msg) {
  // ping is a single destination message
  if (ping_msg.header.destination.data() == connection_mgr_.OurId()) {
    auto targets(connection_mgr_.get_target(ping_msg.header.source.data()));
    for (const auto& target : targets)
      rudp_.Send(target.id, Serialise(ping_response(ping_msg)));
  } else {  // scatter
    auto targets(connection_mgr_.get_target(ping_msg.header.destination.data()));
    for (const auto& target : targets)
      rudp_.Send(target.id, Serialise(ping_response(ping_msg)));
    else rudp_.Send(target.id, Serialise(ping_msg));
  }
}

void MessageHandler::HandleMessage(const ping_response& ping_response_msg) {
  // TODO(dirvine): 2014-11-19 FIXME set a future or some async return type in node or
  // client
}


void MessageHandler::HandleMessage(const connect& connect_msg) {
  if (connect_msg.header.destination.data() == connection_mgr_.OurId()) {
    if (connection_mgr_.suggest_node(connect_msg.header.source)) {
      rudp_.GetNextAvailableEndpoint(
          connect_msg.header.source,
          [this](maidsafe_error error, rudp::endpoint_pair endpoint_pair) {
            if (!error)
              auto targets(connection_mgr_.get_target(connect_msg.header.source));
            for (const auto& target : targets)
              // FIXME (dirvine) Check connect parameters and why no response type 19/11/2014
              rudp_.Send(target.id, Serialise(forward_connect( ));
          });
    }
  } else {
    auto targets(connection_mgr_.get_target(connect_msg.header.destination.data()));
    for (const auto& target : targets)
      rudp_.Send(target.id, Serialise(connect(connect_msg)));
  }
}

}  // namespace routing

}  // namespace maidsafe
