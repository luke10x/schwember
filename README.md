brew install assimp

Start native build locally:

    make clean run

WASM version in a container:

    docker-compose exec wasm bash

and there:

    make clean build
