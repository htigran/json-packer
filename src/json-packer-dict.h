
#ifndef JSON_PACKER_DICT_H
#define JSON_PACKER_DICT_H

// TODO see if we can get rid of this include
#include <yajl/yajl_tree.h>


/// @brief parses a yajl tree root node and generates a serialized tlv box
///        out of json values
/// @param node - yajl (json) node
/// @return serialized tlv_box that can be stored in a file
tlv_box_t* box_serialize_values(yajl_val node);

/// @brief parses a yajl tree root node and generates a serialized tlv box
///        out of json keys
/// @param node - yajl (json) node
/// @return serialized tlv_box that can be stored in a file
tlv_box_t* box_serialize_keys(yajl_val node);

#endif // JSON_PACKER_DICT_H
