/* canbus extension for PHP */

#ifndef PHP_CANBUS_H
# define PHP_CANBUS_H

extern zend_module_entry canbus_module_entry;
# define phpext_canbus_ptr &canbus_module_entry

# define PHP_CANBUS_VERSION "1.0.0"

# if defined(ZTS) && defined(COMPILE_DL_CANBUS)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

PHP_MINIT_FUNCTION(canbus);
PHP_MINFO_FUNCTION(canbus);

#endif	/* PHP_CANBUS_H */
