# Development procedures dealing with git modules

Git modules are quite cumbersome to work with,
but so far I could not find any better solution.

I will try to document some common maintenace procedures
related to git-modules as I keep forgetting them.

**Add new module**

    git submodule add https://github.com/jkuhlmann/cgltf.git external/cgltf
    cd external/cgltf
    git checkout tags/v1.13
    cd ../..
    git submodule init
    git submodule update

**After project clone** run following to fetch the dependencies:

    