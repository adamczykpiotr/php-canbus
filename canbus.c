/* canbus extension for PHP */

#ifdef HAVE_CONFIG_H
    # include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_canbus.h"
#include "canbus_arginfo.h"

#include <linux/can/raw.h>  // can_frame, masks
#include <net/if.h>         // ifreq
#include <sys/ioctl.h>      // ioctl, SIOCGIFINDEX
#include <unistd.h>         // read, close
#include <fcntl.h>          // fcntl

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSCE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

// Defines for easy class member access
#define CANBUS_INTERFACE_P OBJ_PROP_NUM(Z_OBJ_P(ZEND_THIS), 0)
#define CANBUS_SOCKFD_P OBJ_PROP_NUM(Z_OBJ_P(ZEND_THIS), 1)

#define CANFRAME_ID_P(zv) OBJ_PROP_NUM(Z_OBJ_P(zv), 0)
#define CANFRAME_LENGTH_P(zv) OBJ_PROP_NUM(Z_OBJ_P(zv), 1) 
#define CANFRAME_DATA_P(zv) OBJ_PROP_NUM(Z_OBJ_P(zv), 2 )

// Class entries
zend_class_entry *canBus_ce = NULL;
zend_class_entry *canFrame_ce = NULL;

/* {{{ CanBus constructor */
PHP_METHOD(CanBus, __construct) {
    zend_string* interface;

    // Parse arguments
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(interface)
    ZEND_PARSE_PARAMETERS_END();

    // Validate interface name
    if (ZSTR_LEN(interface) == 0) {
        php_error_docref(NULL, E_ERROR, "interface name cannot be empty");
    }

    // Initialize $this->interface with provided name
    ZVAL_STR_COPY(CANBUS_INTERFACE_P, interface);
}
/* }}} */

/* {{{ CanBus init method */
PHP_METHOD(CanBus, init) {
    zend_bool blocking = true;

    // Parse arguments
    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(blocking)
    ZEND_PARSE_PARAMETERS_END();

    // If socket file descriptor is set, close it before another initialization
    zend_long sockFd = Z_LVAL_P(CANBUS_SOCKFD_P);
    if(sockFd != -1) {
        close(sockFd);
    }

    // Open CAN network interface
    sockFd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockFd == -1) {
        RETURN_FALSE;
    }

    // Map CAN interface
    struct ifreq ifr;
    zval* interface = CANBUS_INTERFACE_P;
    strncpy(ifr.ifr_name, Z_STRVAL(*interface), IFNAMSIZ);
    if(ioctl(sockFd, SIOCGIFINDEX, &ifr) == -1) {
        RETURN_FALSE;
    }

    // Bind the socket to the network interface
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if(bind(sockFd, (struct sockaddr*)(&addr), sizeof(addr)) == -1) {
        RETURN_FALSE;
    }

    // Conditionaly set socket to non-blocking mode 
    if(!blocking) {

        // Get flags
        int flags = fcntl(sockFd, F_GETFL, 0);
        if(flags == -1) {
            RETURN_FALSE;
        }

        // Set non-blocking flag
        if (fcntl(sockFd, F_SETFL, flags | O_NONBLOCK) == -1) {
            RETURN_FALSE;
        }
    }

    // Update $this->sockFd file descriptor
    ZVAL_LONG(CANBUS_SOCKFD_P, sockFd);

    RETURN_TRUE;
}
/* }}} */

/* {{{ CanBus read single frame method */
PHP_METHOD(CanBus, read) {
    struct can_frame canFrame;

    // This method doesn't need any 
    ZEND_PARSE_PARAMETERS_NONE();
    
    // Get socket file descriptor
    zend_long sockFd = Z_LVAL_P(CANBUS_SOCKFD_P);

    // Try to read CAN frame
    ssize_t numBytes = read(sockFd, &canFrame, CAN_MTU);

    // Return false if whole frame could not be read
    if(numBytes != CAN_MTU) {
        RETURN_FALSE;
    }

    // Create CanFrame object
    zend_object *object = zend_objects_new(canFrame_ce);
    zval frame;
    ZVAL_OBJ(&frame, object);

    // Initialize $frame->id
    ZVAL_LONG(CANFRAME_ID_P(&frame), canFrame.can_id);
    
    // Initialize $frame->length
    ZVAL_LONG(CANFRAME_LENGTH_P(&frame), canFrame.can_dlc);

    // Initialize $frame->data
    zval* data = CANFRAME_DATA_P(&frame);
    array_init_size(data, canFrame.can_dlc);

    // fill $frame->data
    for(int i = 0; i < canFrame.can_dlc; i++) {
        add_next_index_long(data, canFrame.data[i]);
    }

    // Return CanFrame
    RETURN_ZVAL(&frame, 0, 0);
}
/* }}} */


