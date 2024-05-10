cd lib/emsdk
./emsdk activate latest
source ./emsdk_env.sh
cd ../../bin_web/
emcmake cmake .. -DPLATFORM=Web
emmake make
emrun colorful_pixels.html