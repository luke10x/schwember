FROM emscripten/emsdk:3.1.25

# Install from sources GLM and library
# (These are header-only libraries, no need to compile)
WORKDIR /opt
RUN git clone --depth 1 -b 0.9.9.8 https://github.com/g-truc/glm.git
RUN git clone --depth 1 -b v1.13   https://github.com/jkuhlmann/cgltf.git
RUN git clone --depth 1 -b master  https://github.com/nothings/stb.git

# Install Bullet Physics
RUN git clone --depth 1 -b 3.25 https://github.com/bulletphysics/bullet3.git \
  && mkdir /opt/bullet3/build
WORKDIR /opt/bullet3/build
RUN emcmake cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_CPU_DEMOS=OFF \
  -DBUILD_EXTRAS=OFF \
  -DBUILD_BULLET2_DEMOS=OFF \
  -DBUILD_UNIT_TESTS=OFF \
  .. \
  && emmake make \
  && emmake make install
WORKDIR /opt

