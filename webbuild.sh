printf "INFO: Step no.1: Setting up Emscritpen.\n\n"
cd lib/emsdk
# ./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

printf "\nINFO: Step no.2: Building the project for Webassembly.\n\n"
cd ../..
mkdir bin_web/
cd bin_web/
emcmake cmake .. -DPLATFORM=Web
emmake make

printf "\nINFO: Step no.3: Setting up the files for compression.\n"
mkdir colorful_pixels_build
cp colorful_pixels.js ./colorful_pixels_build
cp colorful_pixels.html ./colorful_pixels_build
cp colorful_pixels.wasm ./colorful_pixels_build
cp colorful_pixels.data ./colorful_pixels_build
mv ./colorful_pixels_build/colorful_pixels.html ./colorful_pixels_build/index.html 

printf "\nINFO: Step no.4: Compressing the final build.\n"
rm colorful_pixels_build.zip
zip -r -j colorful_pixels_build.zip colorful_pixels_build
rm -rf colorful_pixels_build
