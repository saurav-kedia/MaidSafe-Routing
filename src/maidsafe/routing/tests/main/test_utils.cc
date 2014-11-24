/*  Copyright 2012 MaidSafe.net limited

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

#include "maidsafe/routing/tests/main/test_utils.h"

#include <vector>
#include "maidsafe/common/node_id.h"
#include "maidsafe/common/utils.h"
#include "maidsafe/common/make_unique.h"

#include "maidsafe/routing/routing_table.h"
#include "maidsafe/routing/types.h"

namespace maidsafe {

namespace routing {

namespace test {

// ScopedBootstrapFile::ScopedBootstrapFile(const BootstrapContacts& bootstrap_contacts)
//     : kFilePath(detail::GetOverrideBootstrapFilePath<false>()) {
//   boost::filesystem::remove(kFilePath);
//   WriteBootstrapContacts(bootstrap_contacts, kFilePath);
// }
//
// ScopedBootstrapFile::~ScopedBootstrapFile() {
//   EXPECT_NO_THROW(boost::filesystem::remove(kFilePath));
// }
//
// node_info MakeNode() {
//   node_info node;
//   node.id = NodeId(RandomString(NodeId::kSize));
//   asymm::Keys keys(asymm::GenerateKeyPair());
//   node.public_key = keys.public_key;
//   node.connection_id = node.id;
//   return node;
// }
//
// node_infoAndPrivateKey Makenode_infoAndKeys() {
//   passport::Pmid pmid(passport::CreatePmidAndSigner().first);
//   return Makenode_infoAndKeysWithFob(pmid);
// }
//
// node_infoAndPrivateKey Makenode_infoAndKeysWithPmid(passport::Pmid pmid) {
//   return Makenode_infoAndKeysWithFob(pmid);
// }
//
// node_infoAndPrivateKey Makenode_infoAndKeysWithMaid(passport::Maid maid) {
//   return Makenode_infoAndKeysWithFob(maid);
// }
//
// NodeId GenerateUniqueRandomId(const NodeId& holder, unsigned int pos) {
//   std::string holder_id = holder.ToStringEncoded(NodeId::EncodingType::kBinary);
//   std::bitset<64 * 8> holder_id_binary_bitset(holder_id);
//   NodeId new_node;
//   std::string new_node_string;
//   // generate a random ID and make sure it has not been generated previously
//   while (new_node_string == "" || new_node_string == holder_id) {
//     new_node = NodeId(RandomString(NodeId::kSize));
//     std::string new_id = new_node.ToStringEncoded(NodeId::EncodingType::kBinary);
//     std::bitset<64 * 8> binary_bitset(new_id);
//     for (unsigned int i(0); i < pos; ++i)
//       holder_id_binary_bitset[i] = binary_bitset[i];
//     new_node_string = holder_id_binary_bitset.to_string();
//     if (pos == 0)
//       break;
//   }
//   new_node = NodeId(new_node_string, NodeId::EncodingType::kBinary);
//   return new_node;
// }
//
// NodeId GenerateUniqueNonRandomId(const NodeId& holder, uint64_t id) {
//   std::string holder_id = holder.ToStringEncoded(NodeId::EncodingType::kBinary);
//   std::bitset<64 * 8> holder_id_binary_bitset(holder_id);
//   NodeId new_node;
//   std::string new_node_string;
//   // generate a random ID and make sure it has not been generated previously
//   new_node = NodeId(RandomString(NodeId::kSize));
//   std::string new_id = new_node.ToStringEncoded(NodeId::EncodingType::kBinary);
//   std::bitset<64> binary_bitset(id);
//   for (unsigned int i(0); i < 64; ++i)
//     holder_id_binary_bitset[i] = binary_bitset[i];
//   new_node_string = holder_id_binary_bitset.to_string();
//   new_node = NodeId(new_node_string, NodeId::EncodingType::kBinary);
//   return new_node;
// }
//
// NodeId GenerateUniqueRandomNodeId(const std::vector<NodeId>& esisting_ids) {
//   NodeId new_node(RandomString(NodeId::kSize));
//   while (std::find_if(esisting_ids.begin(), esisting_ids.end(), [&new_node](const NodeId&
//   element) {
//            return element == new_node;
//          }) != esisting_ids.end()) {
//     new_node = NodeId(RandomString(NodeId::kSize));
//   }
//   return new_node;
// }
//
// NodeId GenerateUniqueRandomId(unsigned int pos) { return GenerateUniqueRandomId(NodeId(), pos); }
//
// NodeId GenerateUniqueNonRandomId(uint64_t pos) { return GenerateUniqueNonRandomId(NodeId(), pos);
// }
//
// int NetworkStatus(int status) { return (status > 0) ? (status * 100 / kRoutingTableSize) :
// status; }
//
// void SortFromTarget(const NodeId& target, std::vector<node_info>& nodes) {
//   std::sort(nodes.begin(), nodes.end(), [target](const node_info& lhs, const node_info& rhs) {
//     return NodeId::CloserToTarget(lhs.id, rhs.id, target);
//   });
// }
//
// void PartialSortFromTarget(const NodeId& target, std::vector<node_info>& nodes,
//                            size_t num_to_sort) {
//   assert(num_to_sort <= nodes.size());
//   std::partial_sort(nodes.begin(), nodes.begin() + num_to_sort, nodes.end(),
//                     [target](const node_info& lhs, const node_info& rhs) {
//     return NodeId::CloserToTarget(lhs.id, rhs.id, target);
//   });
// }
//
// void SortIdsFromTarget(const NodeId& target, std::vector<NodeId>& nodes) {
//   std::sort(nodes.begin(), nodes.end(), [target](const NodeId& lhs, const NodeId& rhs) {
//     return NodeId::CloserToTarget(lhs, rhs, target);
//   });
// }
//
// void Sortnode_infosFromTarget(const NodeId& target, std::vector<node_info>& nodes) {
//   std::sort(nodes.begin(), nodes.end(), [target](const node_info& lhs, const node_info& rhs) {
//     return NodeId::CloserToTarget(lhs.id, rhs.id, target);
//   });
// }
//
// bool CompareListOfnode_infos(const std::vector<node_info>& lhs, const std::vector<node_info>&
// rhs) {
//   if (lhs.size() != rhs.size())
//     return false;
//   for (const auto& node_info : lhs) {
//     if (std::find_if(rhs.begin(), rhs.end(),
//                      [&](const node_info& node) { return node.id == node_info.id; }) ==
//                      rhs.end())
//       return false;
//   }
//
//   for (const auto& node_info : rhs) {
//     if (std::find_if(lhs.begin(), lhs.end(),
//                      [&](const node_info& node) { return node.id == node_info.id; }) ==
//                      lhs.end())
//       return false;
//   }
//   return true;
// }
//

std::vector<std::unique_ptr<routing_table>> routing_table_network(size_t size) {
  asymm::Keys keys(asymm::GenerateKeyPair());
  std::vector<std::unique_ptr<routing_table>> routing_tables;
  routing_tables.reserve(size);
  for (size_t i = 0; i < size; ++i) {
    routing_tables.emplace_back(
        maidsafe::make_unique<routing_table>(NodeId(RandomString(NodeId::kSize))));
  }
  return routing_tables;
}

}  // namespace test

}  // namespace routing

}  // namespace maidsafe
