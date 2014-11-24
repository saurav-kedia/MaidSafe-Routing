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

#ifndef MAIDSAFE_ROUTING_HEADER_H_
#define MAIDSAFE_ROUTING_HEADER_H_

#include "maidsafe/common/config.h"

#include "maidsafe/routing/types.h"
#include "maidsafe/routing/utils.h"

namespace maidsafe {

namespace routing {

struct header {
  header() = default;
  header(const header&) = delete;
  header(header&& other) MAIDSAFE_NOEXCEPT : destination(std::move(other.destination)),
                                             source(std::move(other.source)),
                                             message_id(std::move(other.message_id)),
                                             checksum(std::move(other.checksum)),
                                             other_checksums(std::move(other.checksums)) {}
  header(Destination destination_in, Source source_in, message_id message_id_in,
               murmur_hash checksum_in, checksums other_checksums_in)
      : destination(std::move(destination_in)),
        source(std::move(source_in)),
        message_id(std::move(message_id_in)),
        checksum(std::move(checksum_in)),
        other_checksums((std::move(other_checksums_in))
  {}
  ~header() = default;
  header& operator=(const header&) = delete;
  header& operator=(header&& other) MAIDSAFE_NOEXCEPT {
    destination = std::move(other.destination);
    source = std::move(other.source);
    message_id = std::move(other.message_id);
    checksum = std::move(other.checksum);
    return *this;
  }

  template <typename Archive>
  void serialize(Archive& archive) {
    archive(destination, source, message_id, checksum);
  }

  NodeId destination[];
  NodeId source[];
  message_id message_id[];
  murmur_hash checksum[];
  checksums other_checksums[];
};

}  // namespace routing

}  // namespace maidsafe

#endif  // MAIDSAFE_ROUTING_HEADER_H_
