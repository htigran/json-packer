
#include <tlv_box.h>
#include <log.h>

#include "json-packer-dict.h"
#include <config.h>

/// @brief
/// @param node
tlv_box_t* dict_serialize(yajl_val node)
{
    tlv_box_t* box = tlv_box_create();

    if ((node)->type == yajl_t_object)
    {
        log_trace("is object");
        size_t nelem = node->u.object.len;
        int ii;
        for (ii = 0; ii < nelem; ++ii)
        {
            // key is just char *
            const char *key = node->u.object.keys[ii]; // key
            // values can be different types
            yajl_val val = node->u.object.values[ii]; // val
            if (YAJL_IS_DOUBLE(val))
            {
                log_trace("double: %s/%f", key, val->u.number.d);
                tlv_box_put_double(box, TLV_TYPE_DOUBLE, val->u.number.d);
            }
            else if (YAJL_IS_INTEGER(val))
            {
                log_trace("int: %s/%d", key, val->u.number.i);
                tlv_box_put_int(box, TLV_TYPE_INT, val->u.number.i);
            }
            else if (YAJL_IS_STRING(val))
            {
                log_trace("string: %s/%s", key, val->u.string);
                tlv_box_put_string(box, TLV_TYPE_STR, val->u.string);
            }
            else if (YAJL_IS_TRUE(val))
            {
                log_trace("bool: %s/TRUE", key, true);
                tlv_box_put_bool(box, TLV_TYPE_BOOL, true);
            }
            else if (YAJL_IS_FALSE(val))
            {
                log_trace("bool: %s/FALSE", key, false);
                tlv_box_put_bool(box, TLV_TYPE_BOOL, false);
            }
        }
    }
    else
    {
        log_error("Error! Unknown input format !");
        exit(1);
    }
    if (tlv_box_serialize(box) != 0)
    {
        log_error("Error! box serialize failed !");
        exit(1);
    }
    log_info("box serialize success, %d bytes ", tlv_box_get_size(box));

    return box;
}