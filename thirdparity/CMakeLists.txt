execute_process ( COMMAND make lib WORKING_DIRECTORY ${LIBLINEAR_DIR} )
execute_process ( COMMAND ln -s liblinear.so.3 liblinear.so WORKING_DIRECTORY ${LIBLINEAR_DIR} )
