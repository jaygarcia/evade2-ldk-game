#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#

LIBXMP_CORE_PLAYER = true
LIBXMP_CORE_DISABLE_IT = true
CFLAGS := LIBXMP_CORE_PLAYER LIBXMP_CORE_DISABLE_IT
COMPONENT_ADD_INCLUDEDIRS :=  ../libxmp/ ../libxmp/loaders/ .
COMPONENT_SRCDIRS :=  ../libxmp/ ../libxmp/loaders/ .
