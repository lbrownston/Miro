set( TESTS_BIN_DIR /bin )

#add_subdirectory( log )
if ( TAO_FOUND )
  add_subdirectory( bidir  )
  add_subdirectory( client )
  add_subdirectory( notify )
  add_subdirectory( server )
  add_subdirectory( singleton )
endif ( TAO_FOUND )
