#if make aiq with raw stream lib, modify -DUSE_RAWSTREAM_LIB value to ON, default value OFF 
cmake \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DRKAIQ_TARGET_SOC=rk3588 \
    -DARCH=aarch64 \
    -DCMAKE_C_COMPILER="/usr/bin/aarch64-linux-gnu-gcc" \
    -DCMAKE_CXX_COMPILER="/usr/bin/aarch64-linux-gnu-g++" \
    -DRKAIQ_BUILD_BINARY_IQ=ON \
    -DCMAKE_SKIP_RPATH=TRUE \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES \
    -DISP_HW_VERSION=-DISP_HW_V30 \
    -DCMAKE_INSTALL_PREFIX="installed" \
    -DRKAIQ_USE_RAWSTREAM_LIB=OFF \
    -DRKAIQ_HAVE_FAKECAM=ON \
    -DRKAIQ_ENABLE_AF=ON \
    . \
&& make -j$(nproc) \
&& make install

status_code=$?

popd

exit $status_code