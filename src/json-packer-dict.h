
#ifndef JSON_PACKER_DICT_H
#define JSON_PACKER_DICT_H

#include <yajl/yajl_tree.h>

tlv_box_t* dict_serialize(yajl_val node);

#endif // JSON_PACKER_DICT_H
