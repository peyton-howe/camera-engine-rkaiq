# camera-engine-rkaiq

Build files will be created in `build/linux/output/aarch64`

**To build:**
1. Enter `build/linux/`
2. Run `envsetup.sh`
3. Choose the rockchip SoC by entering `rk3588` when prompted
4. Run `make-Makefiles-aarch64.bash`


**To install:**\
copy `build/linux/output/aarch64/rkaiq_3A_server/rkaiq_3A_server` to `/bin`\
copy `build/linux/output/aarch64/rkaiq/all_lib/RelWithDebInfo/librkaiq.so` to `/lib`

**NOTE:**\
Unzip the following two files before compiling.\
`common/gen_mesh/android/genMesh_static_64bit/libgenMeshLib.zip`\
`common/gen_mesh/android/genMesh_static_32bit/libgenMeshLib.zip`
