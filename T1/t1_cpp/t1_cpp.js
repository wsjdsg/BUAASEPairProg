import CPPModule from './lib.js'
var wasmInstance = await CPPModule();

function bocchi_shut_up(flag, seq, size) {
    const cppFuncPtr = wasmInstance.cwrap('bocchi_shut_up', 'number', ['number', 'number', 'number']);
    const malloc = wasmInstance.cwrap('malloc', 'number', ['number']);
    const free = wasmInstance.cwrap('free', null, ['number']);
    const dataPtr= malloc(seq.length*Uint32Array.BYTES_PER_ELEMENT)
    const memoryView = new Uint32Array(wasmInstance.HEAPU8.buffer,dataPtr,size)
    for(let i=0;i<size;i++){
        memoryView[i]=seq[i];
    }
    const result = cppFuncPtr(flag,dataPtr,size)
    free(dataPtr)
    return result
}
export {bocchi_shut_up}