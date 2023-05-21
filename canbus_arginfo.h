/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d8e5f37abab0474e2c5bdd98dc9a0dd39fa07e23 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_CanBus___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, interface, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CanBus_init, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, blocking, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_class_CanBus_read, 0, 0, CanFrame, MAY_BE_FALSE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CanBus_send, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, frame, CanFrame, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_CanBus_generateRandomFrame, 0, 0, CanFrame, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_CanFrame___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(CanBus, __construct);
ZEND_METHOD(CanBus, init);
ZEND_METHOD(CanBus, read);
ZEND_METHOD(CanBus, send);
ZEND_METHOD(CanBus, generateRandomFrame);
ZEND_METHOD(CanFrame, __construct);


static const zend_function_entry class_CanBus_methods[] = {
	ZEND_ME(CanBus, __construct, arginfo_class_CanBus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(CanBus, init, arginfo_class_CanBus_init, ZEND_ACC_PUBLIC)
	ZEND_ME(CanBus, read, arginfo_class_CanBus_read, ZEND_ACC_PUBLIC)
	ZEND_ME(CanBus, send, arginfo_class_CanBus_send, ZEND_ACC_PUBLIC)
	ZEND_ME(CanBus, generateRandomFrame, arginfo_class_CanBus_generateRandomFrame, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_CanFrame_methods[] = {
	ZEND_ME(CanFrame, __construct, arginfo_class_CanFrame___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_CanBus(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "CanBus", class_CanBus_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);

	return class_entry;
}

static zend_class_entry *register_class_CanFrame(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "CanFrame", class_CanFrame_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);

	return class_entry;
}
