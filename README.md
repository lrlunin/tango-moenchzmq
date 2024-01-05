# Dependecies installation
## easy (via package manager)
zeromq-dev
hdf5-dev
rapidjson-dev
fmt-dev

## not easy (self compiling)
### tango
also needed for pogo
### slsdetectorpackage
https://slsdetectorgroup.github.io/devdoc/installation.html
we need flags
-DSLS_DEVEL_HEADERS=ON
-DSLS_USE_MOENCH=ON
https://ess-dmsc.github.io/h5cpp/stable/users_guide/installing.html

### nexus
https://github.com/nexusformat/code
cmake .. -DENABLE_CXX=1