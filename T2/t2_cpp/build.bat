emcc   lib.cpp  -O3  -s EXPORT_ES6=1 -s MODULARIZE=1 -s EXPORT_NAME=stubLoader -s EXPORTED_FUNCTIONS="['_mancala_result','_malloc','_free']" -s EXPORTED_RUNTIME_METHODS="['cwrap']"  -o lib.js