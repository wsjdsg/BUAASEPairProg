emcc   mancalaOperator.cpp  -O3  -s EXPORT_ES6=1 -s MODULARIZE=1 -s EXPORT_NAME=stubLoader -s EXPORTED_FUNCTIONS="['_mancala_operator','_malloc','_free']" -s EXPORTED_RUNTIME_METHODS="['cwrap']"  -o mancalaOperator.js