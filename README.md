How to build Module for Platform
- It is only for modules are needed to using Android build system.
- Please check its own install information under its folder for other module.

[Step to build]
1. Get android open source.
: version info - Android jellybean 4.2.2_r1

( Download site : http://source.android.com )

2. Copy module that you want to build - to original android open source
If same module exist in android open source, you should replace it. (no 
overwrite)


# It is possible to build all modules at once.


3. You should add module name to 'PRODUCT_PACKAGES' in 'build\target\product\
core.mk' as following case.

case 1) e2fsprog : should add 'e2fsck' to PRODUCT_PACKAGES
case 2) libexifa : should add 'libexifa' to PRODUCT_PACKAGES
case 3) libjpega : should add 'libjpega' to PRODUCT_PACKAGES
case 4) KeyUtils : should add 'libkeyutils' to PRODUCT_PACKAGES
case 5) libasound : should add 'libasound' to PRODUCT_PACKAGES
case 6) audio\alsaplugin : should add 'libasound_module_pcm_bcmfilter' to PRODUCT_PACKAGES

ex.) [build\target\product\core.mk] - add all module name for all follow cases at once


PRODUCT_PACKAGES += \
e2fsck \
libexifa \
libjpega \
libkeyutils \
libasound \
libasound_module_pcm_bcmfilter

4. In case of 'bluetooth', you should add following text in 'build\target\
board\generic\BoardConfig.mk'


OPENSOURCE_ALSA_AUDIO := true
BOARD_USES_ALSA_AUDIO := true
BRCM_ALSA_LIB_DIR=device/samsung/bcm_common/alsa-lib

5. excute build command
./build.sh user