/* {{{ CanBus send single frame method */
PHP_METHOD(CanBus, send) {
    zval* object;

    // Parse arguments
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(object, canFrame_ce)
    ZEND_PARSE_PARAMETERS_END();

    // Get HashTable
    HashTable* canFrameData = Z_ARRVAL_P(CANFRAME_DATA_P(object));
 
    // Create can_frame struct
    struct can_frame canFrame;

    // Assign id & data length
    canFrame.can_id = Z_LVAL_P(CANFRAME_ID_P(object));
    canFrame.can_dlc = zend_hash_num_elements(canFrameData);

    // Assign data array
    zval* temp;
    zend_long i = 0;
    ZEND_HASH_FOREACH_VAL(canFrameData, temp) {
        canFrame.data[i++] = Z_LVAL_P(temp);
    } ZEND_HASH_FOREACH_END();

    // Get socket file descriptior
    zend_long sockFd = Z_LVAL_P(CANBUS_SOCKFD_P);
   
    // Write data to socket
    zend_long bytesWritten = write(sockFd, &canFrame, sizeof(canFrame));

    // Return success or failure
    RETURN_BOOL(bytesWritten == sizeof(canFrame));
}
/* }}} */

/* {{{ CanBus generate random frame */
PHP_METHOD(CanBus, generateRandomFrame) {
    zend_object *object = zend_objects_new(canFrame_ce);

    // This method doesn't need any params
    ZEND_PARSE_PARAMETERS_NONE();

    zval frame;
    ZVAL_OBJ(&frame, object);

    // Generate random can_frame;
    struct can_frame canFrame;
    srand(time(NULL));
    canFrame.can_id = rand() % 0x7FF;
    canFrame.can_dlc = rand() % 8;
    for(int i = 0; i < canFrame.can_dlc; i++) {
        canFrame.data[i] = rand() % 0xFF;
    }

    // Initialize $frame->id
    ZVAL_LONG(CANFRAME_ID_P(&frame), canFrame.can_id);
    
    // Initialize $frame->length
    ZVAL_LONG(CANFRAME_LENGTH_P(&frame), canFrame.can_dlc);

    // Initialize $frame->data
    zval* data = CANFRAME_DATA_P(&frame);
    array_init_size(data, canFrame.can_dlc);

    // Fill $frame->data
    for(int i = 0; i < canFrame.can_dlc; i++) {
        add_next_index_long(data, canFrame.data[i]);
    }

    // Return CanFrame
    RETURN_ZVAL(&frame, 0, 0);
}
/* }}} */

/* {{{ CanFrame constructor */
PHP_METHOD(CanFrame, __construct) {
    zend_long id;
    HashTable* data;

    // Parse arguments
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(id)
        Z_PARAM_ARRAY_HT(data)
    ZEND_PARSE_PARAMETERS_END();

    // Validate provided ID with CAN 2.0B standard (0 to 2^31 - 1)
    if(id < 0 || id > 0x7FFFFFFF) {
        php_error_docref(NULL, E_ERROR, "parameter ID has to be in range of 0-0x7FFFFFFF");
    }

    // Validate provided data has at most 8 elements
    zend_long dataLength = zend_hash_num_elements(data);
    if(dataLength > 8) {
        php_error_docref(NULL, E_ERROR, "data length has to be not greater than 8 elements");
    }

    // Validate provided data constists only of zend_long in range of 0-255
    zval* temp;
    ZEND_HASH_FOREACH_VAL(data, temp) {
        // Validate type
        if(Z_TYPE_P(temp) != IS_LONG) {
            php_error_docref(NULL, E_ERROR, "data element has to be an int");
            continue;
        } 

        // Validate value
        zend_long value = Z_LVAL_P(temp);
        if(value < 0 || value > 0xFF) {
            php_error_docref(NULL, E_ERROR, "data element value has to be in range od 0-0xFF");
        }
    } ZEND_HASH_FOREACH_END();

    // Initialize $this->id with provided id
    ZVAL_LONG(CANFRAME_ID_P(ZEND_THIS), id);

    // Make copy of passed input data & initialize $this->data
    ZVAL_ARR(CANFRAME_DATA_P(ZEND_THIS), data);

    // Initialize size of passed array
    ZVAL_LONG(CANFRAME_LENGTH_P(ZEND_THIS), dataLength);
}
/* }}} */

