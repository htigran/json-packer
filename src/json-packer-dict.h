
#ifndef JSON_PACKER_DICT_H
#define JSON_PACKER_DICT_H

#include <yajl/yajl_tree.h>

tlv_box_t* box_serialize_values(yajl_val node);
tlv_box_t* box_serialize_keys(yajl_val node);

#endif // JSON_PACKER_DICT_H
