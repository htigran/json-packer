
#ifndef JSON_PACKER_DICT_H
#define JSON_PACKER_DICT_H

#include <yajl/yajl_tree.h>


typedef struct dict_item_t_struct
{
    int key;
    void* value;
    int type;
} dict_item_t;

void dict_add_item();
void dict_reset();
tlv_box_t* dict_serialize(yajl_val node);

#endif // JSON_PACKER_DICT_H