/* {{{ CanBus class registry */
static zend_class_entry *register_class_CanBus_with_members(void) {
    zend_class_entry *classEntry = register_class_CanBus();

    // Register member: public string $id = undefined
    zval property_interface_default_value;
    ZVAL_UNDEF(&property_interface_default_value);
    zend_string *property_interface_name = zend_string_init("interface", strlen("interface"), true);
    zend_declare_typed_property(classEntry, property_interface_name, &property_interface_default_value, ZEND_ACC_PROTECTED, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_STRING));
    zend_string_release(property_interface_name);

    // Register member: protected int $socketFd = -1
    zval property_socketFd_default_value;
    ZVAL_LONG(&property_socketFd_default_value, -1);
    zend_string *property_socketFd_name = zend_string_init("socketFd", strlen("socketFd"), true);

    zend_declare_typed_property(classEntry, property_socketFd_name, &property_socketFd_default_value, ZEND_ACC_PROTECTED, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
    zend_string_release(property_socketFd_name);

    return classEntry;
}
/* }}} */

/* {{{ CanFrame class registry */
static zend_class_entry *register_class_CanFrame_with_members(void) {
    zend_class_entry *classEntry = register_class_CanFrame();

    // Register member: public int $id = 0
    zval id;
    ZVAL_LONG(&id, 0);
    zend_string *idName = zend_string_init("id", strlen("id"), true);
    zend_declare_typed_property(classEntry, idName, &id, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
    zend_string_release(idName);

    // Register member: public int $length = 0
    zval length;
    ZVAL_LONG(&length, 0);
    zend_string *lengthName = zend_string_init("length", strlen("length"), true);
    zend_declare_typed_property(classEntry, lengthName, &length, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
    zend_string_release(lengthName);

    // Register member: public array $data = []
    zval data;
    ZVAL_EMPTY_ARRAY(&data);
    zend_string *dataName = zend_string_init("data", strlen("data"), true);
    zend_declare_typed_property(classEntry, dataName, &data, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
    zend_string_release(dataName);
    
    return classEntry;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(canbus){
    #if defined(ZTS) && defined(COMPILE_DL_CANBUS)
        ZEND_TSRMLS_CACHE_UPDATE();
    #endif

    return SUCCESS;
}
/* }}} */

 /*{{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(canbus) {
    canBus_ce = register_class_CanBus_with_members();
    canFrame_ce = register_class_CanFrame_with_members();

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(canbus) {
    php_info_print_table_start();
    php_info_print_table_header(2, "canbus support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ canbus_module_entry */
zend_module_entry canbus_module_entry = {
    STANDARD_MODULE_HEADER,
    "canbus",           /* Extension name */
    NULL,               /* zend_function_entry */
    PHP_MINIT(canbus),  /* PHP_MINIT - Module initialization */
    NULL,               /* PHP_MSHUTDOWN - Module shutdown */
    PHP_RINIT(canbus),  /* PHP_RINIT - Request initialization */
    NULL,               /* PHP_RSHUTDOWN - Request shutdown */
    PHP_MINFO(canbus),  /* PHP_MINFO - Module info */
    PHP_CANBUS_VERSION, /* Version */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CANBUS
    # ifdef ZTS
        ZEND_TSRMLS_CACHE_DEFINE()
    # endif

    ZEND_GET_MODULE(canbus)
#endif
